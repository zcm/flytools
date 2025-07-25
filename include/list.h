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

FLYAPI void *arlist_get(arlist *l, ptrdiff_t i);
FLYAPI void arlist_push(arlist *l, void *data);
FLYAPI void arlist_unshift(arlist *l, void *data);
FLYAPI void *arlist_pop(arlist *l);
FLYAPI void *arlist_shift(arlist *l);
FLYAPI void arlist_concat(arlist *l1, arlist *l2);
FLYAPI void arlist_shuffle(arlist *l);
FLYAPI void **arlist_draw(arlist * restrict l, void ** restrict cursor);
FLYAPI void *arlist_pick(arlist *l);

FLYAPI void *deque_get(deque *l, ptrdiff_t i);
FLYAPI void deque_push(deque *l, void *data);
FLYAPI void deque_unshift(deque *l, void *data);
FLYAPI void *deque_pop(deque *l);
FLYAPI void *deque_shift(deque *l);
FLYAPI void deque_concat(deque *l1, deque *l2);
FLYAPI void deque_shuffle(deque *l);
FLYAPI void **deque_draw(deque * restrict l, void ** restrict cursor);
FLYAPI void *deque_pick(deque *l);

FLYAPI void *dllist_get(dllist *l, ptrdiff_t i);
FLYAPI void dllist_push(dllist *l, void *data);
FLYAPI void dllist_unshift(dllist *l, void *data);
FLYAPI void *dllist_pop(dllist *l);
FLYAPI void *dllist_shift(dllist *l);
FLYAPI void dllist_concat(dllist *l1, dllist *l2);
FLYAPI void dllist_shuffle(dllist *l);

FLYAPI void *sllist_get(sllist *l, ptrdiff_t i);
FLYAPI void sllist_push(sllist *l, void *data);
FLYAPI void sllist_unshift(sllist *l, void *data);
FLYAPI void *sllist_pop(sllist *l);
FLYAPI void *sllist_shift(sllist *l);
FLYAPI void sllist_concat(sllist *l1, sllist *l2);
FLYAPI void sllist_shuffle(sllist *l);

#if defined(_MSC_VER)
#undef restrict
#endif

#endif
