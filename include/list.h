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
#include "common.h"
#include "generics.h"
#include "llnodes.h"

#if defined(_MSC_VER)
#define restrict __restrict
#endif

struct listkind;

typedef struct list {
  flyobj _parent;
  struct listkind *kind;
  size_t size;
  void *datastore;
} list;

typedef struct listkind {
  flykind _parent;
  void (*init)(list *l);
  void (*destroy)(list *l);
  void (*push)(list *l, void *data);
  void (*unshift)(list *l, void *data);
  void *(*pop)(list *l);
  void *(*shift)(list *l);
  void (*concat)(list *l1, list *l2);
  uintptr_t (*remove_first)(list *, int (*)(void *));
  size_t (*remove_all)(list *, int (*)(void *), int (*)(void *, size_t));
} listkind;

extern FLYAPI listkind *LISTKIND_DLINK;
extern FLYAPI listkind *LISTKIND_SLINK;

FLYAPI list *list_new();
FLYAPI list *list_new_kind(listkind *kind);
FLYAPI list *list_new_with(
    void *(*allocproc)(size_t), void (*freeproc)(void *));
FLYAPI list *list_new_kind_with(
    listkind *kind, void *(*allocproc)(size_t), void (*freeproc)(void *));
FLYAPI void list_del(list *l);
FLYAPI void list_set_freeproc(list *l, void (*freeproc)(void *));
FLYAPI void *list_pop(list *l);
FLYAPI void list_push(list *l, void *data);
FLYAPI void *list_shift(list *l);
FLYAPI void list_unshift(list *l, void *data);

FLYAPI void list_concat(list *l1, list *l2);
FLYAPI void list_concat_into(list *l1, list *l2);

FLYAPI void *list_find_first(list *l, int (*matcher)(void *));
FLYAPI void *list_remove_first(list *l, int (*matcher)(void *));
FLYAPI void list_foreach(list *l, int (*fn)(void *, size_t));
FLYAPI size_t list_remove_all(
    list *l, int (*matcher)(void *), int (*fn)(void *, size_t));

FLYAPI void dllist_init(list *l);
FLYAPI void dllist_del(list *l);
FLYAPI void dllist_push(list *l, void *data);
FLYAPI void dllist_unshift(list *l, void *data);
FLYAPI void *dllist_pop(list *l);
FLYAPI void *dllist_shift(list *l);
FLYAPI void dllist_concat(list *l1, list *l2);

FLYAPI void sllist_init(list *l);
FLYAPI void sllist_del(list *l);
FLYAPI void sllist_push(list *l, void *data);
FLYAPI void sllist_unshift(list *l, void *data);
FLYAPI void *sllist_pop(list *l);
FLYAPI void *sllist_shift(list *l);
FLYAPI void sllist_concat(list *l1, list *l2);

#if defined(_MSC_VER)
#undef restrict
#endif

#endif
