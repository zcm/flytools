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

static uintptr_t dllist_remove_first(dllist *l, int (*matcher)(void *));
static uintptr_t sllist_remove_first(sllist *l, int (*matcher)(void *));
static size_t dllist_remove_all(
    dllist *l, int (*matcher)(void *), int (*fn)(void *, size_t));
static size_t sllist_remove_all(
    sllist *l, int (*matcher)(void *), int (*fn)(void *, size_t));

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
  (void *) &arlist_push,
  (void *) &arlist_unshift,
  (void *) &arlist_pop,
  (void *) &arlist_shift,
  (void *) &arlist_concat,
  NULL, //(void *) &arlist_remove_first,
  NULL, //(void *) &arlist_remove_all,
};

ASSIGN_STATIC_PTR(LISTKIND_DLINK) {
  sizeof (dllist),
  (void *) &dllist_init,
  (void *) &dllist_del,
  (void *) &dllist_push,
  (void *) &dllist_unshift,
  (void *) &dllist_pop,
  (void *) &dllist_shift,
  (void *) &dllist_concat,
  (void *) &dllist_remove_first,
  (void *) &dllist_remove_all,
};

ASSIGN_STATIC_PTR(LISTKIND_SLINK) {
  sizeof (sllist),
  (void *) &sllist_init,
  (void *) &sllist_del,
  (void *) &sllist_push,
  (void *) &sllist_unshift,
  (void *) &sllist_pop,
  (void *) &sllist_shift,
  (void *) &sllist_concat,
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
    FLY_ERR_CLEAR;

    flyobj_init((struct flyobj *) ret, allocproc, freeproc);
    ret->kind = kind;
    kind->init(ret);
  } else {
    FLY_ERR(EFLYNOMEM);
  }
  return ret;
}

