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

// Yeah, this file does nothing right now. But I might want it in the future.
#if 0
FLYAPI void flyobj_del(struct flyobj *obj) {
  FLY_BAIL_IF_NULL(obj);

  free(obj);
}

FLYAPI void flyobj_init(struct flyobj *obj) {
  FLY_BAIL_IF_NULL(obj);
}
#endif

