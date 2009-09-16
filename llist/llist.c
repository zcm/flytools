/** @file llist.c
 * This is the main source file for the linked list type provided by the Flytools. This file
 * simply sets the default linked list types used when an llist object is referenced. It
 * currently defaults to the dllist type.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009 Zachary Murray,
 * all rights reserved. This source file may not be redistributed, in whole or in part. By
 * viewing and/or using this file, you agree not to redistribute the source files without the
 * express written permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 */

#include "llist.h"

dllistnode *llistnode_alloc() {
	return dllistnode_alloc();
}

dllist *llist_alloc() {
	return dllist_alloc();
}

void llist_destroy(dllist *list) {
	dllist_destroy(list);
}

void llistnode_head_init(dllistnode *head) {
	dllistnode_head_init(head);
}
