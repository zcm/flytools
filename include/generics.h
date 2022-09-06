/** @file generics.h
 * This is the header file for the generic type contained in the Flytools.
 * Simply include this file with in your source code to access the Flytools
 * generic data type.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009
 * Zachary Murray, all rights reserved. This source file may not be
 * redistributed, in whole or in part. By viewing and/or using this file, you
 * agree not to redistribute the source files without the express written
 * permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 * @date 09/19/2009 15:58:12 EDT (creation)
 */

#include <stdint.h>
#include "common.h"

#ifndef __FLYOBJ_H__
#define __FLYOBJ_H__

#define STRUCTIFY(def)  \
  struct {              \
    def                 \
  }

#define UNIFY_PARENT(parent, def)  \
  union {                          \
    parent;                        \
    def                            \
  }

#ifdef __TURBOC__
#define INHERIT_STRUCT_DEF(def) def
#define UNIFY_OBJECT_DEF(parent, def) def
#else
#define INHERIT_STRUCT_DEF(def) STRUCTIFY(def);
#define UNIFY_OBJECT_DEF(parent, def) UNIFY_PARENT(parent, STRUCTIFY(def););
#endif

#define FLYOBJ_DEFINITION \
  void *(*alloc)(size_t); \
  void (*del)(void *);

struct flyobj {
  INHERIT_STRUCT_DEF(FLYOBJ_DEFINITION)
};

#define FLYOBJ_SUPER               \
  UNIFY_OBJECT_DEF(struct flyobj _obj, FLYOBJ_DEFINITION)

FLYAPI void flyobj_del(struct flyobj *obj);
FLYAPI void flyobj_init(
    struct flyobj *obj, void *(*allocproc)(size_t), void (*freeproc)(void *));
FLYAPI void flyobj_set_freeproc(struct flyobj *obj, void (*proc)(void *));
FLYAPI void *(*flyobj_get_default_allocproc())(size_t);
FLYAPI void flyobj_set_default_allocproc(void *(*proc)(size_t));
FLYAPI void (*flyobj_get_default_freeproc())(void *);
FLYAPI void flyobj_set_default_freeproc(void (*proc)(void *));

#endif
