/** @file sllist.c
 * This file defines the singly linked list type provided by the Flytools. Each
 * link is circular in nature and has a single sentinel node. Each link provides
 * a single structural link, next.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009
 * Zachary Murray, all rights reserved. This source file may not be
 * redistributed, in whole or in part. By viewing and/or using this file, you
 * agree not to redistribute the source files without the express written
 * permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com) @date 11/13/2008 19:06:55
 * EST (creation)
 */

#include <stdio.h>
#include "sllist.h"

// allocation

sllist *sllist_alloc_with(void *(*alloc_callback)(size_t)) {
	sllist *ret = (sllist *)(*alloc_callback)(sizeof(sllist));
	if(!ret) {
		fprintf(stderr, "Out of memory: %s, line %d.\n", __FILE__, __LINE__);
		abort();
	}
	return ret;
}

sllist *sllist_alloc() {
	return sllist_alloc_with(&malloc);
}

void sllist_destroy_with(sllist *list, void (*free_callback)(void *)) {
	sllist_empty_callback(list, free_callback);
	(*free_callback)(list);
}

void sllist_destroy(sllist *list) {
	sllist_destroy_with(list, !list->free_callback ? &free : list->free_callback);
}

// initialization

void sllist_init_with(sllist *list, void *(*alloc_callback)(size_t)) {
	list->head = sllistnode_alloc_with(alloc_callback);
	sllistnode_head_init(list->head);
	list->size = 0;
	list->alloc_callback = alloc_callback;
	list->free_callback = NULL;
}

void sllist_init(sllist *list) {
	sllist_init_with(list, &malloc);
}

sllist *sllist_create_with(void *(*alloc_callback)(size_t)) {
	sllist *ret = sllist_alloc_with(alloc_callback);
	sllist_init_with(ret, alloc_callback);
	return ret;
}

sllist *sllist_create() {
	return sllist_create_with(&malloc);
}

void sllist_set_destructor(sllist *list, void (*free_callback)(void *)) {
	list->free_callback = free_callback;
}

// singly linked list functions (on nodes)

void sllist_insert_node_after_head(sllist *list, sllistnode *node) {
	node->next = list->head->next;
	list->head->next = node;
	list->size++;
}

// alias function
void sllist_insert_left_node(sllist *list, sllistnode *node) {
	sllist_insert_node_after_head(list, node);
}

// alias function
void sllist_push_node(sllist *list, sllistnode *node) {
	sllist_insert_node_after_head(list, node);
}

sllistnode *sllist_remove_first_node(sllist *list) {
	sllistnode *ret = list->head->next;
	list->head->next = ret->next;
	list->size--;
	return ret;
}

// alias function
sllistnode *sllist_remove_leftmost_node(sllist *list) {
	return sllist_remove_first_node(list);
}

// alias function
sllistnode *sllist_pop_node(sllist *list) {
	return sllist_remove_first_node(list);
}

// alias function
sllistnode *sllist_dequeue_node(sllist *list) {
	return sllist_remove_first_node(list);
}

sllistnode *sllist_get_prev_node(sllist *list, sllistnode *node) {
	sllistnode *ret = node;
	register int i = 0;
	while(i++ < list->size) {
		ret = ret->next;
	}
	return ret;
}

void *sllist_remove_node(sllist *list, sllistnode *node) {
	void *ret;
	if(list->size > 0) {
		sllistnode *prev = sllist_get_prev_node(list, node);
		prev->next = node->next;
		ret = node->elem;
		sllistnode_destroy_with(node, !list->free_callback ? &free : list->free_callback);
		list->size--;
		return ret;
	}
	return NULL;
}

// singly linked list functions (on elements)

void sllist_insert_elem_after_head(sllist *list, void *elem) {
	sllistnode *node = sllistnode_alloc_with(list->alloc_callback);
	node->elem = elem;
	sllist_insert_node_after_head(list, node);
}

// alias function
void sllist_insert_left(sllist *list, void *elem) {
	sllist_insert_elem_after_head(list, elem);
}

// alias function
void sllist_push(sllist *list, void *elem) {
	sllist_insert_elem_after_head(list, elem);
}

void *sllist_remove_first_elem(sllist *list) {
	sllistnode *node = sllist_remove_first_node(list);
	void *ret = node->elem;
	sllistnode_destroy_with(node, !list->free_callback ? &free : list->free_callback);
	return ret;
}

// alias function
void *sllist_remove_leftmost(sllist *list) {
	return sllist_remove_first_elem(list);
}

// alias function
void *sllist_pop(sllist *list) {
	return sllist_remove_first_elem(list);
}

// alias function
void *sllist_dequeue(sllist *list) {
	return sllist_remove_first_elem(list);
}

// other functions

void sllist_empty(sllist *list) {
	sllist_empty_callback(list, NULL);
}

void sllist_empty_and_free(sllist *list) {
	sllist_empty_callback(list, &free);
}

void sllist_empty_callback(sllist *list, void (*callback)(void *)) {
	while(list->size > 0) {
		if(callback) {
			(*callback)(sllist_remove_first_elem(list));
		} else {
			sllist_remove_first_elem(list);
		}
	}
}

void sllist_iterate_callback(sllist *list, void (*callback)(sllistnode *)) {
	sllistnode *current = list->head;
	while(current->next != list->head) {
		(*callback)(current = current->next);
	}
}
