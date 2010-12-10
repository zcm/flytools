/** @file list.c
 * This file defines the doubly linked list type provided by the Flytools. The
 * structure is completely dynamic; no static allocation is possible. Each list
 * is circular in nature with a single sentinel node, and each link provides a
 * next link and a prev link.
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

#include <stdio.h>
#include "list.h"

FLYAPI listkind LISTKIND_DLINK = {
  { FLYTOOLS_TYPE_LIST | FLYTOOLS_TYPE_KIND },
  &listkind_dlink_init,
  &listkind_dlink_destroy,
  &listkind_dlink_get_size,
  &listkind_dlink_set_size,
  &listkind_dlink_push,
  &listkind_dlink_unshift,
  &listkind_dlink_pop,
  &listkind_dlink_shift,
  &listkind_dlink_concat
};

typedef struct list_dlink_ds {
  size_t size;
  dllistnode *head;
} list_dlink_ds;

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
  if(kind->id & (FLYTOOLS_TYPE_LIST | FLYTOOLS_TYPE_KIND)) {
    ret = (listkind *)kind;
  } else {
    // TODO: not a listkind, handle error
  }
  return ret;
}
#endif

FLYAPI list *list_create() {
  return list_create_kind_with(&LISTKIND_DLINK, flyobj_get_default_allocproc());
}

FLYAPI list *list_create_kind(listkind *kind) {
  return list_create_kind_with(kind, flyobj_get_default_allocproc());
}

FLYAPI list *list_create_with(void *(*allocproc)(size_t)) {
  return list_create_kind_with(&LISTKIND_DLINK, allocproc);
}

FLYAPI list *list_create_kind_with(listkind *kind, void *(*allocproc)(size_t)) {
  list *ret = (list *)(*allocproc)(sizeof(list));
  FLYNITROIFELSE(ret != NULL,
    // then
      flyobj_init((flyobj *)ret, allocproc);
      flyobj_set_id((flyobj *)ret, FLYTOOLS_TYPE_LIST);
      ret->kind = (flykind *)kind;
      kind->init(ret);
      ,
    // else
      /* TODO: couldn't allocate, ret is null. handle error */
      // this was the old list_alloc_with() way to deal with it
      fprintf(stderr, "alloc failed: %s, line %d.\n", __FILE__, __LINE__);
      abort();
  );
  return ret;
}

FLYAPI void list_destroy(list *l) {
  listkind *k = listkind_shadowcast(l->kind);
  FLYNITROIFELSE(k,
    // then
      k->destroy(l);
      ,
    // else
      /* TODO: l->kind was null. handle errors */
  );
  flyobj_destroy((flyobj *)l);
}

// struct member interaction

FLYAPI size_t list_get_size(list *l) {
  size_t ret = 0;
  listkind *k = listkind_shadowcast(l->kind);
  FLYNITROIFELSE(k,
    // then
      ret = k->get_size(l);
      ,
    // else
      /* TODO: l->kind was null. handle errors */
  );
  return ret;
}

FLYAPI void *list_pop(list *l) {
  void *ret = NULL;
  listkind *k = listkind_shadowcast(l->kind);
  FLYNITROIFELSE(k,
    // then
      ret = k->pop(l);
      ,
    // else
      /* TODO: l->kind was null. handle errors */
  );
  return ret;
}

FLYAPI void list_push(list *l, void *data) {
  listkind *k = listkind_shadowcast(l->kind);
  FLYNITROIFELSE(k,
    // then
      k->push(l, data);
      ,
    // else
      /* TODO: l->kind was null. handle errors */
  );
}

FLYAPI void *list_shift(list *l) {
  void *ret = NULL;
  listkind *k = listkind_shadowcast(l->kind);
  FLYNITROIFELSE(k,
    // then
      ret = k->shift(l);
      ,
    // else
      /* TODO: l->kind was null. handle errors */
  );
  return ret;
}

FLYAPI void list_unshift(list *l, void *data) {
  listkind *k = listkind_shadowcast(l->kind);
  FLYNITROIFELSE(k,
    // then
      k->unshift(l, data);
      ,
    // else
      /* TODO: l->kind was null. handle errors */
  );
}

FLYAPI void list_concat(list *l1, list *l2) {
  listkind *k1 = listkind_shadowcast(l1->kind);
  listkind *k2 = listkind_shadowcast(l2->kind);
  FLYNITROIFELSE(k1 && k2,
    // then
      if(k1 == k2) {
        k1->concat(l1, l2);
      } else {
        /* TODO: different kinds. for now, concat into l1... */
        /* but in the future... make a conglomerate list     */
        list_concat_into(l1, l2); // TODO: change to conglomerate list
      }
      ,
    // else
      /* TODO: one of the two list kinds were null... handle errors */
      if(!k1 && k2) {
        /* TODO: only k1 was null. */
      } else if (k1 && !k2) {
        /* TODO: only k2 was null. */
      } else {
        /* TODO: both were null, wtf */
      }
  );
}

FLYAPI void list_concat_into(list *l1, list *l2) {
  if(l1->kind == l2->kind && l1->kind != NULL) {
    // silly user, they're the same kind! use list_concat instead (it's faster)
    list_concat(l1, l2);
  } else {
    while(list_get_size(l2) > 0) {
      list_unshift(l1, list_pop(l2));
    }
    list_destroy(l2);
  }
}

static inline dllistnode *listkind_dlink_get_head(list * restrict l) {
  return ((list_dlink_ds *)l->datastore)->head;
}

static inline void listkind_dlink_set_head(list * restrict l, dllistnode *n) {
  ((list_dlink_ds *)l->datastore)->head = n;
}

void listkind_dlink_iterate_callback(list *l, void (*proc)(dllistnode *)) {
  dllistnode *current = listkind_dlink_get_head(l);
  while(dllistnode_get_next(current) != listkind_dlink_get_head(l)) {
    (*proc)(current = dllistnode_get_next(current));
  }
}

FLYAPI void listkind_dlink_init(list *l) {
  void *(*allocproc)(size_t) = ((flyobj *)l)->allocproc;
  l->datastore = (*allocproc)(sizeof(list_dlink_ds));
  listkind_dlink_set_head(l, dllistnode_alloc_with(allocproc));
  dllistnode_head_init(listkind_dlink_get_head(l));
  listkind_dlink_set_size(l, 0);
}

FLYAPI void listkind_dlink_destroy(list *l) {
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
  dllistnode_destroy_with(node, ((flyobj *)l)->freeproc);
  return ret;
}

FLYAPI void *listkind_dlink_shift(list *l) {
  void *ret = NULL;
  dllistnode *node = dllistnode_get_prev(listkind_dlink_get_head(l));
  dllistnode_set_next(dllistnode_get_prev(node), listkind_dlink_get_head(l));
  dllistnode_set_prev(listkind_dlink_get_head(l), dllistnode_get_prev(node));
  listkind_dlink_set_size(l, listkind_dlink_get_size(l) - 1);
  ret = dllistnode_get_data(node);
  dllistnode_destroy_with(node, ((flyobj *)l)->freeproc);
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
  list_destroy(l2);
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

