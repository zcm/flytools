/** @file generics.c
 * This is the source code file for the generic type that is the parent of all
 * Flytools structures.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009
 * Zachary Murray, all rights reserved. This source file may not be
 * redistributed, in whole or in part. By viewing and/or using this file, you
 * agree not to redistribute the source files without the express written
 * permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 * @date 11/28/2010 6:01:50 PM Pacific Standard Time (creation)
 */

#include <stdlib.h>
#include "generics.h"

static void *(*_flyobj_default_allocproc)(size_t) = &malloc;
static void (*_flyobj_default_freeproc)(void *) = &free;

FLYAPI void flyobj_del(struct flyobj *obj) {
  if (obj != NULL) {
    obj->del(obj);
  } else {
    fly_status = FLY_E_NULL_PTR;
  }
}

FLYAPI void flyobj_init(
    struct flyobj *obj, void *(*alloc)(size_t), void (*del)(void *)) {
  if (obj != NULL) {
    obj->alloc = alloc;
    obj->del = del;
  } else {
    fly_status = FLY_E_NULL_PTR;
  }
}

FLYAPI void flyobj_set_freeproc(struct flyobj *obj, void (*proc)(void *)) {
  if (obj != NULL) {
    obj->del = proc == NULL ? flyobj_get_default_freeproc() : proc;
  } else {
    fly_status = FLY_E_NULL_PTR;
  }
}

FLYAPI void *(*flyobj_get_default_allocproc())(size_t) {
  return _flyobj_default_allocproc;
}

FLYAPI void flyobj_set_default_allocproc(void *(*proc)(size_t)) {
  _flyobj_default_allocproc = proc;
}

FLYAPI void (*flyobj_get_default_freeproc())(void *) {
  return _flyobj_default_freeproc;
}

FLYAPI void flyobj_set_default_freeproc(void (*proc)(void *)) {
  _flyobj_default_freeproc = proc;
}

