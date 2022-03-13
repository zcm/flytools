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

#include "list.h"

FLYAPI listkind *LISTKIND_DLINK = &(listkind) {
  { FLYTOOLS_TYPE_LIST | FLYTOOLS_TYPE_KIND },
  &listkind_dlink_init,
  &listkind_dlink_del,
  &listkind_dlink_get_size,
  &listkind_dlink_set_size,
  &listkind_dlink_push,
  &listkind_dlink_unshift,
  &listkind_dlink_pop,
  &listkind_dlink_shift,
  &listkind_dlink_concat
};

FLYAPI listkind *LISTKIND_SLINK = &(listkind) {
  { FLYTOOLS_TYPE_LIST | FLYTOOLS_TYPE_KIND },
  &listkind_slink_init,
  &listkind_slink_del,
  &listkind_slink_get_size,
  &listkind_slink_set_size,
  &listkind_slink_push,
  &listkind_slink_unshift,
  &listkind_slink_pop,
  &listkind_slink_shift,
  &listkind_slink_concat
};

typedef struct list_dlink_ds {
  size_t size;
  dllistnode *head;
} list_dlink_ds;

typedef struct list_slink_ds {
  size_t size;
  sllistnode *head;
  sllistnode *last;
} list_slink_ds;

#if defined(__STRICT_ANSI__)
#define inline
#elif defined(_MSC_VER)
#define inline __inline
#define restrict __restrict
#endif

#ifdef FLYNITRO
#define listkind_shadowcast(kind) ((listkind *)( kind ))
#else
static inline listkind *listkind_shadowcast(flykind *kind) {
  listkind *ret = NULL;
  FLY_ERR_CLEAR;
  if(kind->id & (FLYTOOLS_TYPE_LIST | FLYTOOLS_TYPE_KIND)) {
    ret = (listkind *)kind;
  } else {
    FLY_ERR(EFLYBADCAST);
  }
  return ret;
}
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
  list *ret = (list *)(*allocproc)(sizeof(list));
  FLY_ERR_CLEAR;
  if(ret != NULL) {
    flyobj_init((flyobj *) ret, allocproc, freeproc);
    flyobj_set_id((flyobj *) ret, FLYTOOLS_TYPE_LIST);
    ret->kind = (flykind *) kind;
    kind->init(ret);
  } else {
    FLY_ERR(EFLYNOMEM);
  }
  return ret;
}