FLYAPI void list_del(list *l) {
  if (l != NULL) {
    FLY_ERR_CLEAR;

    l->kind->destroy(l);
    flyobj_del((struct flyobj *) l);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void list_set_freeproc(list *l, void (*freeproc)(void *)) {
  FLY_ERR_CLEAR;
  if (l != NULL) {
    flyobj_set_freeproc((struct flyobj *) l, freeproc);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

// struct member interaction

FLYAPI void *list_pop(list *l) {
  if (l != NULL) {
    if (l->size > 0) {
      FLY_ERR_CLEAR;
      return l->kind->pop(l);
    } else {
      FLY_ERR(EFLYEMPTY);
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }

  return NULL;
}

FLYAPI void list_push(list *l, void *data) {
  if (l != NULL) {
    FLY_ERR_CLEAR;
    l->kind->push(l, data);
  } else {
    FLY_ERR(EFLYBADFN);
  }
}

FLYAPI void *list_shift(list *l) {
  if (l != NULL) {
    if (l->size > 0) {
      FLY_ERR_CLEAR;
      return l->kind->shift(l);
    } else {
      FLY_ERR(EFLYEMPTY);
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }

  return NULL;
}

FLYAPI void list_unshift(list *l, void *data) {
  if (l != NULL) {
    FLY_ERR_CLEAR;
    l->kind->unshift(l, data);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void list_concat(list *l1, list *l2) {
  if (!(l1 && l2)) {
    FLY_ERR(EFLYBADARG);
    return;
  }

  FLY_ERR_CLEAR;

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
    FLY_ERR(EFLYBADARG);
    return;
  }

  if (l1->kind == l2->kind) {
    // silly user, they're the same kind! use list_concat instead (it's faster)
    list_concat(l1, l2);
    return;
  }

  FLY_ERR_CLEAR;

  while (l2->size > 0) {
    list_unshift(l1, list_pop(l2));
  }

  list_del(l2);
}

FLYAPI void *list_find_first(list *l, int (*matcher)(void *)) {
  if (!(l && matcher)) {
    FLY_ERR(EFLYBADARG);
    return NULL;
  }

  if (!l->size) {
    FLY_ERR(EFLYEMPTY);
    return NULL;
  }

  if (l->kind == LISTKIND_DLINK || l->kind == LISTKIND_SLINK) {
    sllistnode *head = ((sllist *) l)->head,
               *current = head->next;

    while (current != head) {
      if (matcher(current->data)) {
        return current->data;
      }
      current = current->next;
    }

    return NULL;
  }

  FLY_ERR(EFLYINTERNAL);
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

static uintptr_t dllist_remove_first(dllist *l, int (*matcher)(void *)) {
  dllistnode *head = l->head,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      void *data = current->data;

      dllist_stitch(current);

      l->del(current);

      return (uintptr_t) data ^ (uintptr_t) 0x1;
    }

    current = current->next;
  }

  return 0x0;
}

static uintptr_t sllist_remove_first(sllist *l, int (*matcher)(void *)) {
  sllistnode *head = l->head,
             *prev = head,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      uintptr_t data = (uintptr_t) current->data;

      sllist_stitch(current, prev, l);

      l->del(current);

      return data ^ 0x1;
    }

    current = (prev = current)->next;
  }

  return 0x0;
}

FLYAPI void *list_remove_first(list *l, int (*matcher)(void *)) {
  uintptr_t data;

  if (!(l && matcher)) {
    FLY_ERR(EFLYBADARG);
    return NULL;
  }

  if (!l->size) {
    return NULL;
  }

  FLY_ERR_CLEAR;

  data = l->kind->remove_first(l, matcher);

  if (data) {
    l->size--;
    return (void *) (data ^ 0x1);
  }

  return NULL;
}

FLYAPI void list_foreach(list *l, int (*fn)(void *, size_t)) {
  size_t i;
  sllistnode *current, *head;

  if (!(l && fn)) {
    FLY_ERR(EFLYBADARG);
    return;
  }

  FLY_ERR_CLEAR;

  i = 0;
  current = head = ((sllist *) l)->head;

  while (head != (current = current->next) && !fn(current->data, i++));
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
    FLY_ERR(EFLYBADARG);
    return 0;
  }

  return l->kind->remove_all(l, matcher, fn ? fn : &do_nothing);
}

#define ARLIST_DEFAULT_CAPACITY 8

FLYAPI void arlist_init(arlist *l) {
  l->size = 0;
  l->capacity = 0;
  l->elements = NULL;
}

FLYAPI void arlist_del(arlist *l) {
  l->del(l->elements);
}

static bool arlist_grow(arlist *l, size_t new_elements) {
  void *next_elements;
  size_t delta = l->capacity >> 1;

  if (delta < new_elements) {
    delta = new_elements;
  }

  l->capacity += delta;

  if (l->capacity < ARLIST_DEFAULT_CAPACITY) {
    l->capacity = ARLIST_DEFAULT_CAPACITY;
  }

  if (!(next_elements = realloc(l->elements, l->capacity * sizeof (void *)))) {
    // realloc() failed, l->elements unchanged
    return 0;
  }

  l->elements = next_elements;
  return 1;
}

#undef ARLIST_DEFAULT_CAPACITY

static inline bool arlist_ensure_capacity(arlist *l, size_t new_elements) {
  if (l->size + new_elements > l->capacity) {
    return arlist_grow(l, new_elements);
  }
  return 1;
}

#define ARLIST_HAS_CAPACITY_OR_DIE(l, new_elements)  \
  if (!arlist_ensure_capacity(l, new_elements)) {    \
    FLY_ERR(EFLYNOMEM);                              \
    return;                                          \
  }

FLYAPI void arlist_push(arlist *l, void *data) {
  ARLIST_HAS_CAPACITY_OR_DIE(l, 1)

  memmove(l->elements + 1, l->elements, l->size * sizeof (void *));

  l->size++;
  l->elements[0] = data;

  FLY_ERR_CLEAR;
}

FLYAPI void arlist_unshift(arlist *l, void *data) {
  ARLIST_HAS_CAPACITY_OR_DIE(l, 1);

  l->elements[l->size++] = data;

  FLY_ERR_CLEAR;
}

FLYAPI void *arlist_pop(arlist *l) {
  void *ret;

  if (!l->size) {
    FLY_ERR(EFLYEMPTY);
    return NULL;
  }

  ret = l->elements[0];

  memmove(l->elements, l->elements + 1, --(l->size) * sizeof (void *));

  FLY_ERR_CLEAR;

  return ret;
}

FLYAPI void *arlist_shift(arlist *l) {
  if (!l->size) {
    FLY_ERR(EFLYEMPTY);
    return NULL;
  }

  FLY_ERR_CLEAR;

  return l->elements[--(l->size)];
}

FLYAPI void arlist_concat(arlist * restrict l1, arlist * restrict l2) {
  ARLIST_HAS_CAPACITY_OR_DIE(l1, l2->size)

  memcpy(l1->elements + l1->size, l2->elements, l2->size * sizeof (void *));

  l1->size += l2->size;

  FLY_ERR_CLEAR;
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
    FLY_ERR(EFLYNOMEM);
  }

  return node;
}

FLYAPI void dllist_init(dllist *l) {
  l->size = 0;

  if ((l->head = l->alloc(sizeof (dllistnode)))) {
    dllistnode_head_init(l->head);
  } else {
    FLY_ERR(EFLYNOMEM);
  }
}

FLYAPI void dllist_del(dllist *l) {
  while (l->size > 0) {
    dllist_pop(l);
  }

  l->del(l->head);
}

FLYAPI void dllist_push(dllist *l, void *data) {
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

FLYAPI void *dllist_pop(dllist *l) {
  void *ret = NULL;
  dllistnode *node = l->head->next;
  node->next->prev = l->head;
  l->head->next = node->next;
  l->size--;
  ret = node->data;
  l->del(node);
  return ret;
}

FLYAPI void *dllist_shift(dllist *l) {
  void *ret = NULL;
  dllistnode *node = l->head->prev;
  node->prev->next = l->head;
  l->head->prev = node->prev;
  l->size--;
  ret = node->data;
  l->del(node);
  return ret;
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

FLYAPI void sllist_init(sllist *l) {
  l->size = 0;

  if ((l->head = l->last = l->alloc(sizeof (dllistnode)))) {
    sllistnode_head_init(l->head);
  } else {
    FLY_ERR(EFLYNOMEM);
  }
}

FLYAPI void sllist_del(sllist *l) {
  while (l->size > 0) {
    sllist_pop(l);
  }

  l->del(l->head);
}

FLYAPI void sllist_push(sllist *l, void *data) {
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

FLYAPI void *sllist_pop(sllist *l) {
  void *ret = NULL;
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
  void *ret = NULL;
  /* This'll unfortunately run in O(n) time, but we have no way to find what the
   * new last pointer will become unless we iterate all the way to the end.
   * TODO: Do not iterate here; use direct access (like, list_get_at or
   * something) or use an implementation of the iterator pattern. -zack
   */
  sllistnode *new_last = NULL;
  sllistnode *current = l->head;
  sllistnode *last = l->last;
  while(current != last) {
    new_last = current;
    current = current->next;
  }
  new_last->next = l->head;
  l->last = new_last;
  l->size--;
  ret = last->data;
  l->del(last);
  return ret;
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

