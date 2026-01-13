#ifndef __ZCM_ARENA_H__
#define __ZCM_ARENA_H__

#include <stdint.h>

typedef struct fly_arena {
  uint8_t *data, *next;
} arena;

FLYAPI arena *arena_new();
FLYAPI void arena_del(arena *a);

__attribute__((alloc_size(2)))
FLYAPI void *arena_alloc(arena *a, size_t size);

__attribute__((alloc_size(2, 3)))
FLYAPI void arena_calloc(arena *a, size_t num, size_t size);

FLYAPI void arena_free(arena *a, void *ptr);

FLYAPI void arena_clear(arena *a);
FLYAPI void arena_push_context(arena *a);
FLYAPI void arena_pop_context(arena *a);

#endif