FLYAPI void list_del(list *l) {
  listkind *k;
  FLY_ERR_CLEAR;
  if (l != NULL) {
    k = listkind_shadowcast(l->kind);
    if(k) {
      k->destroy(l);
    } else {
      FLY_ERR(EFLYBADFN);
    }
    flyobj_del((flyobj *)l);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void list_set_freeproc(list *l, void (*freeproc)(void *)) {
  FLY_ERR_CLEAR;
  if (l != NULL) {
    flyobj_set_freeproc((flyobj *)l, freeproc);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

// struct member interaction

FLYAPI size_t list_size(list *l) {
  listkind *k;
  size_t ret = 0;
  FLY_ERR_CLEAR;
  if (l != NULL) {
    k = listkind_shadowcast(l->kind);
    if(k) {
      ret = k->get_size(l);
    } else {
      FLY_ERR(EFLYBADFN);
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
  return ret;
}

FLYAPI void *list_pop(list *l) {
  void *ret = NULL;
  listkind *k;
  FLY_ERR_CLEAR;
  if (l != NULL) {
    k = listkind_shadowcast(l->kind);
    if(k) {
      if(list_size(l) > 0) {
        ret = k->pop(l);
      } else {
        FLY_ERR(EFLYEMPTY);
      }
    } else {
      FLY_ERR(EFLYBADFN);
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
  return ret;
}

FLYAPI void list_push(list *l, void *data) {
  listkind *k;
  FLY_ERR_CLEAR;
  if (l != NULL) {
    k = listkind_shadowcast(l->kind);
    if(k) {
      k->push(l, data);
    } else {
      FLY_ERR(EFLYBADFN);
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void *list_shift(list *l) {
  void *ret = NULL;
  listkind *k;
  FLY_ERR_CLEAR;
  if (l != NULL) {
    k = listkind_shadowcast(l->kind);
    if(k) {
      if(list_size(l) > 0) {
        ret = k->shift(l);
      } else {
        FLY_ERR(EFLYEMPTY);
      }
    } else {
      FLY_ERR(EFLYBADFN);
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
  return ret;
}

FLYAPI void list_unshift(list *l, void *data) {
  listkind *k;
  FLY_ERR_CLEAR;
  if (l != NULL) {
    k = listkind_shadowcast(l->kind);
    if(k) {
      k->unshift(l, data);
    } else {
      FLY_ERR(EFLYBADFN);
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void list_concat(list *l1, list *l2) {
  listkind *k1;
  listkind *k2;
  int ptr_valid = 1;
  FLY_ERR_CLEAR;
  if (l1 != NULL) {
    k1 = listkind_shadowcast(l1->kind);
  } else {
    ptr_valid = 0;
  }
  if (l2 != NULL) {
    k2 = listkind_shadowcast(l2->kind);
  } else {
    ptr_valid = 0;
  }
  if (ptr_valid) {
    if(k1 && k2) {
      if(k1 == k2) {
        k1->concat(l1, l2);
      } else {
        /* TODO: different kinds. for now, concat into l1... */
        /* but in the future... make a conglomerate list     */
        list_concat_into(l1, l2); // TODO: change to conglomerate list
      }
    } else {
      if(!k1 && k2) {
        FLY_ERR(EFLYBADFNONLY1);
      } else if (k1 && !k2) {
        FLY_ERR(EFLYBADFNONLY2);
      } else {
        FLY_ERR(EFLYBADFNBOTH);
      }
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void list_concat_into(list *l1, list *l2) {
  FLY_ERR_CLEAR;
  if (l1 == NULL || l2 == NULL) {
    FLY_ERR(EFLYBADARG);
  } else {
    if(l1->kind == l2->kind && l1->kind != NULL) {
      // silly user, they're the same kind! use list_concat instead (it's faster)
      list_concat(l1, l2);
    } else {
      while(list_size(l2) > 0) {
        list_unshift(l1, list_pop(l2));
      }
      list_del(l2);
    }
  }
}

FLYAPI void *list_find_first(list *l, int (*matcher)(void *)) {
  FLY_ERR_CLEAR;

  if (!(l && matcher)) {
    FLY_ERR(EFLYBADARG);
    return NULL;
  }

  if (!list_size(l)) {
    return NULL;
  }

  if (l->kind == (flykind *) LISTKIND_DLINK
      || l->kind == (flykind *) LISTKIND_SLINK) {
    sllistnode *head = ((list_slink_ds *) l->datastore)->head,
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

static inline uintptr_t __dllist_remove_first(list *l, int (*matcher)(void *)) {
  dllistnode *head = ((list_dlink_ds *) l->datastore)->head,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      void *data = current->data;

      current->prev->next = current->next;
      current->next->prev = current->prev;

      ((flyobj *) l)->freeproc(current);

      return (uintptr_t) data ^ (uintptr_t) 0x1;
    }

    current = current->next;
  }

  return 0x0;
}

static inline uintptr_t __sllist_remove_first(list *l, int (*matcher)(void *)) {
  sllistnode *head = ((list_slink_ds *) l->datastore)->head,
             *prev = head,
             *current = head->next;

  while (current != head) {
    if (matcher(current->data)) {
      void *data = current->data;
      list_slink_ds *ds = (list_slink_ds *) l->datastore;

      if (current == ds->last) {
        ds->last = prev;
      }

      prev->next = current->next;

      ((flyobj *) l)->freeproc(current);

      return (uintptr_t) data ^ (uintptr_t) 0x1;
    }

    current = (prev = current)->next;
  }

  return 0x0;
}

static inline uintptr_t __list_remove_first(list *l, int (*matcher)(void *)) {
  FLY_ERR_CLEAR;

  if (!(l && matcher)) {
    FLY_ERR(EFLYBADARG);
    return 0x0;
  }

  if (!list_size(l)) {
    return 0x0;
  }

  if (l->kind == (flykind *) LISTKIND_DLINK) {
    return __dllist_remove_first(l, matcher);
  } else if (l->kind == (flykind *) LISTKIND_SLINK) {
    return __sllist_remove_first(l, matcher);
  }

  FLY_ERR(EFLYINTERNAL);
  return 0x0;
}

FLYAPI void *list_remove_first(list *l, int (*matcher)(void *)) {
  uintptr_t data = __list_remove_first(l, matcher);

  if (data) {
    --((list_slink_ds *) l->datastore)->size;
    return (void *) (data ^ 0x1);
  }

  return NULL;
}

FLYAPI void list_foreach(list *l, int (*fn)(void *, size_t)) {
  FLY_ERR_CLEAR;

  if (!(l && fn)) {
    FLY_ERR(EFLYBADARG);
    return;
  }

  size_t i = 0;
  sllistnode *current, *head;
  current = head = ((list_slink_ds *) l->datastore)->head;

  while (head != (current = current->next) && !fn(current->data, i++));
}

static inline dllistnode *listkind_dlink_get_head(list * restrict l) {
  return ((list_dlink_ds *)l->datastore)->head;
}

static inline void listkind_dlink_set_head(list * restrict l, dllistnode *n) {
  ((list_dlink_ds *)l->datastore)->head = n;
}

FLYAPI void listkind_dlink_init(list *l) {
  void *(*allocproc)(size_t) = ((flyobj *)l)->allocproc;
  l->datastore = (*allocproc)(sizeof(list_dlink_ds));
  listkind_dlink_set_head(l, dllistnode_alloc_with(allocproc));
  dllistnode_head_init(listkind_dlink_get_head(l));
  listkind_dlink_set_size(l, 0);
}

FLYAPI void listkind_dlink_del(list *l) {
  while(listkind_dlink_get_size(l) > 0) {
    listkind_dlink_pop(l);
  }
  ((flyobj *)l)->freeproc(listkind_dlink_get_head(l));
  ((flyobj *)l)->freeproc(l->datastore);
}

FLYAPI size_t listkind_dlink_get_size(list *l) {
  return ((list_dlink_ds *)l->datastore)->size;
}

FLYAPI void listkind_dlink_set_size(list *l, size_t size) {
  ((list_dlink_ds *)l->datastore)->size = size;
}

FLYAPI void listkind_dlink_push(list *l, void *data) {
  dllistnode *node = dllistnode_alloc_with(((flyobj *)l)->allocproc);
  dllistnode_set_data(node, data);
  dllistnode_set_next(node, dllistnode_get_next(listkind_dlink_get_head(l)));
  dllistnode_set_prev(node, listkind_dlink_get_head(l));
  dllistnode_set_prev(dllistnode_get_next(listkind_dlink_get_head(l)), node);
  dllistnode_set_next(listkind_dlink_get_head(l), node);
  listkind_dlink_set_size(l, listkind_dlink_get_size(l) + 1);
}

FLYAPI void listkind_dlink_unshift(list *l, void *data) {
  dllistnode *node = dllistnode_alloc_with(((flyobj *)l)->allocproc);
  dllistnode_set_data(node, data);
  dllistnode_set_next(node, listkind_dlink_get_head(l));
  dllistnode_set_prev(node, dllistnode_get_prev(listkind_dlink_get_head(l)));
  dllistnode_set_next(dllistnode_get_prev(listkind_dlink_get_head(l)), node);
  dllistnode_set_prev(listkind_dlink_get_head(l), node);
  listkind_dlink_set_size(l, listkind_dlink_get_size(l) + 1);
}

FLYAPI void *listkind_dlink_pop(list *l) {
  void *ret = NULL;
  dllistnode *node = dllistnode_get_next(listkind_dlink_get_head(l));
  dllistnode_set_prev(dllistnode_get_next(node), listkind_dlink_get_head(l));
  dllistnode_set_next(listkind_dlink_get_head(l), dllistnode_get_next(node));
  listkind_dlink_set_size(l, listkind_dlink_get_size(l) - 1);
  ret = dllistnode_get_data(node);
  dllistnode_del_with(node, ((flyobj *)l)->freeproc);
  return ret;
}

FLYAPI void *listkind_dlink_shift(list *l) {
  void *ret = NULL;
  dllistnode *node = dllistnode_get_prev(listkind_dlink_get_head(l));
  dllistnode_set_next(dllistnode_get_prev(node), listkind_dlink_get_head(l));
  dllistnode_set_prev(listkind_dlink_get_head(l), dllistnode_get_prev(node));
  listkind_dlink_set_size(l, listkind_dlink_get_size(l) - 1);
  ret = dllistnode_get_data(node);
  dllistnode_del_with(node, ((flyobj *)l)->freeproc);
  return ret;
}

FLYAPI void listkind_dlink_concat(list *l1, list *l2) {
  size_t newsize = listkind_dlink_get_size(l1) + listkind_dlink_get_size(l2);
  dllistnode_set_next(dllistnode_get_prev(listkind_dlink_get_head(l1)),
                      dllistnode_get_next(listkind_dlink_get_head(l2)));
  dllistnode_set_prev(dllistnode_get_next(listkind_dlink_get_head(l2)),
                      dllistnode_get_prev(listkind_dlink_get_head(l1)));
  dllistnode_set_next(dllistnode_get_prev(listkind_dlink_get_head(l2)),
                      listkind_dlink_get_head(l1));
  dllistnode_set_prev(listkind_dlink_get_head(l1),
                      dllistnode_get_prev(listkind_dlink_get_head(l2)));
  listkind_dlink_set_size(l1, newsize);
  // set the size to 0 so that destroy avoids destroying the inner nodes
  // (because they are now part of the first list's structure)
  listkind_dlink_set_size(l2, 0);
  list_del(l2);
}

static inline sllistnode *listkind_slink_get_head(list * restrict l) {
  return ((list_slink_ds *)l->datastore)->head;
}

static inline sllistnode *listkind_slink_get_last(list * restrict l) {
  return ((list_slink_ds *)l->datastore)->last;
}

static inline void listkind_slink_set_head(list * restrict l, sllistnode *n) {
  ((list_slink_ds *)l->datastore)->head = n;
}

static inline void listkind_slink_set_last(list * restrict l, sllistnode *n) {
  ((list_slink_ds *)l->datastore)->last = n;
}

FLYAPI void listkind_slink_init(list *l) {
  void *(*allocproc)(size_t) = ((flyobj *)l)->allocproc;
  l->datastore = (*allocproc)(sizeof(list_slink_ds));
  listkind_slink_set_head(l, sllistnode_alloc_with(allocproc));
  listkind_slink_set_last(l, listkind_slink_get_head(l));
  sllistnode_head_init(listkind_slink_get_head(l));
  listkind_slink_set_size(l, 0);
}

FLYAPI void listkind_slink_del(list *l) {
  while(listkind_slink_get_size(l) > 0) {
    listkind_slink_pop(l);
  }
  ((flyobj *)l)->freeproc(listkind_slink_get_head(l));
  ((flyobj *)l)->freeproc(l->datastore);
}

FLYAPI size_t listkind_slink_get_size(list *l) {
  return ((list_slink_ds *)l->datastore)->size;
}

FLYAPI void listkind_slink_set_size(list *l, size_t size) {
  ((list_slink_ds *)l->datastore)->size = size;
}

FLYAPI void listkind_slink_push(list *l, void *data) {
  sllistnode *node = sllistnode_alloc_with(((flyobj *)l)->allocproc);
  sllistnode_set_data(node, data);
  sllistnode_set_next(node, sllistnode_get_next(listkind_slink_get_head(l)));
  sllistnode_set_next(listkind_slink_get_head(l), node);
  if(listkind_slink_get_size(l) == 0) {
    listkind_slink_set_last(l, node);
  }
  listkind_slink_set_size(l, listkind_slink_get_size(l) + 1);
}

FLYAPI void listkind_slink_unshift(list *l, void *data) {
  sllistnode *node = sllistnode_alloc_with(((flyobj *)l)->allocproc);
  sllistnode_set_data(node, data);
  sllistnode_set_next(node, listkind_slink_get_head(l));
  sllistnode_set_next(listkind_slink_get_last(l), node);
  listkind_slink_set_last(l, node);
  listkind_slink_set_size(l, listkind_slink_get_size(l) + 1);
}

FLYAPI void *listkind_slink_pop(list *l) {
  void *ret = NULL;
  sllistnode *node = sllistnode_get_next(listkind_slink_get_head(l));
  sllistnode_set_next(listkind_slink_get_head(l), sllistnode_get_next(node));
  listkind_slink_set_size(l, listkind_slink_get_size(l) - 1);
  if(listkind_slink_get_size(l) == 0) {
    listkind_slink_set_last(l, listkind_slink_get_head(l));
  }
  ret = sllistnode_get_data(node);
  sllistnode_del_with(node, ((flyobj *)l)->freeproc);
  return ret;
}

FLYAPI void *listkind_slink_shift(list *l) {
  void *ret = NULL;
  /* This'll unfortunately run in O(n) time, but we have no way to find what the
   * new last pointer will become unless we iterate all the way to the end.
   * TODO: Do not iterate here; use direct access (like, list_get_at or
   * something) or use an implementation of the iterator pattern. -zack
   */
  sllistnode *new_last = NULL;
  sllistnode *current = listkind_slink_get_head(l);
  sllistnode *last = listkind_slink_get_last(l);
  while(current != last) {
    new_last = current;
    current = sllistnode_get_next(current);
  }
  sllistnode_set_next(new_last, listkind_slink_get_head(l));
  listkind_slink_set_last(l, new_last);
  listkind_slink_set_size(l, listkind_slink_get_size(l) - 1);
  ret = sllistnode_get_data(last);
  sllistnode_del_with(last, ((flyobj *)l)->freeproc);
  return ret;
}

FLYAPI void listkind_slink_concat(list *l1, list *l2) {
  size_t newsize = listkind_slink_get_size(l1) + listkind_slink_get_size(l2);
  sllistnode_set_next(listkind_slink_get_last(l1),
                      sllistnode_get_next(listkind_slink_get_head(l2)));
  listkind_slink_set_last(l1, listkind_slink_get_last(l2));
  sllistnode_set_next(listkind_slink_get_last(l1), listkind_slink_get_head(l1));
  listkind_slink_set_size(l1, newsize);
  // set the size to 0 so that destroy avoids destroying the inner nodes
  // (because they are now part of the first list's structure)
  listkind_slink_set_size(l2, 0);
  list_del(l2);
}

#ifdef listkind_shadowcast
#undef listkind_shadowcast
#endif

#if defined(__STRICT_ANSI__)
#undef inline
#elif defined(_MSC_VER)
#undef inline
#undef restrict
#endif

