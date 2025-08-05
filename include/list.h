/** @file list.h
 * This is the header file for the linked list abstract data type provided by
 * the Flytools. To use this type, simply include this header file in your
 * source code.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009
 * Zachary Murray, all rights reserved. This source file may not be
 * redistributed, in whole or in part. By viewing and/or using this file, you
 * agree not to redistribute the source files without the express written
 * permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 */

#ifndef __ZCM_LIST_H__
#define __ZCM_LIST_H__

#include <stdlib.h>
#include <stddef.h>

#include "common.h"
#include "generics.h"
#include "random.h"

#if defined(_MSC_VER)
#define restrict __restrict
#endif

struct sllistnode {
  void *data;
  struct sllistnode *next;
};

struct dllistnode {
  void *data;
  struct dllistnode *next;
  struct dllistnode *prev;
};

struct listkind;

#define LIST_DEFINITION             \
  INHERIT_STRUCT_DEF(FLYOBJ_SUPER)  \
  struct listkind *kind;            \
  size_t size;                      \
  rng64 rng;

typedef struct list {
  INHERIT_STRUCT_DEF(LIST_DEFINITION)
} list;

typedef struct arlist {
  UNIFY_OBJECT_DEF(list _list, LIST_DEFINITION)
  size_t capacity;
  void **items;
} arlist;

typedef struct deque {
  UNIFY_OBJECT_DEF(list _list, LIST_DEFINITION)
  size_t capacity;
  void **items;
  size_t start;
  size_t end;
} deque;

typedef struct dllist {
  UNIFY_OBJECT_DEF(list _list, LIST_DEFINITION)
  struct dllistnode *head;
} dllist;

typedef struct sllist {
  UNIFY_OBJECT_DEF(list _list, LIST_DEFINITION)
  struct sllistnode *head;
  struct sllistnode *last;
} sllist;

#undef LIST_DEFINITION

typedef struct listkind {
  size_t size;
  void (*init)(list *l);
  void (*destroy)(list *l);
  void *(*get)(list *l, ptrdiff_t i);
  void (*push)(list *l, void *data);
  void (*unshift)(list *l, void *data);
  void *(*pop)(list *l);
  void *(*shift)(list *l);
  void (*concat)(list *l1, list *l2);
  void (*append_array)(list *l, size_t n, void **items);
  void (*foreach)(list *l, int (*)(void *, size_t));
  void *(*find_first)(list *, int (*)(void *));
  void *(*discard)(list *, int (*)(void *));
  size_t (*discard_all)(list *, int (*)(void *), int (*)(void *, size_t));
  void (*shuffle)(list *);
  void (*sort)(list *, int(*)(const void *, const void *));
} listkind;

extern FLYAPI listkind *LISTKIND_ARRAY;
extern FLYAPI listkind *LISTKIND_DEQUE;
extern FLYAPI listkind *LISTKIND_DLINK;
extern FLYAPI listkind *LISTKIND_SLINK;

FLYAPI list *list_new();
FLYAPI list *list_new_kind(listkind *kind);
FLYAPI list *list_new_with(
    void *(*allocproc)(size_t), void (*freeproc)(void *));
FLYAPI list *list_new_kind_with(
    listkind *kind, void *(*allocproc)(size_t), void (*freeproc)(void *));
FLYAPI void list_del(list *l);
FLYAPI void *list_get(list *l, ptrdiff_t i);
FLYAPI void *list_pop(list *l);
FLYAPI void list_push(list *l, void *data);
FLYAPI void *list_shift(list *l);
FLYAPI void list_unshift(list *l, void *data);

FLYAPI void list_concat(list *l1, list *l2);
FLYAPI void list_concat_into(list *l1, list *l2);
FLYAPI void list_append_array(list *l, size_t n, void **items);

FLYAPI void *list_find_first(list *l, int (*matcher)(void *));
FLYAPI void list_foreach(list *l, int (*fn)(void *, size_t));
FLYAPI void *list_discard(list *l, int (*matcher)(void *));
FLYAPI size_t list_discard_all(
    list *l, int (*matcher)(void *), int (*fn)(void *, size_t));
FLYAPI void list_shuffle(list *l);
FLYAPI void list_sort(list *l, int (*comp)(const void *, const void *));

static inline enum FLY_STATUS list_bad_call(void *lp, ptrdiff_t i) {
  ptrdiff_t size;
  list *l = (list *) lp;

  if (!l) {
    return fly_status = FLY_E_NULL_PTR;
  }

  size = (ptrdiff_t) l->size;

  if (i >= size || i < -size) {
    return fly_status = FLY_E_OUT_OF_RANGE;
  }

  return fly_status = FLY_OK;
}

static inline void *arlist_get_unsafe(arlist *l, ptrdiff_t i) {
  if (i < 0) {
    i += l->size;
  }

  return l->items[i];
}

static inline void *deque_get_unsafe(deque *l, ptrdiff_t i) {
  if (i < 0) {
    i += l->size;
  }

  return l->items[(l->start + i) % l->capacity];
}

FLYAPI size_t arlist_grow(arlist *l, size_t new_elements);
FLYAPI void deque_reorient(deque *l, size_t grew_by);

