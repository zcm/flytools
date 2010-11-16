/** @file sllist.h
 * This is the header file for the singly linked list abstract data type
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

#ifndef __ZCM_SLLIST_H__
#define __ZCM_SLLIST_H__

#include <stdlib.h>
#include "common.h"
#include "llnodes.h"

// link structures
/**
 * A structure that acts as an interface to a singly linked list.
 */
typedef struct sllist {
    /** The stored allocation routine for allocating new nodes. */
    void *(*alloc_callback)(size_t);
    /**
     * The stored freeing routine for destroying the structure and nodes. If
     * null, the standard free() will be used.
     */
    void (*free_callback)(void *);
    int size; //!< The number of nodes contained within this linked list.
    sllistnode *head; //!< The header or sentinel node for this linked list.
} sllist;

// allocation
/**
 * Allocates a singly linked list with the specified allocation routine. Does no
 * initialization.
 * @param alloc_callback the function pointer to the allocation routine for
 * allocating the new list
 * @return a pointer to the newly allocated linked list structure
 */
sllist *sllist_alloc_with(void *(*alloc_callback)(size_t));
/**
 * Allocates a singly linked list. Does no initialization.
 * @return a pointer to the newly allocated linked list structure
 */
sllist *sllist_alloc();
/**
 * Frees all of the memory associated with this singly linked list using the
 * specified freeing routine. Any nodes and elements left in the array will be
 * freed with the same freeing routine.
 * @param list the singly linked list to destroy
 * @param free_callback the function pointer to the freeing routine for freeing
 * the specified list
 */
void sllist_destroy_with(sllist *list, void (*free_callback)(void *));
/**
 * Frees all of the memory associated with this singly linked list. Any nodes
 * and elements left in the array are attempted to be freed with a call to
 * free().
 * @param list the singly linked list to destroy
 */
void sllist_destroy(sllist *list);

// initialization
/**
 * Initializes a previously allocated singly list that was allocated with the
 * specified allocation callback. This callback is used again to allocate the
 * sentinel node in the list. Additionally, the callback specified here is
 * stored in the list for further use, i.e. when allocating new nodes.
 * @param list the singly linked list to initialize
 * @param alloc_callback the allocation callback that was used to allocate this
 * list structure
 */
void sllist_init_with(sllist *list, void *(*alloc_callback)(size_t));
/**
 * Initializes a previously allocated singly linked list. This will allocate the
 * proper header node and set up the links.
 * @param list the singly linked list to initialize
 */
void sllist_init(sllist *list);
/**
 * Allocates a singly linked list with the specified allocation routine and then
 * initializes it with a call to sllist_init().
 * @param alloc_callback the allocation routine for which to allocate this
 * singly linked list
 * @return a pointer to the newly created linked list
 */
sllist *sllist_create_with(void *(*alloc_callback)(size_t));
/**
 * Allocates and initializes a singly linked list.
 * @return a pointer to the newly created linked list
 */
sllist *sllist_create();
/**
 * Sets the freeing routine for this linked list. Nodes that are freed will be
 * freed with a call to the specified function. This should be set at the time
 * that the linked list is allocated, or at least before any nodes are inserted
 * into the list.
 * @param list the list for which to set the freeing routine (destructor)
 * @param free_callback the freeing routine that will be used for this linked list
 */
void sllist_set_destructor(sllist *list, void (*free_callback)(void *));

// singly linked list functions (on nodes)
/**
 * Inserts the specified node after the header node (which is considered the
 * left of the list).
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void sllist_insert_node_after_head(sllist *list, sllistnode *node);
/**
 * Alias function for sllist_insert_node_after_head().
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void sllist_insert_left_node(sllist *list, sllistnode *node); // alias function
/**
 * Alias function for sllist_insert_node_after_head().
 * @param list the list into which to insert the node
 * @param node the node to insert
 */
