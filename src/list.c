/** @file list.c
 * This file defines the linked list type provided by the Flytools. The
 * structure is completely dynamic; no static allocation is possible. Each list
 * is circular in nature with a single sentinel node, and each link provides a
 * next link and a prev link (for doubly linked lists) or just a next link (for
 * singly linked lists). Other list kinds might not provide a sentinel node.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2011
 * Zachary Murray, all rights reserved. This source file may not be
 * redistributed, in whole or in part. By viewing and/or using this file, you
 * agree not to redistribute the source files without the express written
 * permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 * @date 11/13/2008 19:06:55 EST (creation, sllist.c)
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "list.h"

typedef struct sllistnode sllistnode;
typedef struct dllistnode dllistnode;

static size_t arlist_remove_all(
    arlist *l, int (*matcher)(void *), int (*fn)(void *, size_t));
static size_t dllist_remove_all(
    dllist *l, int (*matcher)(void *), int (*fn)(void *, size_t));
static size_t sllist_remove_all(
    sllist *l, int (*matcher)(void *), int (*fn)(void *, size_t));

static void arlist_init(arlist *l);
static void arlist_del(arlist *l);
static inline void *_unsafe_arlist_get(arlist *l, size_t i);
static void _unsafe_arlist_push(arlist *l, void *data);
static void _unsafe_arlist_unshift(arlist *l, void *data);
static void *_unsafe_arlist_pop(arlist *l);
static void *_unsafe_arlist_shift(arlist *l);
static void _unsafe_arlist_foreach(arlist *l, int (*)(void *, size_t));
static void *_unsafe_arlist_find_first(arlist *l, int (*matcher)(void *));
static void *arlist_remove_first(arlist *l, int (*matcher)(void *));

static void deque_init(deque *l);
static inline void *_unsafe_deque_get(deque *l, size_t i);
static void _unsafe_deque_push(deque *l, void *data);
static void _unsafe_deque_unshift(deque *l, void *data);
static void *_unsafe_deque_pop(deque *l);
static void *_unsafe_deque_shift(deque *l);
//static void _unsafe_deque_foreach(deque *l, int (*)(void *, size_t));
//static void *_unsafe_deque_find_first(deque *l, int (*matcher)(void *));
//static void *deque_remove_first(deque *l, int (*matcher)(void *));

static void dllist_init(dllist *l);
static void dllist_del(dllist *l);
static void _unsafe_dllist_push(dllist *l, void *data);
static void _unsafe_dllist_unshift(dllist *l, void *data);
static void *_unsafe_dllist_pop(dllist *l);
static void *_unsafe_dllist_shift(dllist *l);
static void *dllist_remove_first(dllist *l, int (*matcher)(void *));

static void sllist_init(sllist *l);
static void sllist_del(sllist *l);
static inline void *_unsafe_sllist_get(sllist *l, size_t i);
static void _unsafe_sllist_push(sllist *l, void *data);
static void _unsafe_sllist_unshift(sllist *l, void *data);
static void *_unsafe_sllist_pop(sllist *l);
static void *_unsafe_sllist_shift(sllist *l);
static void _unsafe_sllist_foreach(sllist *l, int (*)(void *, size_t));
static void *_unsafe_sllist_find_first(sllist *l, int (*matcher)(void *));
static void *sllist_remove_first(sllist *l, int (*matcher)(void *));

#ifdef __TURBOC__
#define ASSIGN_STATIC_PTR(KIND) \
  static listkind KIND##_IMPL; \
  FLYAPI listkind *KIND = &KIND##_IMPL; \
  static listkind KIND##_IMPL =
#else
#define ASSIGN_STATIC_PTR(KIND) \
  FLYAPI listkind *KIND = &(listkind)
#endif

ASSIGN_STATIC_PTR(LISTKIND_ARRAY) {
  sizeof (arlist),
  (void *) &arlist_init,
  (void *) &arlist_del,
  (void *) &_unsafe_arlist_get,
  (void *) &_unsafe_arlist_push,
  (void *) &_unsafe_arlist_unshift,
  (void *) &_unsafe_arlist_pop,
  (void *) &_unsafe_arlist_shift,
  (void *) &arlist_concat,
  (void *) &_unsafe_arlist_foreach,
  (void *) &_unsafe_arlist_find_first,
  (void *) &arlist_remove_first,
  (void *) &arlist_remove_all,
};

ASSIGN_STATIC_PTR(LISTKIND_DEQUE) {
  sizeof (deque),
  (void *) &deque_init,
  (void *) &arlist_del,  /* not a typo */
  (void *) &_unsafe_deque_get,
  (void *) &_unsafe_deque_push,
  (void *) &_unsafe_deque_unshift,
  (void *) &_unsafe_deque_pop,
  (void *) &_unsafe_deque_shift,
  (void *) &deque_concat,
  (void *) NULL, //&_unsafe_deque_foreach,
  (void *) NULL, //&_unsafe_deque_find_first,
  (void *) NULL, //&deque_remove_first,
  (void *) NULL, //&deque_remove_all,
};

