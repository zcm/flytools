#ifndef __ZCM_ARENA_H__
#define __ZCM_ARENA_H__

#include <stdalign.h>
#include <stdint.h>

#include "common.h"
#include "jargon.h"

struct arena_block {
  struct arena_block *prev;
  uint8_t *end;
  alignas (max_align_t) uint8_t data[];
};

struct arena_large_alloc {
  struct arena_large_alloc *prev;
  void *data;
};

typedef struct arena {
  uint8_t *next;
  uint8_t *end;
  struct arena_large_alloc *large;
  struct arena_block *block;
} arena;

#define ARENA_DEFAULT_SIZE (64 * 1024)
#define ARENA_MINIMUM_SIZE (64 * sizeof (struct arena_large_alloc))

FLYAPI arena *arena_new(size_t size);
FLYAPI void arena_del(arena *a);

__attribute__((alloc_size(2)))
FLYAPI void *arena_alloc(arena *a, size_t size);

__attribute__((alloc_size(2)))
FLYAPI void *arena_alloc_aligned(arena *a, size_t size, size_t align);

__attribute__((alloc_size(2, 3)))
FLYAPI void *arena_calloc(arena *a, size_t num, size_t size);

__attribute__((alloc_size(2, 3)))
FLYAPI void *arena_calloc_aligned(
    arena *a, size_t num, size_t size, size_t align);

#define arena_alloc_type(a, T) \
  (T *) arena_alloc_aligned((a), sizeof (T), alignof (T))

#define arena_calloc_type(a, n, T) \
  (T *) arena_calloc_type((a), (n), sizeof (T), alignof (T))

FLYAPI void arena_free(arena *a, void *ptr);

FLYAPI void arena_clear(arena *a);
//FLYAPI void arena_push_context(arena *a);
//FLYAPI void arena_pop_context(arena *a);

#include "unjargon.h"

#endif
