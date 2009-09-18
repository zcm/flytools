/** @file llist.h
 * This is the header file for the linked list types provided by the Flytools.
 * To use any one of them (or multiple), simply include this header file in your
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

#ifndef __ZCM_LINKED_LIST_H__
#define __ZCM_LINKED_LIST_H__

#include <stdlib.h>
#include "llnodes.h"
#include "dllist.h"
#include "sllist.h"

// type aliases
typedef struct DOUBLE_LINKED_LIST_NODE linked_list_node;
typedef linked_list_node llistnode;
typedef struct DOUBLE_LINKED_LIST linked_list;
typedef linked_list llist;

// linked list allocation
/**
 * Alias function for dllistnode_alloc().
 * @return a newly allocated doubly linked list node
 */
dllistnode *llistnode_alloc();
/**
 * Alias function for dllist_alloc().
 * @return a newly allocated doubly linked list
 */
dllist *llist_alloc();
/**
 * Alias function for dllist_destroy().
 * @param list the list to destroy
 */
void llist_destroy(dllist *list);
/**
 * Alias function for dllistnode_head_init().
 * @param head the node to initialize as a header node
 */
void llistnode_head_init(dllistnode *head);

#endif