ASSIGN_STATIC_PTR(LISTKIND_DLINK) {
  sizeof (dllist),
  (void *) &dllist_init,
  (void *) &dllist_del,
  (void *) &_unsafe_sllist_get,
  (void *) &_unsafe_dllist_push,
  (void *) &_unsafe_dllist_unshift,
  (void *) &_unsafe_dllist_pop,
  (void *) &_unsafe_dllist_shift,
  (void *) &dllist_concat,
  (void *) &_unsafe_sllist_foreach,  /* not a typo */
  (void *) &_unsafe_sllist_find_first,
  (void *) &dllist_remove_first,
  (void *) &dllist_remove_all,
};

ASSIGN_STATIC_PTR(LISTKIND_SLINK) {
  sizeof (sllist),
  (void *) &sllist_init,
  (void *) &sllist_del,
  (void *) &_unsafe_sllist_get,
  (void *) &_unsafe_sllist_push,
  (void *) &_unsafe_sllist_unshift,
  (void *) &_unsafe_sllist_pop,
  (void *) &_unsafe_sllist_shift,
  (void *) &sllist_concat,
  (void *) &_unsafe_sllist_foreach,
  (void *) &_unsafe_sllist_find_first,
  (void *) &sllist_remove_first,
  (void *) &sllist_remove_all,
};

#undef ASSIGN_STATIC_PTR

#if defined(__STRICT_ANSI__)
#define inline
#elif defined(_MSC_VER)
#define inline __inline
#define restrict __restrict
#elif __STDC_VERSION__ < 199901L
#define restrict
#endif

#ifdef __TURBOC__
#define inline
#endif

FLYAPI list *list_new() {
  return list_new_kind_with(
      LISTKIND_DLINK,
      flyobj_get_default_allocproc(), flyobj_get_default_freeproc());
}

FLYAPI list *list_new_kind(listkind *kind) {
  return list_new_kind_with(
      kind, flyobj_get_default_allocproc(), flyobj_get_default_freeproc());
}

FLYAPI list *list_new_with(
    void *(*allocproc)(size_t), void (*freeproc)(void *)) {
  return list_new_kind_with(LISTKIND_DLINK, allocproc, freeproc);
}

FLYAPI list *list_new_kind_with(
    listkind *kind, void *(*allocproc)(size_t), void (*freeproc)(void *)) {
  list *ret = allocproc(kind->size);

  if(ret != NULL) {
    fly_status = FLY_OK;

    flyobj_init((struct flyobj *) ret, allocproc, freeproc);
    ret->kind = kind;
    kind->init(ret);
  } else {
    fly_status = FLY_E_OUT_OF_MEMORY;
  }
  return ret;
}

FLYAPI void list_del(list *l) {
  if (l != NULL) {
    fly_status = FLY_OK;

    l->kind->destroy(l);
    flyobj_del((struct flyobj *) l);
  } else {
    fly_status = FLY_E_NULL_PTR;
  }
}

static inline void *_unsafe_arlist_get(arlist *l, size_t i) {
  return l->items[i];
}

static inline void *_unsafe_deque_get(deque *l, size_t i) {
  return l->items[(l->start + i) % l->capacity];
}

