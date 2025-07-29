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
#include "internal/common.h"

typedef struct sllistnode sllistnode;
typedef struct dllistnode dllistnode;


static void arlist_init(arlist *l);
static void arlist_del(arlist *l);
static inline void *_unsafe_arlist_get(arlist *l, ptrdiff_t i);
static void _unsafe_arlist_push(arlist *l, void *data);
static void _unsafe_arlist_unshift(arlist *l, void *data);
static void *_unsafe_arlist_pop(arlist *l);
static void *_unsafe_arlist_shift(arlist *l);
static void _unsafe_arlist_append_array(arlist *l, size_t n, void **items);
static void _unsafe_arlist_foreach(arlist *l, int (*)(void *, size_t));
static void *_unsafe_arlist_find_first(arlist *l, int (*matcher)(void *));
static void *_unsafe_arlist_discard(arlist *l, int (*matcher)(void *));
static size_t _unsafe_arlist_discard_all(
    arlist *l, int (*matcher)(void *), int (*fn)(void *, size_t));
static void _unsafe_arlist_shuffle(arlist *l);
static void _unsafe_arlist_sort(
    arlist *l, int (*comp)(const void *, const void *));

static void deque_init(deque *l);
static inline void *_unsafe_deque_get(deque *l, ptrdiff_t i);
static void _unsafe_deque_push(deque *l, void *data);
static void _unsafe_deque_unshift(deque *l, void *data);
static void *_unsafe_deque_pop(deque *l);
static void *_unsafe_deque_shift(deque *l);
static void _unsafe_deque_append_array(deque *l, size_t n, void **items);
static void _unsafe_deque_foreach(deque *l, int (*)(void *, size_t));
static void *_unsafe_deque_find_first(deque *l, int (*matcher)(void *));
static void *_unsafe_deque_discard(deque *l, int (*matcher)(void *));
static size_t _unsafe_deque_discard_all(
    deque *l, int (*matcher)(void *), int (*fn)(void *, size_t));
static void _unsafe_deque_shuffle(deque *l);
static void _unsafe_deque_sort(
    deque *l, int (*comp)(const void *, const void *));

static void dllist_init(dllist *l);
static void dllist_del(dllist *l);
static inline void *_unsafe_dllist_get(dllist *l, ptrdiff_t i);
static void _unsafe_dllist_push(dllist *l, void *data);
static void _unsafe_dllist_unshift(dllist *l, void *data);
static void *_unsafe_dllist_pop(dllist *l);
static void *_unsafe_dllist_shift(dllist *l);
static void _unsafe_dllist_append_array(dllist *l, size_t n, void **items);
static void *_unsafe_dllist_discard(dllist *l, int (*matcher)(void *));
static size_t _unsafe_dllist_discard_all(
    dllist *l, int (*matcher)(void *), int (*fn)(void *, size_t));
static void _unsafe_dllist_shuffle(dllist *l);
static void _unsafe_dllist_sort(
    dllist *l, int (*comp)(const void *, const void *));

static void sllist_init(sllist *l);
static void sllist_del(sllist *l);
static inline void *_unsafe_sllist_get(sllist *l, ptrdiff_t i);
static void _unsafe_sllist_push(sllist *l, void *data);
static void _unsafe_sllist_unshift(sllist *l, void *data);
static void *_unsafe_sllist_pop(sllist *l);
static void *_unsafe_sllist_shift(sllist *l);
static void _unsafe_sllist_append_array(sllist *l, size_t n, void **items);
static void _unsafe_sllist_foreach(sllist *l, int (*)(void *, size_t));
static void *_unsafe_sllist_find_first(sllist *l, int (*matcher)(void *));
static void *_unsafe_sllist_discard(sllist *l, int (*matcher)(void *));
static size_t _unsafe_sllist_discard_all(
    sllist *l, int (*matcher)(void *), int (*fn)(void *, size_t));