void sllist_push_node(sllist *list, sllistnode *node); // alias function
/**
 * Removes the first (or leftmost) node from the list and returns it.
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
sllistnode *sllist_remove_first_node(sllist *list);
/**
 * Alias function for sllist_remove_first_node().
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
sllistnode *sllist_remove_leftmost_node(sllist *list); // alias function
/**
 * Alias function for sllist_remove_first_node().
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
sllistnode *sllist_pop_node(sllist *list); // alias function
/**
 * Alias function for sllist_remove_first_node().
 * @param list the list from which to remove the node
 * @return the node removed from the list
 */
sllistnode *sllist_dequeue_node(sllist *list); // alias function
/**
 * Follows all of the links in the list to find the previous node of the given
 * node in the list. Note that this function assumes that the given node is
 * actually a part of the given list. This function has undefined results if
 * the node is not actually in the list provided. Also, this function is quite
 * costly, having a best-case (big-omega) running time of O(n). 
 * @param list the list to search
 * @param node the node in list of which to find the previous node
 * @return the previous node of the given node in the specified list
 */
sllistnode *sllist_get_prev_node(sllist *list, sllistnode *node);
/**
 * Removes the given node from the list and returns that node's data. This
 * function assumes that the node is actually contained within the given list
 * and has undefined results if this is not true. Since this function updates
 * links by relying on sllist_get_prev_node(), it has a best-case (big-omega)
 * running time of O(n) and should therefore be avoided when possible.
 * @param list the list from which to remove the given node
 * @param node the node to remove from the list
 * @return the data contained within the specified node (NULL if list is empty)
 */
void *sllist_remove_node(sllist *list, sllistnode *node);

// singly linked list functions (on elements)
/**
 * Allocates a node to insert into a list at the left and copies the element
 * pointer into the node inserted. This will effectively insert the element into
 * the list.
 * @param list the list in which to insert the element
 * @param data a pointer to the element that will be inserted into the list
 */
void sllist_insert_item_after_head(sllist *list, void *data);
/**
 * Alias function for sllist_insert_elem_after_head().
 * @param list the list in which to insert the element
 * @param data a pointer to the element that will be inserted into the list
 */
void sllist_insert_left(sllist *list, void *data); // alias function
/**
 * Alias function for sllist_insert_elem_after_head().
 * @param list the list in which to insert the element
 * @param data a pointer to the element that will be inserted into the list
 */
void sllist_push(sllist *list, void *data); // alias function
/**
 * Removes the first node from the list and returns the element pointer stored
 * in the node. The node will be freed.
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *sllist_remove_first_item(sllist *list);
/**
 * Alias function for sllist_remove_first_elem().
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *sllist_remove_leftmost(sllist *list); // alias function
/**
 * Alias function for sllist_remove_first_elem().
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *sllist_pop(sllist *list); // alias function
/**
 * Alias function for sllist_remove_first_elem().
 * @param list the list from which to remove the element
 * @return a generic pointer to the element removed from the list
 */
void *sllist_dequeue(sllist *list); // alias function

// other functions
/**
 * Removes all nodes from the specified list, doing nothing to the respective
 * nodes' element pointers.
 * @param list the list to empty
 */
void sllist_empty(sllist *list);
/**
 * Removes all nodes from the specified list, freeing the elements pointed
 * to by the respective nodes' element pointers.
 * @param list the list to empty
 */
void sllist_empty_and_free(sllist *list);
/**
 * Removes all nodes from the specified list, applying the callback function
 * to every element pointer removed in the list.
 * @param list the list to empty
 * @param callback the callback function to call for every element pointer in
 * the list
 */
void sllist_empty_callback(sllist *list, void (*callback)(void *));
/**
 * Calls the specified callback function for each node in the list.
 * @param list the list through which to iterate
 * @param callback the callback function to call for each node in the list
 */
void sllist_iterate_callback(sllist *list, void (*callback)(sllistnode *));

#endif