static inline sllistnode *_unsafe_sllist_get_node(sllist *l, size_t i) {
  sllistnode *current = l->head->next;

  while (i) {
    current = current->next;
    i--;
  }

  return current;
}

static inline void *_unsafe_sllist_get(sllist *l, size_t i) {
  return _unsafe_sllist_get_node(l, i)->data;
}

#define CHECK_LIST_BOUNDS(l, i)       \
  if (!l) {                           \
    fly_status = FLY_E_NULL_PTR;      \
    return NULL;                      \
  }                                   \
  if (i >= l->size) {                 \
    fly_status = FLY_E_OUT_OF_RANGE;  \
    return NULL;                      \
  }                                   \
  fly_status = FLY_OK;                \

FLYAPI void *list_get(list *l, size_t i) {
  CHECK_LIST_BOUNDS(l, i);
  return l->kind->get(l, i);
}

FLYAPI void *arlist_get(arlist *l, size_t i) {
  CHECK_LIST_BOUNDS(l, i);
  return l->items[i];
}

FLYAPI void *dllist_get(dllist *l, size_t i) {
  CHECK_LIST_BOUNDS(l, i);
  return _unsafe_sllist_get((sllist *) l, i);
}

FLYAPI void *sllist_get(sllist *l, size_t i) {
  CHECK_LIST_BOUNDS(l, i);
  return _unsafe_sllist_get(l, i);
}

// struct member interaction

static inline void *list_end_remove_op(
    void * restrict vl, void *(*remove)(void *)) {
  list *l = (list *) vl;

  if (l->size > 0) {
    fly_status = FLY_OK;
    return remove(l);
  }

  fly_status = FLY_EMPTY;
  return NULL;
}

FLYAPI void *list_pop(list *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, l->kind->pop);
}

FLYAPI void *list_shift(list *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, l->kind->shift);
}

FLYAPI void list_push(list *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  fly_status = FLY_OK;
  l->kind->push(l, data);
}

FLYAPI void list_unshift(list *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  fly_status = FLY_OK;
  l->kind->unshift(l, data);
}

FLYAPI void list_concat(list *l1, list *l2) {
  if (!(l1 && l2)) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l1->kind == l2->kind) {
    l1->kind->concat(l1, l2);
  } else {
    /* TODO: different kinds. for now, concat into l1... */
    /* but in the future... make a conglomerate list     */
    list_concat_into(l1, l2); // TODO: change to conglomerate list
  }
}

FLYAPI void list_concat_into(list *l1, list *l2) {
  if (l1 == NULL || l2 == NULL) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  if (l1->kind == l2->kind) {
    // silly user, they're the same kind! use list_concat instead (it's faster)
    list_concat(l1, l2);
    return;
  }

  fly_status = FLY_OK;

  while (l2->size > 0) {
    list_unshift(l1, list_pop(l2));
  }

  list_del(l2);
}

static void *_unsafe_arlist_find_first(arlist *l, int (*matcher)(void *)) {
  size_t i;
  void **each = l->items;

  for (i = l->size; i; i--, each++) {
    if (matcher(*each)) {
      fly_status = FLY_OK;
      return *each;
    }
  }

  fly_status = FLY_NOT_FOUND;
  return NULL;
}

static void *_unsafe_sllist_find_first(sllist *l, int (*matcher)(void *)) {
  sllistnode *head = l->head,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      fly_status = FLY_OK;
      return current->data;
    }
    current = current->next;
  }

  fly_status = FLY_NOT_FOUND;
  return NULL;
}

FLYAPI void *list_find_first(list *l, int (*matcher)(void *)) {
  if (!(l && matcher)) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  if (!l->size) {
    fly_status = FLY_NOT_FOUND;
    return NULL;
  }

  return l->kind->find_first(l, matcher);
}

static void *arlist_remove_first(arlist *l, int (*matcher)(void *)) {
  size_t i = 0;

  while (i < l->size) {
    if (matcher(l->items[i++])) {
      void *ret = l->items[i - 1];

      memmove(
          l->items + i - 1, l->items + i,
          (l->size - i) * sizeof (void *));

      l->size--;

      fly_status = FLY_OK;
      return ret;
    }
  }

  fly_status = FLY_NOT_FOUND;
  return NULL;
}

