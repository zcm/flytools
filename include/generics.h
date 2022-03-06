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

typedef struct flyobj {
  uint32_t type_id;
  /** The stored allocation routine for allocating new nodes. */
  void *(*allocproc)(size_t);
  /**
   * The stored freeing routine for destroying the structure and nodes. If
   * null, the standard free() will be used.
   */
  void (*freeproc)(void *);
} flyobj;

typedef struct flykind {
  uint32_t id;
} flykind;

FLYAPI void flyobj_del(flyobj *obj);
FLYAPI void flyobj_init(flyobj *obj, void *(*allocproc)(size_t));
FLYAPI void flyobj_set_freeproc(flyobj *obj, void (*proc)(void *));
FLYAPI void *(*flyobj_get_default_allocproc())(size_t);
FLYAPI void flyobj_set_default_allocproc(void *(*proc)(size_t));
FLYAPI void (*flyobj_get_default_freeproc())(void *);
FLYAPI void flyobj_set_default_freeproc(void (*proc)(void *));
FLYAPI void flyobj_set_id(flyobj *obj, uint32_t id);

#endif
