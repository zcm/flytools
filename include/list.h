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

typedef struct list {
  flyobj _parent;
  flykind *kind;
  void *datastore;
} list;

typedef struct listkind {
  flykind _parent;
  void (*init)(list *l);
  void (*destroy)(list *l);
  size_t (*get_size)(list *l);
  void (*set_size)(list *l, size_t size);
  void (*push)(list *l, void *data);
  void (*unshift)(list *l, void *data);
  void *(*pop)(list *l);
  void *(*shift)(list *l);
  void (*concat)(list *l1, list *l2);
} listkind;

extern FLYAPI listkind LISTKIND_DLINK_DEFINITION;
extern FLYAPI listkind LISTKIND_SLINK_DEFINITION;

#define LISTKIND_DLINK &LISTKIND_DLINK_DEFINITION
#define LISTKIND_SLINK &LISTKIND_SLINK_DEFINITION

FLYAPI list *list_new();
FLYAPI list *list_new_kind(listkind *kind);
FLYAPI list *list_new_with(void *(*allocproc)(size_t));
FLYAPI list *list_new_kind_with(listkind *kind, void *(*allocproc)(size_t));
FLYAPI void list_del(list *l);
FLYAPI void list_set_freeproc(list *l, void (*freeproc)(void *));
FLYAPI size_t list_get_size(list *l);
FLYAPI void *list_pop(list *l);
FLYAPI void list_push(list *l, void *data);
FLYAPI void *list_shift(list *l);
FLYAPI void list_unshift(list *l, void *data);

FLYAPI void list_concat(list *l1, list *l2);
FLYAPI void list_concat_into(list *l1, list *l2);

FLYAPI void listkind_dlink_init(list *l);
FLYAPI void listkind_dlink_del(list *l);
FLYAPI size_t listkind_dlink_get_size(list *l);
FLYAPI void listkind_dlink_set_size(list *l, size_t size);
FLYAPI void listkind_dlink_push(list *l, void *data);
FLYAPI void listkind_dlink_unshift(list *l, void *data);
FLYAPI void *listkind_dlink_pop(list *l);
FLYAPI void *listkind_dlink_shift(list *l);
FLYAPI void listkind_dlink_concat(list *l1, list *l2);

FLYAPI void listkind_slink_init(list *l);
FLYAPI void listkind_slink_del(list *l);
FLYAPI size_t listkind_slink_get_size(list *l);
FLYAPI void listkind_slink_set_size(list *l, size_t size);
FLYAPI void listkind_slink_push(list *l, void *data);
FLYAPI void listkind_slink_unshift(list *l, void *data);
FLYAPI void *listkind_slink_pop(list *l);
FLYAPI void *listkind_slink_shift(list *l);
FLYAPI void listkind_slink_concat(list *l1, list *l2);

#if defined(_MSC_VER)
#undef restrict
#endif

#endif