static inline void dllist_stitch(dllistnode *current) {
  current->prev->next = current->next;
  current->next->prev = current->prev;
}

static inline void sllist_stitch(
    sllistnode *current, sllistnode *prev, sllist * restrict l) {
  if (current == l->last) {
    l->last = prev;
  }

  prev->next = current->next;
}

static void *dllist_remove_first(dllist *l, int (*matcher)(void *)) {
  dllistnode *head = l->head,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      void *data = current->data;

      dllist_stitch(current);

      l->del(current);
      l->size--;

      fly_status = FLY_OK;
      return data;
    }

    current = current->next;
  }

  fly_status = FLY_NOT_FOUND;
  return NULL;
}

static void *sllist_remove_first(sllist *l, int (*matcher)(void *)) {
  sllistnode *head = l->head,
             *prev = head,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      void *data = current->data;

      sllist_stitch(current, prev, l);

      l->del(current);
      l->size--;

      fly_status = FLY_OK;
      return data;
    }

    current = (prev = current)->next;
  }

  fly_status = FLY_NOT_FOUND;
  return NULL;
}

FLYAPI void *list_remove_first(list *l, int (*matcher)(void *)) {
  if (!(l && matcher)) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  if (!l->size) {
    fly_status = FLY_NOT_FOUND;
    return NULL;
  }

  return l->kind->remove_first(l, matcher);
}

static void _unsafe_arlist_foreach(arlist *l, int (*fn)(void *, size_t)) {
  size_t i = 0;

  while (i++ < l->size && !fn(l->items[i - 1], i - 1));
}

static void _unsafe_sllist_foreach(sllist *l, int (*fn)(void *, size_t)) {
  size_t i = 0;
  sllistnode *current, *head;

  current = head = l->head;

  while (head != (current = current->next) && !fn(current->data, i++));
}

FLYAPI void list_foreach(list *l, int (*fn)(void *, size_t)) {
  if (!(l && fn)) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  l->kind->foreach(l, fn);
}

static size_t arlist_remove_all(
    arlist *l, int (*matcher)(void *), int (*fn)(void *, size_t)) {
  size_t total_removed = 0, i = 0;
  void **left;

  while (i < l->size) {
    if (matcher(l->items[i])) {
      goto removal_start;
    }
    i++;
  }

  return 0;  /* loop exited normally - nothing found */

  while (i < l->size) {
    if (!matcher(l->items[i])) {
      i++;
    } else {
      memmove(left, left + total_removed,
          ((l->items + i) - (left + total_removed)) * sizeof (void *));

removal_start:
      left = l->items + i - total_removed;

      do {
        total_removed++;

        if (fn(l->items[i], i)) {
          goto final_move;
        }
      } while (++i < l->size && matcher(l->items[i]));
    }
  }

final_move:
  memmove(left, left + total_removed,
      ((l->items + l->size) - (left + total_removed)) * sizeof (void *));

  l->size -= total_removed;
  return total_removed;
}

static size_t dllist_remove_all(
    dllist *l, int (*matcher)(void *), int (*fn)(void *, size_t)) {
  size_t i = 0, total_removed = 0;
  dllistnode *head = l->head,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      void *data = current->data;
      dllistnode *next = current->next;

      dllist_stitch(current);

      l->del(current);

      ++total_removed;

      if (fn(data, i++)) {
        break;
      }

      current = next;
    } else {
      current = current->next;
      ++i;
    }
  }

  l->size -= total_removed;
  return total_removed;
}

static size_t sllist_remove_all(
    sllist *l, int (*matcher)(void *), int (*fn)(void *, size_t)) {
  size_t i = 0, total_removed = 0;
  sllistnode *head = l->head,
             *prev = head,
             *chopping_block = NULL,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      void *data = current->data;
      sllistnode *next = current->next;

      sllist_stitch(current, prev, l);

      // Defer freeing because of need to modify prev
      if (chopping_block) {
        l->del(chopping_block);
      }
      chopping_block = current;

      ++total_removed;

      if (fn(data, i++)) {
        break;
      }

      current = next;
    } else {
      current = (prev = current)->next;
      ++i;
    }
  }

  if (chopping_block) {
    l->del(chopping_block);
  }

  l->size -= total_removed;
  return total_removed;
}

