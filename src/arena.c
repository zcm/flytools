#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

#define BLOCK_ALIGNMENT_PADDING (sizeof (arena) % alignof (max_align_t))

FLYAPI arena *arena_new(size_t size) {
  if (size < ARENA_MINIMUM_SIZE) {
    size = ARENA_DEFAULT_SIZE;
  }

  arena *ret = (arena *) malloc(
      sizeof (arena) + BLOCK_ALIGNMENT_PADDING
      + sizeof (struct arena_block) + size);

  if (ret) {
    fly_status = FLY_OK;

    ret->large = NULL;
    ret->block = (struct arena_block *)
      ((uintptr_t) (ret + 1) + BLOCK_ALIGNMENT_PADDING);

    ret->block->prev = NULL;
    ret->block->end = ret->block->data + size;

    arena_clear(ret);
  } else {
    fly_status = FLY_E_OUT_OF_MEMORY;
  }

  return ret;
}

static inline void arena_unwind(arena *a) {
  while (a->large) {
    free(a->large->data);
    a->large = a->large->prev;
  }
  while (a->block->prev) {
    struct arena_block *abp = a->block->prev;
    free(a->block);
    a->block = abp;
  }
}

FLYAPI void arena_del(arena *a) {
  arena_unwind(a);
  free(a);
}

FLYAPI void *arena_alloc(arena *a, size_t size) {
  return arena_alloc_aligned(a, size, alignof (max_align_t));
}

FLYAPI void *arena_alloc_aligned(arena *a, size_t size, size_t align) {
  uintptr_t aligned, next;

retry:
  aligned = ((uintptr_t) a->next + align - 1) & ~(align - 1);
  next = aligned + size;

  if (next > (uintptr_t) a->end) {
    size_t block_size = a->end - a->block->data;

    if (size > block_size || (size_t) (a->end - a->next) >= block_size / 64) {
      // ARENA_MINIMUM_SIZE should prevent this from recurring forever.
      struct arena_large_alloc *large =
        arena_alloc_type(a, struct arena_large_alloc);

      if (!large) {
        fly_status = FLY_E_OUT_OF_MEMORY;
        return NULL;
      }

      large->prev = a->large;
      return (a->large = large)->data = malloc(size);
    }

    size_t next_size = 2 * block_size;

    struct arena_block *next_block =
      (struct arena_block *) malloc(sizeof (struct arena_block) + next_size);

    if (!next_block) {
      fly_status = FLY_E_OUT_OF_MEMORY;
      return NULL;
    }

    next_block->prev = a->block;
    a->block = next_block;
    a->next = next_block->data;
    a->end = next_block->end = next_block->data + next_size;

    goto retry;
  }

  fly_status = FLY_OK;

  a->next = (uint8_t *) next;

  return (void *) aligned;
}

FLYAPI void *arena_calloc(arena *a, size_t num, size_t size) {
  return arena_calloc_aligned(a, num, size, alignof (max_align_t));
}

FLYAPI void *arena_calloc_aligned(
    arena *a, size_t num, size_t size, size_t align) {
  if (size && num > SIZE_MAX / size) {
    fly_status = FLY_E_TOO_BIG;
    return NULL;
  }

  size *= num;

  void *ret = arena_alloc_aligned(a, size, align);

  if (ret) {
    memset(ret, 0, size);
  }

  return ret;
}

FLYAPI void arena_free(arena *a, void *ptr) {
}

FLYAPI void arena_clear(arena *a) {
  arena_unwind(a);
  a->next = a->block->data;
  a->end = a->block->end;
  a->frame = NULL;
}

FLYAPI void arena_push(arena *a) {
  struct arena_context context = a->context;
  struct arena_frame *frame = arena_alloc_type(a, struct arena_frame);

  if (!frame) {
    fly_status = FLY_E_OUT_OF_MEMORY;
    return;
  } else {
    fly_status = FLY_OK;
  }

  frame->context = context;
  frame->prev = a->frame;
  a->frame = frame;
}

FLYAPI void arena_pop(arena *a) {
  if (!a->frame) {
    fly_status = FLY_EMPTY;
    return;
  }

  for (struct arena_large_alloc *current = a->large,
                                *target = a->frame->context.large;
       current && current != target;
       current = current->prev) {
    free(current->data);
  }

  fly_status = FLY_OK;
  a->context = a->frame->context;
  a->frame = a->frame->prev;
}

FLYAPI void arena_commit(arena *a) {
  if (!a->frame) {
    fly_status = FLY_EMPTY;
    return;
  }

  fly_status = a->frame ? FLY_OK : FLY_EMPTY;
  a->frame = a->frame->prev;
}
