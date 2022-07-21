/** @file llnodes.h
 * This is the header file for the nodes used in the linked list types, both
 * single and double. To use any of the functions contained herein, simply
 * include this header file in your source code.
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

#ifndef __ZCM_LINKED_LIST_NODES_H__
#define __ZCM_LINKED_LIST_NODES_H__

#include <stdlib.h>
#include "common.h"

// node structs

/**
 * A structure that encapsulates a singly linked list node.
 */
typedef struct sllistnode {
	void *data; //!< A generic pointer to the element contained in this node.
	struct sllistnode *next; //!< The next node in the list.
} sllistnode;

/**
 * A structure that encapsulates a doubly linked list node.
 */
typedef struct dllistnode {
	void *data; //!< A generic pointer to the element contained in this node.
	struct dllistnode *next; //!< The next node in the list.
	struct dllistnode *prev; //!< The previous node in the list.
} dllistnode;

// allocation
/**
 * Allocates a dllistnode using the given allocation routine.
 * @param alloc_callback an allocation routine, such as malloc(), that will be
 * used to allocate the memory for the dllistnode
 * @return the newly allocated doubly linked list node
 */
dllistnode *dllistnode_alloc_with(void *(*alloc_callback)(size_t));

/**
 * Allocates a sllistnode using the given allocation routine.
 * @param alloc_callback an allocation routine, such as malloc(), that will be
 * used to allocate the memory for the sllistnode
 * @return the newly allocated doubly linked list node
 */
sllistnode *sllistnode_alloc_with(void *(*alloc_callback)(size_t));

/**
 * Frees the memory associated with this doubly linked list node using the
 * specified freeing routine.
 * @param node the node to destroy
 * @param free_callback a freeing routine, such as free(), that will be used to
 * return the memory for the node to the system
 */
void dllistnode_del_with(dllistnode *node, void (*free_callback)(void *));

/**
 * Frees the memory associated with this singly linked list node using the
 * specified freeing routine.
 * @param node the node to destroy
 * @param free_callback a freeing routine, such as free(), that will be used to
 * return the memory for the node to the system
 */
void sllistnode_del_with(sllistnode *node, void (*free_callback)(void *));

#endif
