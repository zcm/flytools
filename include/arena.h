#ifndef __ZCM_ARENA_H__
#define __ZCM_ARENA_H__

#include <stdint.h>

#include "common.h"
#include "jargon.h"

typedef struct fly_arena {
  uint8_t *next;
  uintptr_t end;
  alignas (max_align_t) uint8_t data[];
} arena;

#ifndef ARENA_DEFAULT_SIZE
#define ARENA_DEFAULT_SIZE (64 * 1024)
#endif

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

FLYAPI void arena_free(arena *a, void *ptr);

FLYAPI void arena_clear(arena *a);
//FLYAPI void arena_push_context(arena *a);
//FLYAPI void arena_pop_context(arena *a);

#endif
