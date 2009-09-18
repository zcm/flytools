/** @file dllist.h
 * This is the header file for the doubly linked list abstract data type
 * provided by the Flytools. To use this type, simply include this header file
 * in your source code.
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

#ifndef __ZCM_DLLIST_H__
#define __ZCM_DLLIST_H__

#include <stdlib.h>
#include "llnodes.h"

// structure definition
/**
 * A structure that acts as an interface to a doubly linked list.
 */
typedef struct dllist {
    /** The stored allocation routine for allocating new nodes. */
	void *(*alloc_callback)(size_t);
    /**
     * The stored freeing routine for destroying the structure and nodes. If
     * null, the standard free() will be used.
     */
	void (*free_callback)(void *);
	int size; //!< The number of nodes contained within this linked list.
	dllistnode *head; //!< The header or sentinel node for this linked list.
} dllist;

// allocation
/**
 * Allocates a doubly linked list with the specified allocation routine.  Does
 * no initialization.
 * @param alloc_callback the function pointer to the allocation routine for
 * allocating the new list
 * @return a pointer to the newly allocated linked list structure
 */
dllist *dllist_alloc_with(void *(*alloc_callback)(size_t));
/**
 * Allocates a doubly linked list. Does no initialization.
 * @return a pointer to the newly allocated linked list structure
 */
dllist *dllist_alloc();
/**
 * Frees all of the memory associated with this doubly linked list using the
 * specified freeing routine. Any nodes and elements left in the array will be
 * freed with the same freeing routine.
 * @param free_callback the function pointer to the freeing routine for freeing
 * the specified list
 */
void dllist_destroy_with(dllist *list, void (*free_callback)(void *));
/**
 * Frees all of the memory associated with this doubly linked list. Any nodes
 * and elements left in the array are attempted to be freed with a call to
 * free().
 * @param list the doubly linked list to destroy
 */
void dllist_destroy(dllist *list);

// initialization
/**
 * Initializes a previously allocated doubly list that was allocated with the
 * specified allocation callback. This callback is used again to allocate the
 * sentinel node in the list. Additionally, the callback specified here is
 * stored in the list for further use, i.e. when allocating new nodes.
 * @param list the doubly linked list to initialize
 * @param alloc_callback the allocation callback that was used to allocate this
 * list structure
 */
void dllist_init_with(dllist *list, void *(*alloc_callback)(size_t));
/**
 * Initializes a previously allocated doubly linked list. This will allocate the
 * proper header node and set up the links.
 * @param list the doubly linked list to initialize
 */
void dllist_init(dllist *list);
/**
 * Allocates a doubly linked list with the specified allocation routine and
 * then initializes it with a call to dllist_init().
 * @param alloc_callback the allocation routine for which to allocate this
 * doubly linked list
 * @return a pointer to the newly created linked list
 */
dllist *dllist_create_with(void *(*alloc_callback)(size_t));
/**
 * Allocates and initializes a doubly linked list.
 * @return a pointer to the newly created linked list
 */
dllist *dllist_create();
/**
 * Sets the freeing routine for this linked list. Nodes that are freed will be
 * freed with a call to the specified function. This should be set at the time
 * that the linked list is allocated, or at least before any nodes are inserted
 * into the list.
 * @param list the list for which to set the destructor
 * @param free_callback the freeing routine that will be used for this linked
 * list
 */
void dllist_set_destructor(dllist *list, void (*free_callback)(void *));

