#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

FLYAPI arena *arena_new(size_t size) {
  if (!size) {
    size = ARENA_DEFAULT_SIZE;
  }

  arena *ret = (arena *) malloc(sizeof (arena) + size);

  if (ret) {
    fly_status = FLY_OK;
    arena_clear(ret);
  } else {
    fly_status = FLY_E_OUT_OF_MEMORY;
  }

  return ret;
}

FLYAPI void arena_del(arena *a) {
  free(a);
}

FLYAPI void *arena_alloc(arena *a, size_t size) {
  return arena_alloc_aligned(a, size, alignof (max_align_t));
}

FLYAPI void *arena_alloc_aligned(arena *a, size_t size, size_t align) {
  size = (size + align - 1) & ~(align - 1);

  void *ret = a->next;
  a->next += size;

  return ret;
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
  a->next = (uint8_t *) (a + 1);
}