static inline size_t arlist_ensure_capacity(arlist *l, size_t new_elements) {
  // 0 means out of memory. SIZE_MAX means no resize happened.
  if (new_elements > l->capacity - l->size) {
    return arlist_grow(l, new_elements);
  }
  return SIZE_MAX;
}

#define ARLIST_HAS_CAPACITY_OR_DIE_CAPTURE(l, new_elements, capture)  \
  if (!(capture arlist_ensure_capacity((arlist *) l, new_elements))) {  \
    return;  \
  }

#define ARLIST_HAS_CAPACITY_OR_DIE(l, new_elements) \
  ARLIST_HAS_CAPACITY_OR_DIE_CAPTURE(l, new_elements, )

// Using SIZE_MAX to mean no resize makes sense; the only time it can grow by
// SIZE_MAX is when it's initially empty, and no reorientation is needed then.
#define DEQUE_HAS_CAPACITY_OR_DIE(l, new_elements)  \
  size_t new_capacity, old_capacity = l->capacity;  \
  ARLIST_HAS_CAPACITY_OR_DIE_CAPTURE(l, new_elements, new_capacity =)  \
  if (new_capacity != SIZE_MAX) {  \
    deque_reorient(l, new_capacity - old_capacity);  \
  }

static inline void arlist_push_unsafe(arlist *l, void *data) {
  ARLIST_HAS_CAPACITY_OR_DIE(l, 1);

  l->items[l->size++] = data;
}

static inline void deque_push_unsafe(deque *l, void *data) {
  DEQUE_HAS_CAPACITY_OR_DIE(l, 1)

  l->size++;
  l->items[l->end] = data;
  l->end = (l->end + 1) % l->capacity;
}

static inline void *arlist_pop_unsafe(arlist *l) {
  return l->items[--(l->size)];
}

static inline void *deque_pop_unsafe(deque *l) {
  l->size--;
  return l->items[l->end = (l->end ? l->end : l->capacity) - 1];
}

static inline void arlist_unshift_unsafe(arlist *l, void *data) {
  ARLIST_HAS_CAPACITY_OR_DIE(l, 1)

  memmove(l->items + 1, l->items, l->size * sizeof (void *));

  l->size++;
  l->items[0] = data;

  fly_status = FLY_OK;
}

static inline void deque_unshift_unsafe(deque *l, void *data) {
  DEQUE_HAS_CAPACITY_OR_DIE(l, 1)

  l->size++;
  l->items[l->start = (l->start ? l->start : l->capacity) - 1] = data;
}

static inline void *arlist_shift_unsafe(arlist *l) {
  void *ret = l->items[0];
  memmove(l->items, l->items + 1, --(l->size) * sizeof (void *));
  return ret;
}

static inline void *deque_shift_unsafe(deque *l) {
  void *ret = l->items[l->start];

  l->size--;
  l->start = (l->start + 1) % l->capacity;

  return ret;
}

FLYAPI void *arlist_get(arlist *l, ptrdiff_t i);
FLYAPI void arlist_push(arlist *l, void *data);
FLYAPI void arlist_unshift(arlist *l, void *data);
FLYAPI void *arlist_pop(arlist *l);
FLYAPI void *arlist_shift(arlist *l);
FLYAPI void arlist_concat(arlist *l1, arlist *l2);
FLYAPI void arlist_shuffle(arlist *l);
FLYAPI void **arlist_draw(arlist * restrict l, void ** restrict cursor);
FLYAPI void *arlist_pick(arlist *l);
FLYAPI void arlist_sort(arlist *l, int (*comp)(const void *, const void *));

FLYAPI void *deque_get(deque *l, ptrdiff_t i);
FLYAPI void deque_push(deque *l, void *data);
FLYAPI void deque_unshift(deque *l, void *data);
FLYAPI void *deque_pop(deque *l);
FLYAPI void *deque_shift(deque *l);
FLYAPI void deque_concat(deque *l1, deque *l2);
FLYAPI void deque_shuffle(deque *l);
FLYAPI void **deque_draw(deque * restrict l, void ** restrict cursor);
FLYAPI void *deque_pick(deque *l);
FLYAPI void deque_sort(deque *l, int (*comp)(const void *, const void *));

FLYAPI void *dllist_get(dllist *l, ptrdiff_t i);
FLYAPI void dllist_push(dllist *l, void *data);
FLYAPI void dllist_unshift(dllist *l, void *data);
FLYAPI void *dllist_pop(dllist *l);
FLYAPI void *dllist_shift(dllist *l);
FLYAPI void dllist_concat(dllist *l1, dllist *l2);
FLYAPI void dllist_shuffle(dllist *l);
FLYAPI void dllist_sort(dllist *l, int (*comp)(const void *, const void *));

FLYAPI void *sllist_get(sllist *l, ptrdiff_t i);
FLYAPI void sllist_push(sllist *l, void *data);
FLYAPI void sllist_unshift(sllist *l, void *data);
FLYAPI void *sllist_pop(sllist *l);
FLYAPI void *sllist_shift(sllist *l);
FLYAPI void sllist_concat(sllist *l1, sllist *l2);
FLYAPI void sllist_shuffle(sllist *l);
FLYAPI void sllist_sort(sllist *l, int (*comp)(const void *, const void *));

#if defined(_MSC_VER)
#undef restrict
#endif

#endif