// doubly linked list functions (on nodes)
/**
 * Inserts the specified node after the header node (which is considered the
 * left of the list).
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void dllist_insert_node_after_head(dllist *list, dllistnode *node);
/**
 * Alias function for dllist_insert_node_after_head().
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void dllist_insert_left_node(dllist *list, dllistnode *node); // alias function
/**
 * Alias function for dllist_insert_node_after_head().
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void dllist_push_node(dllist *list, dllistnode *node); // alias function
/**
 * Inserts the specified node before the header node (which is considered the
 * right of the list).
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void dllist_insert_node_before_head(dllist *list, dllistnode *node);
/**
 * Alias function for dllist_insert_node_before_head().
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void dllist_insert_right_node(dllist *list, dllistnode *node); // alias function
/**
 * Alias function for dllist_insert_node_before_head().
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void dllist_enqueue_node(dllist *list, dllistnode *node); // alias function
/**
 * Removes the first (or leftmost) node from the list and returns it.
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
dllistnode *dllist_remove_first_node(dllist *list);
/**
 * Alias function for dllist_remove_first_node().
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
dllistnode *dllist_remove_leftmost_node(dllist *list); // alias function
/**
 * Alias function for dllist_remove_first_node().
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
dllistnode *dllist_pop_node(dllist *list); // alias function
/**
 * Alias function for dllist_remove_first_node().
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
dllistnode *dllist_dequeue_node(dllist *list); // alias function
/**
 * Removes the last (or rightmost) node from the list and returns it.
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
dllistnode *dllist_remove_last_node(dllist *list);
/**
 * Alias function for dllist_remove_last_node().
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
dllistnode *dllist_remove_rightmost_node(dllist *list); // alias function
/**
 * Removes the given node from the list and returns its data. This function has
 * undefined results if the node is not actually contained in the given list.
 * Unlike the singly-linked list version of this function, this function runs in
 * constant best and worse case times.
 * @param list the list from which to remove the node
 * @param node the node to remove from the list
 * @return the data contained in the node removed (NULL if list is empty)
 */
void *dllist_remove_node(dllist *list, dllistnode *node);

// doubly linked list functions (on elements)
/**
 * Allocates a node to insert into a list at the left and copies the element
 * pointer into the node inserted. This will effectively insert the element into
 * the list.
 * @param list the list in which to insert the element
 * @param elem a pointer to the element that will be inserted into the list
 */
void dllist_insert_item_after_head(dllist *list, void *data);
/**
 * Alias function for dllist_insert_elem_after_head().
 * @param list the list in which to insert the element
 * @param elem a pointer to the element that will be inserted into the list
 */
void dllist_insert_left(dllist *list, void *data); // alias function
/**
 * Alias function for dllist_insert_elem_after_head().
 * @param list the list in which to insert the element
 * @param elem a pointer to the element that will be inserted into the list
 */
void dllist_push(dllist *list, void *data); // alias function
/**
 * Allocates a node to insert into a list at the right and copies the element
 * pointer into the node inserted. This will effectively insert the element into
 * the list.
 * @param list the list in which to insert the element
 * @param elem a pointer to the element that will be inserted into the list
 */
void dllist_insert_item_before_head(dllist *list, void *data);
/**
 * Alias function for dllist_insert_elem_before_head().
 * @param list the list in which to insert the element
 * @param elem a pointer to the element that will be inserted into the list
 */
void dllist_insert_right(dllist *list, void *data); // alias function
/**
 * Alias function for dllist_insert_elem_before_head().
 * @param list the list in which to insert the element
 * @param elem a pointer to the element that will be inserted into the list
 */
void dllist_enqueue(dllist *list, void *data); // alias function
/**
 * Removes the first node from the list and returns the element pointer stored
 * in the node. The node will be freed.
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *dllist_remove_first_item(dllist *list);
/**
 * Alias function for dllist_remove_first_elem().
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *dllist_remove_leftmost(dllist *list); // alias function
/**
 * Alias function for dllist_remove_first_elem().
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *dllist_pop(dllist *list); // alias function
/**
 * Alias function for dllist_remove_first_elem().
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *dllist_dequeue(dllist *list); // alias function
/**
 * Removes the last node from the list and returns the element pointer stored in
 * the node. The node will be freed.
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *dllist_remove_last_item(dllist *list);
/**
 * Alias function for dllist_remove_last_elem().
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *dllist_remove_rightmost(dllist *list); // alias function

// other functions
/**
 * Removes all nodes from the specified list, doing nothing to the respective
 * nodes' element pointers.
 * @param list the list to empty
 */
void dllist_empty(dllist *list);
/**
 * Removes all nodes from the specified list, freeing the elements pointed to by
 * the respective nodes' element pointers.
 * @param list the list to empty
 */
void dllist_empty_and_free(dllist *list);
/**
 * Removes all nodes from the specified list, applying the callback function to
 * every element pointer removed in the list.
 * @param list the list to empty
 * @param callback the callback function to call for every element pointer in
 * the list
 */
void dllist_empty_callback(dllist *list, void (*callback)(void *));
/**
 * Calls the specified callback function for each node in the list.
 * @param list the list through which to iterate
 * @param callback the callback function to call for each node in the list
 */
void dllist_iterate_callback(dllist *list, void (*callback)(dllistnode *));

#endif
