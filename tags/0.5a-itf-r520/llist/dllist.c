/** @file dllist.c
 * This file defines the doubly linked list type provided by the Flytools. The
 * structure is completely dynamic; no static allocation is possible. Each list
 * is circular in nature with a single sentinel node, and each link provides a
 * next link and a prev link.
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

#include <stdio.h>
#include "dllist.h"

// allocation

dllist *dllist_alloc_with(void *(*alloc_callback)(size_t)) {
	dllist *ret = (dllist *)(*alloc_callback)(sizeof(dllist));
	if(!ret) {
		fprintf(stderr, "Out of memory: %s, line %d.\n", __FILE__, __LINE__);
		abort();
	}
	return ret;
}

dllist *dllist_alloc() {
	return dllist_alloc_with(&malloc);
}

void dllist_destroy_with(dllist *list, void (*free_callback)(void *)) {
	dllist_empty_callback(list, free_callback);
	(*free_callback)(list);
}

void dllist_destroy(dllist *list) {
	dllist_destroy_with(list, !list->free_callback ? &free : list->free_callback);
}

// initialization

void dllist_init_with(dllist *list, void *(*alloc_callback)(size_t)) {
	list->head = dllistnode_alloc_with(alloc_callback);
	dllistnode_head_init(list->head);
	list->size = 0;
	list->alloc_callback = alloc_callback;
	list->free_callback = NULL;
}

void dllist_init(dllist *list) {
	dllist_init_with(list, &malloc);
}

dllist *dllist_create_with(void *(*alloc_callback)(size_t)) {
	dllist *ret = dllist_alloc_with(alloc_callback);
	dllist_init_with(ret, alloc_callback);
	return ret;
}

dllist *dllist_create() {
	return dllist_create_with(&malloc);
}

void dllist_set_destructor(dllist *list, void (*free_callback)(void *)) {
	list->free_callback = free_callback;
}

// doubly linked list functions (on nodes)

void dllist_insert_node_after_head(dllist *list, dllistnode *node) {
	node->next = list->head->next;
	node->prev = list->head;
	list->head->next->prev = node;
	list->head->next = node;
	list->size++;
}

// alias function
void dllist_insert_left_node(dllist *list, dllistnode *node) {
	dllist_insert_node_after_head(list, node);
}

// alias function
void dllist_push_node(dllist *list, dllistnode *node) {
	dllist_insert_node_after_head(list, node);
}

void dllist_insert_node_before_head(dllist *list, dllistnode *node) {
	node->next = list->head;
	node->prev = list->head->prev;
	list->head->prev->next = node;
	list->head->prev = node;
	list->size++;
}

// alias function
void dllist_insert_right_node(dllist *list, dllistnode *node) {
	dllist_insert_node_before_head(list, node);
}

// alias function
void dllist_enqueue_node(dllist *list, dllistnode *node) {
	dllist_insert_node_before_head(list, node);
}

dllistnode *dllist_remove_first_node(dllist *list) {
	dllistnode *ret = list->head->next;
	ret->next->prev = list->head;
	list->head->next = ret->next;
	list->size--;
	return ret;
}

// alias function
dllistnode *dllist_remove_leftmost_node(dllist *list) {
	return dllist_remove_first_node(list);
}

// alias function
dllistnode *dllist_pop_node(dllist *list) {
	return dllist_remove_first_node(list);
}

// alias function
dllistnode *dllist_dequeue_node(dllist *list) {
	return dllist_remove_first_node(list);
}

dllistnode *dllist_remove_last_node(dllist *list) {
	dllistnode *ret = list->head->prev;
	ret->prev->next = list->head;
	list->head->prev = ret->prev;
	list->size--;
	return ret;
}

dllistnode *dllist_remove_rightmost_node(dllist *list) {
	return dllist_remove_last_node(list);
}

void *dllist_remove_node(dllist *list, dllistnode *node) {
	void *ret;
	if(list->size > 0) {
		node->prev->next = node->next;
		node->next->prev = node->prev;
		ret = node->elem;
		dllistnode_destroy_with(node, !list->free_callback ? &free : list->free_callback);
		list->size--;
		return ret;
	}
	return NULL;
}

// doubly linked list functions (on elements)

void dllist_insert_elem_after_head(dllist *list, void *elem) {
	dllistnode *node = dllistnode_alloc_with(list->alloc_callback);
	node->elem = elem;
	dllist_insert_node_after_head(list, node);
}

// alias function
void dllist_insert_left(dllist *list, void *elem) {
	dllist_insert_elem_after_head(list, elem);
}

// alias function
void dllist_push(dllist *list, void *elem) {
	dllist_insert_elem_after_head(list, elem);
}

void dllist_insert_elem_before_head(dllist *list, void *elem) {
	dllistnode *node = dllistnode_alloc_with(list->alloc_callback);
	node->elem = elem;
	dllist_insert_node_before_head(list, node);
}

// alias function
void dllist_insert_right(dllist *list, void *elem) {
	dllist_insert_elem_before_head(list, elem);
}

// alias function
void dllist_enqueue(dllist *list, void *elem) {
	dllist_insert_elem_before_head(list, elem);
}

void *dllist_remove_first_elem(dllist *list) {
	dllistnode *node = dllist_remove_first_node(list);
	void *ret = node->elem;
	dllistnode_destroy_with(node, !list->free_callback ? &free : list->free_callback);
	return ret;
}

// alias function
void *dllist_remove_leftmost(dllist *list) {
	return dllist_remove_first_elem(list);
}

// alias function
void *dllist_pop(dllist *list) {
	return dllist_remove_first_elem(list);
}

// alias function
void *dllist_dequeue(dllist *list) {
	return dllist_remove_first_elem(list);
}

void *dllist_remove_last_elem(dllist *list) {
	dllistnode *node = dllist_remove_last_node(list);
	void *ret = node->elem;
	dllistnode_destroy_with(node, !list->free_callback ? &free : list->free_callback);
	return ret;
}

// alias function
void *dllist_remove_rightmost(dllist *list) {
	return dllist_remove_last_elem(list);
}

// other functions

void dllist_empty(dllist *list) {
	dllist_empty_callback(list, NULL);
}

void dllist_empty_and_free(dllist *list) {
	dllist_empty_callback(list, &free);
}

void dllist_empty_callback(dllist *list, void (*callback)(void *)) {
	while(list->size > 0) {
		if(callback) {
			(*callback)(dllist_remove_first_elem(list));
		} else {
			dllist_remove_first_elem(list);
		}
	}
}

void dllist_iterate_callback(dllist *list, void (*callback)(dllistnode *)) {
	dllistnode *current = list->head;
	while(current->next != list->head) {
		(*callback)(current = current->next);
	}
}