static void _unsafe_sllist_shuffle(sllist *l);
static void _unsafe_sllist_sort(
    sllist *l, int (*comp)(const void *, const void *));

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
  (void *) &_unsafe_arlist_append_array,
  (void *) &_unsafe_arlist_foreach,
  (void *) &_unsafe_arlist_find_first,
  (void *) &_unsafe_arlist_discard,
  (void *) &_unsafe_arlist_discard_all,
  (void *) &_unsafe_arlist_shuffle,
  (void *) &_unsafe_arlist_sort,
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
  (void *) &_unsafe_deque_append_array,
  (void *) &_unsafe_deque_foreach,
  (void *) &_unsafe_deque_find_first,
  (void *) &_unsafe_deque_discard,
  (void *) &_unsafe_deque_discard_all,
  (void *) &_unsafe_deque_shuffle,
  (void *) &_unsafe_deque_sort,
};

ASSIGN_STATIC_PTR(LISTKIND_DLINK) {
  sizeof (dllist),
  (void *) &dllist_init,
  (void *) &dllist_del,
  (void *) &_unsafe_dllist_get,
  (void *) &_unsafe_dllist_push,
  (void *) &_unsafe_dllist_unshift,
  (void *) &_unsafe_dllist_pop,
  (void *) &_unsafe_dllist_shift,
  (void *) &dllist_concat,
  (void *) &_unsafe_dllist_append_array,
  (void *) &_unsafe_sllist_foreach,  /* not a typo */
  (void *) &_unsafe_sllist_find_first,
  (void *) &_unsafe_dllist_discard,
  (void *) &_unsafe_dllist_discard_all,
  (void *) &_unsafe_dllist_shuffle,
  (void *) &_unsafe_dllist_sort,
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
  (void *) &_unsafe_sllist_append_array,
  (void *) &_unsafe_sllist_foreach,
  (void *) &_unsafe_sllist_find_first,
  (void *) &_unsafe_sllist_discard,
  (void *) &_unsafe_sllist_discard_all,
  (void *) &_unsafe_sllist_shuffle,
  (void *) &_unsafe_sllist_sort,
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

static inline void *_unsafe_arlist_get(arlist *l, ptrdiff_t i) {
  if (i < 0) {
    i += l->size;
  }

  return l->items[i];
}

static inline void *_unsafe_deque_get(deque *l, ptrdiff_t i) {
  if (i < 0) {
    i += l->size;
  }

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

static inline void *_unsafe_sllist_get(sllist *l, ptrdiff_t i) {
  if (i < 0) {
    if (i == -1) {
      return l->last->data;
    } else {
      i += l->size;
    }
  }

  return _unsafe_sllist_get_node(l, i)->data;
}

static inline dllistnode *_unsafe_dllist_get_node_reverse(dllist *l, size_t i) {
  dllistnode *current = l->head->prev;

  while (--i) {
    current = current->prev;
  }

  return current;
}

static inline void *_unsafe_dllist_get(dllist *l, ptrdiff_t i) {
  if (i < 0) {
    i += l->size;
  }

  if ((size_t) i > l->size / 2) {
    return _unsafe_dllist_get_node_reverse(l, l->size - (size_t) i)->data;
  }

  return _unsafe_sllist_get_node((sllist *) l, (size_t) i)->data;
}

static inline enum FLY_STATUS list_bad_call(void * restrict lp, ptrdiff_t i) {
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

FLYAPI void *list_get(list *l, ptrdiff_t i) {
  if (list_bad_call(l, i)) {
    return NULL;
  }
  return l->kind->get(l, i);
}

FLYAPI void *arlist_get(arlist *l, ptrdiff_t i) {
  if (list_bad_call(l, i)) {
    return NULL;
  }
  return _unsafe_arlist_get(l, i);
}

FLYAPI void *deque_get(deque *l, ptrdiff_t i) {
  if (list_bad_call(l, i)) {
    return NULL;
  }
  return _unsafe_deque_get(l, i);
}

FLYAPI void *dllist_get(dllist *l, ptrdiff_t i) {
  if (list_bad_call(l, i)) {
    return NULL;
  }
  return _unsafe_sllist_get((sllist *) l, i);
}

FLYAPI void *sllist_get(sllist *l, ptrdiff_t i) {
  if (list_bad_call(l, i)) {
    return NULL;
  }
  return _unsafe_sllist_get(l, i);
}

// struct member interaction

static inline void *list_end_remove_op_(
    void * restrict vl, void *(*remove)(void *)) {
  list *l = (list *) vl;

  if (l->size > 0) {
    fly_status = FLY_OK;
    return remove(l);
  }

  fly_status = FLY_EMPTY;
  return NULL;
}

#define list_end_remove_op(vl, remove) list_end_remove_op_(vl, (void *) remove)

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

FLYAPI void list_append_array(list *l, size_t n, void **items) {
  if (!(l && items)) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  if (n > PTRINDEX_MAX - l->size) {
    fly_status = FLY_E_TOO_BIG;
    return;
  }

  fly_status = FLY_OK;

  if (!n) {
    return;
  }

  l->kind->append_array(l, n, items);
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

static void *_unsafe_deque_find_first(deque *l, int (*matcher)(void *)) {
  void ** const boundary = l->items + l->capacity;
  void ** const segment = l->end <= l->start ? boundary : l->items + l->end;

  void **items = l->items + l->start;

  do {
    if (matcher(*items)) {
      fly_status = FLY_OK;
      return *items;
    }
  } while (++items < segment);

  if (items == boundary) {
    void ** const end = (items = l->items) + l->end;

    for (; items < end; items++) {
      if (matcher(*items)) {
        fly_status = FLY_OK;
        return *items;
      }
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

static void *_unsafe_arlist_discard(arlist *l, int (*matcher)(void *)) {
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

static inline size_t _deque_inc_start(deque *l) {
  if (l->start == l->capacity - 1) {
    const size_t old_start = l->start;
    l->start = 0;
    return old_start;
  } else {
    return l->start++;
  }
}

static inline size_t _deque_dec_end(deque *l) {
  if (l->end) {
    return l->end--;
  } else {
    l->end = l->capacity - 1;
    return 0;
  }
}

static inline void *_deque_nudge_left(void **item, void **start) {
  void *found = *item;
  memmove(start + 1, start, (item - start) * sizeof (void *));
  return found;
}

static inline void *_deque_nudge_right(void **item, void **end) {
  void *found = *item;
  memmove(item, item + 1, (end - item) * sizeof (void *));
  return found;
}

static void *_unsafe_deque_discard(deque *l, int (*matcher)(void *)) {
  void ** const boundary = l->items + l->capacity;
  void ** const segment = l->end <= l->start ? boundary : l->items + l->end;

  void **item = l->items + l->start;

  if (matcher(*item)) {
    fly_status = FLY_OK;

    l->size--;
    _deque_inc_start(l);
    return *item;
  }

  while (++item < segment) {
    if (matcher(*item)) {
      fly_status = FLY_OK;

      l->size--;

      if ((segment == boundary && l->end)
          || item - (l->items + l->start) < segment - item) {
        return _deque_nudge_left(item, l->items + _deque_inc_start(l));
      } else {
        const size_t prev_end = _deque_dec_end(l);

        if (item == segment - 1 && (segment != boundary || prev_end == 0)) {
          return *item;
        }
        return _deque_nudge_right(item, segment - 1);
      }
    }
  }

  if (item == boundary) {
    void ** const end = (item = l->items) + l->end;

    for (; item < end; item++) {
      if (matcher(*item)) {
        l->size--;
        l->end--;
        fly_status = FLY_OK;
        return _deque_nudge_right(item, end);
      }
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

static void *_unsafe_dllist_discard(dllist *l, int (*matcher)(void *)) {
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

static void *_unsafe_sllist_discard(sllist *l, int (*matcher)(void *)) {
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

FLYAPI void *list_discard(list *l, int (*matcher)(void *)) {
  if (!(l && matcher)) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }
  if (!l->size) {
    fly_status = FLY_NOT_FOUND;
    return NULL;
  }

  return l->kind->discard(l, matcher);
}

static void _unsafe_arlist_foreach(arlist *l, int (*fn)(void *, size_t)) {
  size_t i = 0;

  while (i++ < l->size && !fn(l->items[i - 1], i - 1));
}

static void _unsafe_deque_foreach(deque *l, int (*fn)(void *, size_t)) {
  void ** const boundary = l->items + l->capacity;
  void **segment = l->end <= l->start ? boundary : l->items + l->end;

  size_t i = 0;
  void **item = l->items + l->start;

  do {
    if (fn(*item, i)) {
      return;
    }
  } while (++i, ++item != segment);

  if (segment != boundary || !l->end) {
    return;
  }

  segment = (item = l->items) + l->end;

  for (;;) {
    if (fn(*item, i)) {
      return;
    } else if (++item == segment) {
      return;
    } else {
      i++;
    }
  }
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

  if (!l->size) {
    return;
  }

  l->kind->foreach(l, fn);
}

static size_t _unsafe_arlist_discard_all(
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

static inline void **_deque_move_range(
    void **start, void ***next, void ***end,
    size_t i, size_t left_size, unsigned int wrapmode) {
  size_t delta;

  switch (wrapmode) {    // wrapmode 0: deque does not wrap
    case 1: goto left;   // wrapmode 1: deque wraps; range is before wrap point
    case 2: goto right;  // wrapmode 2: deque wraps; range is after wrap point
  }

  if (i < (size_t) (*end - *next)) {
left:
    // left_size should always be 0 on the left branch
    return memmove(*next - i, start, i * sizeof (void *));
  }
right:
  memmove(start + i - left_size, *next, (*end - *next) * sizeof (void *));
  delta = *next - (start + i - left_size);
  *next -= delta;
  *end -= delta;
  return start;
}

static size_t _unsafe_deque_discard_all(
    deque *l, int (*matcher)(void *), int (*fn)(void *, size_t)) {
  void ** const boundary = l->items + l->capacity;

  unsigned int wrapmode = l->start > l->capacity - l->size;
  void **segment = wrapmode ? boundary : l->items + l->start + l->size;

  void **item;
  void **start = item = l->items + l->start;

  size_t removed;
  size_t i = 0;
  size_t left_size = 0;

  if (matcher(*item)) {
    do {
      if (fn(*item++, i++)) {
        l->start = (item - l->items) % l->capacity;
        l->size -= i;
        return i;
      }

      if (item == segment) {
        if (wrapmode != 1) {
          l->start = l->end = l->size = 0;
          return i;
        }

        left_size = 0;
        segment = (start = item = l->items) + l->end;
        wrapmode = 0;
      }
    } while (matcher(*item));

    start = item;
    removed = i;
  } else {
    removed = 0;
  }

keep_start:
  do {
    size_t i_start;
    void **item_base = item;

    do {
      if (++item == segment) {
        if (wrapmode != 1) {
          goto done;
        }

        i += item - item_base;
        left_size = item - start;

        l->start = start - l->items;
        segment = (start = item = item_base = l->items) + l->end;
        wrapmode = 2;
      }
    } while (!matcher(*item));

    i_start = (i += item - item_base);

discard_start:
    do {
      if (fn(*item++, i++)) {
        removed += i - i_start;
        start = _deque_move_range(
            start, &item, &segment, i - removed, left_size, wrapmode);
        goto done;
      }

      if (item == segment) {
        removed += i - i_start;

        if (wrapmode != 1) {
          segment -= i - i_start;
          goto done_skip;
        }

        // TODO: Unwrap and move kept items to post-wrap free space if available
        left_size =
          item - _deque_move_range(start, &item, &segment, i - removed, 0, 1);

        l->start = l->capacity - left_size;

        segment = (start = item = l->items) + l->end;
        wrapmode = 2;

        if (matcher(*item)) {
          i_start = i;
          goto discard_start;
        } else {
          goto keep_start;
        }
      }
    } while (matcher(*item));

    removed += i - i_start;
    start = _deque_move_range(
        start, &item, &segment, i - removed, left_size, wrapmode);
  } while (item < segment);

done:
  if (wrapmode != 1) {
done_skip:
    l->end = (segment - l->items) % l->capacity;
  }
  if (wrapmode < 2) {
    l->start = start - l->items;
  }
  l->size -= removed;
  return removed;
}

static size_t _unsafe_dllist_discard_all(
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

static size_t _unsafe_sllist_discard_all(
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

FLYAPI size_t list_discard_all(
    list *l, int (*matcher)(void *), int (*fn)(void *, size_t)) {
  if (!(l && matcher)) {
    fly_status = FLY_E_NULL_PTR;
    return 0;
  }
  if (!l->size) {
    fly_status = FLY_OK;
    return 0;
  }

  return l->kind->discard_all(l, matcher, fn ? fn : &do_nothing);
}

FLYAPI void list_shuffle(list *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    l->kind->shuffle(l);
  }
}

static int comp_uintptr(const void *lp, const void *rp) {
  uintptr_t left = *(uintptr_t *) lp;
  uintptr_t right = *(uintptr_t *) rp;

  return (left > right) - (left < right);
}

FLYAPI void list_sort(list *l, int (*comp)(const void *, const void *)) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    l->kind->sort(l, comp ? comp : &comp_uintptr);
  }
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

  if (delta > PTRINDEX_MAX - l->capacity) {
    if (new_elements > PTRINDEX_MAX - l->size) {
      fly_status = FLY_E_TOO_BIG;
      return 0;
    }
    delta = PTRINDEX_MAX;
  } else {
    delta += l->capacity;

    if (delta < ARLIST_DEFAULT_CAPACITY) {
      delta = ARLIST_DEFAULT_CAPACITY;
    }
  }

  if (!(next_elements = realloc(l->items, delta * sizeof (void *)))) {
    // realloc() failed, l->items unchanged
    fly_status = FLY_E_OUT_OF_MEMORY;
    return 0;
  }

  l->items = next_elements;
  return l->capacity = delta;
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
  if (new_elements > l->capacity - l->size) {
    return arlist_grow(l, new_elements);
  }
  return SIZE_MAX;
}

#define ARLIST_HAS_CAPACITY_OR_DIE(l, new_elements, capture)            \
  if (!(capture arlist_ensure_capacity((arlist *) l, new_elements))) {  \
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

    memcpy(
        l1->items + l1->end,
        l2->items + l2->start,
        segment1 * sizeof (void *));

    l1->end = (l1->end + segment1) % l1->capacity;
    remaining -= segment1;
  }

  fly_status = FLY_OK;
}

static void _unsafe_arlist_append_array(arlist *l, size_t n, void **suffix) {
  ARLIST_HAS_CAPACITY_OR_DIE(l, n, )

  memcpy(l->items + l->size, suffix, n * sizeof (void *));
  l->size += n;
}

static void _unsafe_deque_append_array(deque *l, size_t n, void **suffix) {
  size_t right;

  DEQUE_HAS_CAPACITY_OR_DIE(l, n)

  if (l->end < l->start || (right = l->capacity - l->end) > n) {
    memcpy(l->items + l->end, suffix, n * sizeof (void *));
    l->end += n;
  } else {
    memcpy(l->items + l->end, suffix, right * sizeof (void *));
    memcpy(l->items, suffix + right, (l->end = n - right) * sizeof (void *));
  }

  l->size += n;
}

static inline void unsafe_array_shuffle(
    void ** const restrict items, size_t i, rng64 * const restrict rng) {
  void *temp;
  size_t j;

  ASSUME(i > 1);

  while (i > 1) {
    j = rng64_next_in(rng, i--);

    temp     = items[i];
    items[i] = items[j];
    items[j] = temp;
  }
}

static inline void _unsafe_arlist_shuffle(arlist *l) {
  unsafe_array_shuffle(l->items, l->size, &l->rng);
}

FLYAPI void arlist_shuffle(arlist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    _unsafe_arlist_shuffle(l);
  }
}

static inline void unsafe_deque_unwrap(deque *l) {
  if (l->end < l->start && l->end != 0) {
    // Unwrap by cutting the deque by the smaller segment.
    // Doesn't matter that we change the order since we're shuffling anyway.
    if (l->end < l->capacity - l->start) {
      // Move left segment before the right
      memmove(l->items + l->start - l->end, l->items, l->end * sizeof (void *));
      l->start -= l->end;
      l->end = 0;
    } else {
      // Move right segment after the left
      memmove(
          l->items + l->end, l->items + l->start,
          (l->capacity - l->start) * sizeof (void *));
      l->end = l->size;
      l->start = 0;
    }
  }
}

static void _unsafe_deque_shuffle(deque *l) {
  unsafe_deque_unwrap(l);

  l->items += l->start;
  unsafe_array_shuffle(l->items, l->size, &l->rng);
  l->items -= l->start;
}

FLYAPI void deque_shuffle(deque *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    _unsafe_deque_shuffle(l);
  }
}

FLYAPI void **arlist_draw(arlist * restrict l, void ** restrict cursor) {
  void *temp;
  size_t j;

  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }

  if (l->size == 0) {
    fly_status = FLY_EMPTY;
    return NULL;
  }

  fly_status = FLY_OK;

  if (cursor == NULL) {
    cursor = l->items + l->size;
  } else if (cursor <= l->items) {
    return NULL;
  }

  j = rng64_next_in(&l->rng, cursor-- - l->items);

  temp        = *cursor;
  *cursor     = l->items[j];
  l->items[j] = temp;

  return cursor;
}

FLYAPI void **deque_draw(deque * restrict l, void ** restrict cursor) {
  void *temp, **next;
  size_t remaining;
  uint64_t r;

  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }

  if (l->size == 0) {
    fly_status = FLY_EMPTY;
    return NULL;
  }

  fly_status = FLY_OK;

  if (cursor > l->items + l->start) {
    remaining = cursor - (l->items + l->start);
  } else if (cursor == NULL) {
    cursor = l->items + (l->end ? l->end : l->capacity);
    remaining = l->size;
  } else if (cursor == l->items + l->start || cursor < l->items) {
    return NULL;
  } else if (cursor == l->items) {
    remaining = l->capacity - l->start;
  } else {
    remaining = cursor - l->items + l->capacity - l->start;
  }

  r = rng64_next_in(&l->rng, remaining);
  next = l->items + (l->start + r) % l->capacity;

  if (cursor != l->items) {
    --cursor;
  } else {
    cursor = l->items + l->capacity - 1;
  }

  temp    = *cursor;
  *cursor = *next;
  *next   = temp;

  return cursor;
}

FLYAPI void *arlist_pick(arlist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }

  if (l->size == 0) {
    fly_status = FLY_EMPTY;
    return NULL;
  }

  fly_status = FLY_OK;

  return l->items[rng64_next_in(&l->rng, l->size)];
}

FLYAPI void *deque_pick(deque *l) {
  size_t i;

  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return NULL;
  }

  if (l->size == 0) {
    fly_status = FLY_EMPTY;
    return NULL;
  }

  fly_status = FLY_OK;

  i = (l->start + rng64_next_in(&l->rng, l->size)) % l->capacity;

  return l->items[i];
}

static inline void _unsafe_arlist_sort(
    arlist *l, int (*comp)(const void *, const void *)) {
  ASSUME(l != NULL && l->items != NULL && l->size > 1);
  ASSUME(comp != NULL);

  qsort(l->items, l->size, sizeof (void *), comp);
}

static inline void _unsafe_deque_sort(
    deque *l, int (*comp)(const void *, const void *)) {
  ASSUME(l != NULL && l->items != NULL && l->size > 1);
  ASSUME(comp != NULL);

  unsafe_deque_unwrap(l);
  qsort(l->items + l->start, l->size, sizeof (void *), comp);
}

FLYAPI void arlist_sort(arlist *l, int (*comp)(const void *, const void *)) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    _unsafe_arlist_sort(l, comp ? comp : &comp_uintptr);
  }
}

FLYAPI void deque_sort(deque *l, int (*comp)(const void *, const void *)) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    _unsafe_deque_sort(l, comp ? comp : &comp_uintptr);
  }
}

#undef ARLIST_HAS_CAPACITY_OR_DIE
#undef DEQUE_HAS_CAPACITY_OR_DIE

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

static void _unsafe_dllist_append_array(dllist *l, size_t n, void **items) {
  const size_t n_save = n;
  dllistnode *current;
  dllistnode *last = l->head->prev;

  do {
    if ((current = l->alloc(sizeof (dllistnode)))) {
      (last->next = current)->prev = last;
      (last = current)->data = *items++;
    } else {
      goto out_of_memory_unwind;
    }
  } while (--n);

  (last->next = l->head)->prev = last;
  l->size += n_save;
  return;

out_of_memory_unwind:
  while (last != l->head->prev) {
    current = last->prev;
    l->del(last);
    last = current;
  }

  last->next = l->head;
  fly_status = FLY_E_OUT_OF_MEMORY;
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

  l->del(l->last);

  if (l->size == 1) {
    l->size = 0;
    (l->last = l->head)->next = l->head;
    return ret;
  }

  l->last = _unsafe_sllist_get_node(l, l->size - 2);
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

static void _unsafe_sllist_append_array(sllist *l, size_t n, void **items) {
  const size_t n_save = n;
  sllistnode *current = l->last;

  do {
    if ((current->next = l->alloc(sizeof (sllistnode)))) {
      (current = current->next)->data = *items++;
    } else {
      goto out_of_memory_unwind;
    }
  } while (--n);

  (l->last = current)->next = l->head;
  l->size += n_save;
  return;

out_of_memory_unwind:
  current = l->last;

  while (current->next) {
    l->del(current = current->next);
  }

  l->last = l->head;
  fly_status = FLY_E_OUT_OF_MEMORY;
}

static void _unsafe_sllist_shuffle(sllist *l) {
  const size_t size = l->size;

  ASSUME(size > 1);

  sllistnode **dest;
  sllistnode ** const nodes = (sllistnode **) l->alloc(size * sizeof (void *));
  sllistnode ** const end = dest = nodes + size;
  sllistnode *cursor = l->head;

  do {
    *--dest = cursor = cursor->next;
  } while (dest != nodes);

  unsafe_array_shuffle((void **) dest, size, &l->rng);
  cursor = cursor->next;  // advance back to head

  do {
    cursor = cursor->next = *dest;
  } while (++dest != end);

  (l->last = cursor)->next = l->head;

  l->del(nodes);
}

static void _unsafe_dllist_shuffle(dllist *l) {
  const size_t size = l->size;

  ASSUME(size > 1);

  dllistnode **dest;
  dllistnode ** const nodes = (dllistnode **) l->alloc(size * sizeof (void *));
  dllistnode ** end = dest = nodes + size;
  dllistnode *cursor = l->head;

  do {
    *--dest = cursor = cursor->next;
  } while (dest != nodes);

  unsafe_array_shuffle((void **) dest, size, &l->rng);

  ((*dest)->prev = l->head)->next = *dest;
  (l->head->prev = *--end)->next = l->head;

  do {
    ((*dest)->next = *(dest + 1))->prev = *dest;
  } while (++dest != end);

  l->del(nodes);
}

FLYAPI void sllist_shuffle(sllist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    _unsafe_sllist_shuffle(l);
  }
}

FLYAPI void dllist_shuffle(dllist *l) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    _unsafe_dllist_shuffle(l);
  }
}

static void _unsafe_sllist_sort(
    sllist *l, int (*comp)(const void *, const void *)) {
}

static void _unsafe_dllist_sort(
    dllist *l, int (*comp)(const void *, const void *)) {
}

FLYAPI void sllist_sort(sllist *l, int (*comp)(const void *, const void *)) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    _unsafe_sllist_sort(l, comp ? comp : &comp_uintptr);
  }
}

FLYAPI void dllist_sort(dllist *l, int (*comp)(const void *, const void *)) {
  if (!l) {
    fly_status = FLY_E_NULL_PTR;
    return;
  }

  fly_status = FLY_OK;

  if (l->size > 1) {
    _unsafe_dllist_sort(l, comp ? comp : &comp_uintptr);
  }
}

#if defined(__STRICT_ANSI__)
#undef inline
#elif defined(_MSC_VER)
#undef inline
#undef restrict
#endif