static int do_nothing(void *unused_data, size_t unused_i) {
  return 0;
}

FLYAPI size_t list_remove_all(
    list *l, int (*matcher)(void *), int (*fn)(void *, size_t)) {
  if (!(l && matcher)) {
    fly_status = FLY_E_NULL_PTR;
    return 0;
  }

  return l->kind->remove_all(l, matcher, fn ? fn : &do_nothing);
}

#define ARLIST_DEFAULT_CAPACITY 8

static inline void arlist_init(arlist *l) {
  l->size = 0;
  l->capacity = 0;
  l->items = NULL;
}

static void deque_init(deque *l) {
  arlist_init((arlist *) l);
  l->start = l->end = 0;
}

static void arlist_del(arlist *l) {
  l->del(l->items);
}

static size_t arlist_grow(arlist *l, size_t new_elements) {
  void *next_elements;
  size_t delta = l->capacity >> 1;

  if (delta < new_elements) {
    delta = new_elements;
  }

  l->capacity += delta;

  if (l->capacity < ARLIST_DEFAULT_CAPACITY) {
    l->capacity = ARLIST_DEFAULT_CAPACITY;
  }

  if (!(next_elements = realloc(l->items, l->capacity * sizeof (void *)))) {
    // realloc() failed, l->items unchanged
    return 0;
  }

  l->items = next_elements;
  return delta;
}

static void deque_reorient(deque *l, size_t grew_by) {
  if (l->start < l->end || !l->size) {
    return;
  }

  if (!l->end) {
    l->end = l->size;
    return;
  }

  if (grew_by >= l->end) {
    memcpy(l->items + l->size, l->items, l->end * sizeof (void *));
    l->end = (l->end + l->size) % l->capacity;
  } else {
    memcpy(
        l->items + l->size, l->items,
        grew_by * sizeof (void *));
    memcpy(
        l->items, l->items + grew_by,
        (l->end - grew_by) * sizeof (void *));
    l->end -= grew_by;
  }
}

#undef ARLIST_DEFAULT_CAPACITY

// 0 means out of memory. SIZE_MAX means no resize happened.
static inline size_t arlist_ensure_capacity(arlist *l, size_t new_elements) {
  if (l->size + new_elements > l->capacity) {
    return arlist_grow(l, new_elements);
  }
  return SIZE_MAX;
}

#define ARLIST_HAS_CAPACITY_OR_DIE(l, new_elements, capture)            \
  if (!(capture arlist_ensure_capacity((arlist *) l, new_elements))) {  \
    fly_status = FLY_E_OUT_OF_MEMORY;                    \
    return;                                          \
  }

// Using SIZE_MAX to mean no resize makes sense; the only time it can grow by
// SIZE_MAX is when it's initially empty, and no reorientation is needed then.
#define DEQUE_HAS_CAPACITY_OR_DIE(l, new_elements)  \
  size_t grew_by;  \
  ARLIST_HAS_CAPACITY_OR_DIE(l, new_elements, grew_by =)  \
  if (grew_by != SIZE_MAX) {  \
    deque_reorient(l, grew_by);  \
  }

static void _unsafe_arlist_push(arlist *l, void *data) {
  ARLIST_HAS_CAPACITY_OR_DIE(l, 1, );

  l->items[l->size++] = data;
}

FLYAPI void arlist_push(arlist *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  _unsafe_arlist_push(l, data);
}

static void _unsafe_deque_push(deque *l, void *data) {
  DEQUE_HAS_CAPACITY_OR_DIE(l, 1)

  l->size++;
  l->items[l->end] = data;
  l->end = (l->end + 1) % l->capacity;
}

FLYAPI void deque_push(deque *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  _unsafe_deque_push(l, data);
}

static void _unsafe_arlist_unshift(arlist *l, void *data) {
  ARLIST_HAS_CAPACITY_OR_DIE(l, 1, )

  memmove(l->items + 1, l->items, l->size * sizeof (void *));

  l->size++;
  l->items[0] = data;

  fly_status = FLY_OK;
}

