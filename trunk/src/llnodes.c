/** @file llnodes.c
 * This file defines the nodes used in the linked lists, both single and double.
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
#include "llnodes.h"

// allocation

dllistnode *dllistnode_alloc_with(void *(*alloc_callback)(size_t)) {
	dllistnode *ret = (dllistnode *)(*alloc_callback)(sizeof(dllistnode));
	if(!ret) {
		fprintf(stderr, "Out of memory: %s, line %d.\n", __FILE__, __LINE__);
		abort();
	}
	return ret;
}

dllistnode *dllistnode_alloc() {
	return dllistnode_alloc_with(&malloc);
}

sllistnode *sllistnode_alloc_with(void *(*alloc_callback)(size_t)) {
	sllistnode *ret = (sllistnode *)(*alloc_callback)(sizeof(sllistnode));
	if(!ret) {
		fprintf(stderr, "Out of memory: %s, line %d.\n", __FILE__, __LINE__);
		abort();
	}
	return ret;
}

sllistnode *sllistnode_alloc() {
	return sllistnode_alloc_with(&malloc);
}

void dllistnode_destroy_with(dllistnode *node, void (*free_callback)(void *)) {
	(*free_callback)(node);
}

void dllistnode_destroy(dllistnode *node) {
	dllistnode_destroy_with(node, &free);
}

void sllistnode_destroy_with(sllistnode *node, void (*free_callback)(void *)) {
	(*free_callback)(node);
}

void sllistnode_destroy(sllistnode *node) {
	sllistnode_destroy_with(node, &free);
}

// linked list node operations

dllistnode *dllistnode_get_next(dllistnode *curnode) {
	return curnode->next->data == NULL ? curnode->next->next : curnode->next;
}

dllistnode *dllistnode_get_prev(dllistnode *curnode) {
	return curnode->prev->data == NULL ? curnode->prev->prev : curnode->prev;
}

sllistnode *sllistnode_get_next(sllistnode *curnode) {
	return curnode->next->data == NULL ? curnode->next->next : curnode->next;
}

void dllistnode_set_next(dllistnode *curnode, dllistnode *next) {
  curnode->next = next;
}

void dllistnode_set_prev(dllistnode *curnode, dllistnode *prev) {
  curnode->prev = prev;
}

void sllistnode_set_next(sllistnode *curnode, sllistnode *next) {
  curnode->next = next;
}

void *dllistnode_get_data(dllistnode *node) {
  return node->data;
}

void dllistnode_set_data(dllistnode *node, void *data) {
  node->data = data;
}

void *sllistnode_get_data(sllistnode *node) {
  return node->data;
}

void sllistnode_set_data(sllistnode *node, void *data) {
  node->data = data;
}

void sllistnode_head_init(sllistnode *head) {
	head->data = NULL;
	head->next = head;
}

void dllistnode_head_init(dllistnode *head) {
	head->data = NULL;
	head->next = head;
	head->prev = head;
}
