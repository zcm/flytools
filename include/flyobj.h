/** @file flyobj.h
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

#include "common.h"

#ifndef __FLY_GENERIC__
#define __FLY_GENERIC__

typedef struct flyobj {
    unsigned int type_id;
	void *(*alloc_callback)(size_t);
	void (*free_callback)(void *);
}

#endif