FLYAPI void arlist_unshift(arlist *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  _unsafe_arlist_unshift(l, data);
}

static void _unsafe_deque_unshift(deque *l, void *data) {
  DEQUE_HAS_CAPACITY_OR_DIE(l, 1)

  l->size++;
  l->items[l->start = (l->start ? l->start : l->capacity) - 1] = data;
}

FLYAPI void deque_unshift(deque *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  _unsafe_deque_unshift(l, data);
}

static void *_unsafe_arlist_pop(arlist *l) {
  return l->items[--(l->size)];
}

FLYAPI void *arlist_pop(arlist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, &_unsafe_arlist_pop);
}

static void *_unsafe_deque_pop(deque *l) {
  l->size--;
  return l->items[l->end = (l->end ? l->end : l->capacity) - 1];
}

FLYAPI void *deque_pop(deque *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, &_unsafe_deque_pop);
}

static void *_unsafe_arlist_shift(arlist *l) {
  void *ret;

  ret = l->items[0];

  memmove(l->items, l->items + 1, --(l->size) * sizeof (void *));

  return ret;
}

FLYAPI void *arlist_shift(arlist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, &_unsafe_arlist_shift);
}

static void *_unsafe_deque_shift(deque *l) {
  void *ret = l->items[l->start];

  l->size--;
  l->start = (l->start + 1) % l->capacity;

  return ret;
}

FLYAPI void *deque_shift(deque *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, &_unsafe_deque_shift);
}

FLYAPI void arlist_concat(arlist * restrict l1, arlist * restrict l2) {
  ARLIST_HAS_CAPACITY_OR_DIE(l1, l2->size, )

  memcpy(l1->items + l1->size, l2->items, l2->size * sizeof (void *));

  l1->size += l2->size;
  fly_status = FLY_OK;
}

FLYAPI void deque_concat(deque * restrict l1, deque * restrict l2) {
  size_t remaining, segment1, segment2;

  DEQUE_HAS_CAPACITY_OR_DIE(l1, l2->size)

  l1->size += remaining = l2->size;

  while (remaining) {
    segment1 = l1->capacity - l1->end;
    segment2 = l2->capacity - l2->start;

    if (segment2 < segment1) {
      segment1 = segment2;
    }
    if (segment1 > remaining) {
      segment1 = remaining;
    }

    memcpy(l1->items + l1->end, l2->start, segment1 * sizeof (void *));
    l1->end = (l1->end + segment1) % l1->capacity;
    remaining -= segment1;
  }

  fly_status = FLY_OK;
}

#undef ARLIST_HAS_CAPACITY_OR_DIE

static inline void dllistnode_head_init(dllistnode * restrict head) {
  head->data = NULL;
  head->next = head;
  head->prev = head;
}

static inline void sllistnode_head_init(sllistnode * restrict head) {
  head->data = NULL;
  head->next = head;
}

static inline void *listnode_alloc(void * restrict l, size_t node_size) {
  void *node;

  if (!(node = ((struct flyobj *) l)->alloc(node_size))) {
    fly_status = FLY_E_OUT_OF_MEMORY;
  }

  return node;
}

static void dllist_init(dllist *l) {
  l->size = 0;

  if ((l->head = l->alloc(sizeof (dllistnode)))) {
    dllistnode_head_init(l->head);
  } else {
    fly_status = FLY_E_OUT_OF_MEMORY;
  }
}

static void dllist_del(dllist *l) {
  while (l->size > 0) {
    dllist_pop(l);
  }

  l->del(l->head);
}

static void _unsafe_dllist_push(dllist *l, void *data) {
  dllistnode *node;

  if (!(node = listnode_alloc(l, sizeof (dllistnode)))) {
    return;
  }

  node->data = data;
  node->next = l->head;
  node->prev = l->head->prev;
  l->head->prev->next = node;
  l->head->prev = node;
  l->size++;
}

FLYAPI void dllist_push(dllist *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  _unsafe_dllist_push(l, data);
}

static void _unsafe_dllist_unshift(dllist *l, void *data) {
  dllistnode *node;

  if (!(node = listnode_alloc(l, sizeof (dllistnode)))) {
    return;
  }

  node->data = data;
  node->next = l->head->next;
  node->prev = l->head;
  l->head->next->prev = node;
  l->head->next = node;
  l->size++;
}

FLYAPI void dllist_unshift(dllist *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  _unsafe_dllist_unshift(l, data);
}

static void *_unsafe_dllist_pop(dllist *l) {
  void *ret;

  dllistnode *node = l->head->prev;
  node->prev->next = l->head;
  l->head->prev = node->prev;
  l->size--;
  ret = node->data;
  l->del(node);

  return ret;
}

FLYAPI void *dllist_pop(dllist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, &_unsafe_dllist_pop);
}

static void *_unsafe_dllist_shift(dllist *l) {
  void *ret;

  dllistnode *node = l->head->next;
  node->next->prev = l->head;
  l->head->next = node->next;
  l->size--;
  ret = node->data;
  l->del(node);

  return ret;
}

FLYAPI void *dllist_shift(dllist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, _unsafe_dllist_shift);
}

FLYAPI void dllist_concat(dllist *l1, dllist *l2) {
  size_t newsize = l1->size + l2->size;

  l1->head->prev->next = l2->head->next;
  l2->head->next->prev = l1->head->prev;
  l2->head->prev->next = l1->head;
  l1->head->prev = l2->head->prev;
  l1->size = newsize;
  // set the size to 0 so that destroy avoids destroying the inner nodes
  // (because they are now part of the first list's structure)
  l2->size = 0;

  list_del((list *) l2);
}

static void sllist_init(sllist *l) {
  l->size = 0;

  if ((l->head = l->last = l->alloc(sizeof (dllistnode)))) {
    sllistnode_head_init(l->head);
  } else {
    fly_status = FLY_E_OUT_OF_MEMORY;
  }
}

void sllist_del(sllist *l) {
  while (l->size > 0) {
    sllist_pop(l);
  }

  l->del(l->head);
}

static void _unsafe_sllist_push(sllist *l, void *data) {
  sllistnode *node;

  if (!(node = listnode_alloc(l, sizeof (sllistnode)))) {
    return;
  }

  node->data = data;
  node->next = l->head;
  l->last->next = node;
  l->last = node;

  l->size++;
}

FLYAPI void sllist_push(sllist *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  _unsafe_sllist_push(l, data);
}

static void _unsafe_sllist_unshift(sllist *l, void *data) {
  sllistnode *node;

  if (!(node = listnode_alloc(l, sizeof (sllistnode)))) {
    return;
  }

  node->data = data;
  node->next = l->head->next;
  l->head->next = node;

  if (l->size == 0) {
    l->last = node;
  }

  l->size++;
}

FLYAPI void sllist_unshift(sllist *l, void *data) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }
  _unsafe_sllist_unshift(l, data);
}

static void *_unsafe_sllist_pop(sllist *l) {
  void *ret = l->last->data;

  l->del(l->last);  // Do not call if l is empty! You'll free the head node!
  l->last = _unsafe_sllist_get_node(l, (l->size - 2) % l->size);
  l->last->next = l->head;
  l->size--;

  return ret;
}

FLYAPI void *sllist_pop(sllist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, &_unsafe_sllist_pop);
}

static void *_unsafe_sllist_shift(sllist *l) {
  void *ret;

  sllistnode *node = l->head->next;
  l->head->next = node->next;

  l->size--;

  if (l->size == 0) {
    l->last = l->head;
  }

  ret = node->data;
  l->del(node);

  return ret;
}

FLYAPI void *sllist_shift(sllist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  return list_end_remove_op(l, &_unsafe_sllist_shift);
}

FLYAPI void sllist_concat(sllist *l1, sllist *l2) {
  size_t newsize = l1->size + l2->size;

  l1->last->next = l2->head->next;
  l1->last = l2->last;
  l1->last->next = l1->head;

  l1->size = newsize;
  // set the size to 0 so that destroy avoids destroying the inner nodes
  // (because they are now part of the first list's structure)
  l2->size = 0;

  list_del((list *) l2);
}

#if defined(__STRICT_ANSI__)
#undef inline
#elif defined(_MSC_VER)
#undef inline
#undef restrict
#endif

