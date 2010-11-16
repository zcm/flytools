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

FLYAPI dllist *dllist_alloc_with(void *(*alloc_callback)(size_t)) {
  dllist *ret = (dllist *)(*alloc_callback)(sizeof(dllist));
  if(!ret) {
    fprintf(stderr, "alloc failed: %s, line %d.\n", __FILE__, __LINE__);
    abort();
  }
  return ret;
}

FLYAPI dllist *dllist_alloc() {
  return dllist_alloc_with(&malloc);
}

FLYAPI void dllist_destroy_with(dllist *list, void (*free_callback)(void *)) {
  dllist_empty_callback(list, free_callback);
  (*free_callback)(list);
}

FLYAPI void dllist_destroy(dllist *list) {
  dllist_destroy_with(list, !list->free_callback ? &free : list->free_callback);
}

// struct member interaction

FLYAPI dllistnode *dllist_get_head(dllist *list) {
  return list->head;
}

FLYAPI void dllist_set_head(dllist *list, dllistnode *head) {
  list->head = head;
}

FLYAPI int dllist_get_size(dllist *list) {
  return list->size;
}

//private
void dllist_set_size(dllist *list, int size) {
  list->size = size;
}

//private
void dllist_inc_size(dllist *list) {
  dllist_set_size(list, dllist_get_size(list) + 1);
}

//private
void dllist_dec_size(dllist *list) {
  dllist_set_size(list, dllist_get_size(list) - 1);
}

FLYAPI void dllist_set_destructor(dllist *list, void (*free_callback)(void *)) {
  list->free_callback = free_callback;
}

// initialization

FLYAPI void dllist_init_with(dllist *list, void *(*alloc_callback)(size_t)) {
  dllist_set_head(list, dllistnode_alloc_with(alloc_callback));
  dllistnode_head_init(list->head);
  dllist_set_size(list, 0);
  list->alloc_callback = alloc_callback;
  dllist_set_destructor(list, NULL);
}

FLYAPI void dllist_init(dllist *list) {
  dllist_init_with(list, &malloc);
}

FLYAPI dllist *dllist_create_with(void *(*alloc_callback)(size_t)) {
  dllist *ret = dllist_alloc_with(alloc_callback);
  dllist_init_with(ret, alloc_callback);
  return ret;
}

FLYAPI dllist *dllist_create() {
  return dllist_create_with(&malloc);
}

// doubly linked list functions (on nodes)

FLYAPI void dllist_push_node(dllist *list, dllistnode *node) {
  dllistnode_set_next(node, dllistnode_get_next(dllist_get_head(list)));
  dllistnode_set_prev(node, dllist_get_head(list));
  dllistnode_set_prev(dllistnode_get_next(dllist_get_head(list)), node);
  dllistnode_set_next(dllist_get_head(list), node);
  dllist_inc_size(list);
}

FLYAPI void dllist_shift_node(dllist *list, dllistnode *node) {
  dllistnode_set_next(node, dllist_get_head(list));
  dllistnode_set_prev(node, dllistnode_get_prev(dllist_get_head(list)));
  dllistnode_set_next(dllistnode_get_prev(dllist_get_head(list)), node);
  dllistnode_set_prev(dllist_get_head(list), node);
  dllist_inc_size(list);
}

FLYAPI dllistnode *dllist_pop_node(dllist *list) {
  dllistnode *ret = dllistnode_get_next(dllist_get_head(list));
  dllistnode_set_prev(dllistnode_get_next(ret), dllist_get_head(list));
  dllistnode_set_next(dllist_get_head(list), dllistnode_get_next(ret));
  dllist_dec_size(list);
  return ret;
}

FLYAPI dllistnode *dllist_unshift_node(dllist *list) {
  dllistnode *ret = dllistnode_get_prev(dllist_get_head(list));
  dllistnode_set_next(dllistnode_get_prev(ret), dllist_get_head(list));
  dllistnode_set_prev(dllist_get_head(list), dllistnode_get_prev(ret));
  dllist_dec_size(list);
  return ret;
}

FLYAPI void *dllist_remove_node(dllist *list, dllistnode *node) {
  void *ret = NULL;
  if(dllist_get_size(list) > 0) {
    dllistnode_set_next(dllistnode_get_prev(node), dllistnode_get_next(node));
    dllistnode_set_prev(dllistnode_get_next(node), dllistnode_get_prev(node));
    ret = dllistnode_get_data(node);
    dllistnode_destroy_with(node, !list->free_callback ? &free : list->free_callback);
    dllist_dec_size(list);
  }
  return ret;
}

// doubly linked list functions (on elements)

FLYAPI void dllist_push(dllist *list, void *data) {
  dllistnode *node = dllistnode_alloc_with(list->alloc_callback);
  dllistnode_set_data(node, data);
  dllist_push_node(list, node);
}

FLYAPI void dllist_shift(dllist *list, void *data) {
  dllistnode *node = dllistnode_alloc_with(list->alloc_callback);
  dllistnode_set_data(node, data);
  dllist_shift_node(list, node);
}

FLYAPI void *dllist_pop(dllist *list) {
  dllistnode *node = dllist_pop_node(list);
  void *ret = dllistnode_get_data(node);
  dllistnode_destroy_with(node, !list->free_callback ? &free : list->free_callback);
  return ret;
}

FLYAPI void *dllist_unshift(dllist *list) {
  dllistnode *node = dllist_unshift_node(list);
  void *ret = dllistnode_get_data(node);
  dllistnode_destroy_with(node, !list->free_callback ? &free : list->free_callback);
  return ret;
}

// other functions

FLYAPI void dllist_empty(dllist *list) {
  dllist_empty_callback(list, NULL);
}

FLYAPI void dllist_empty_and_free(dllist *list) {
  dllist_empty_callback(list, &free);
}

FLYAPI void dllist_empty_callback(dllist *list, void (*proc)(void *)) {
  while(dllist_get_size(list) > 0) {
    if(proc) {
      (*proc)(dllist_pop(list));
    } else {
      dllist_pop(list);
    }
  }
}

FLYAPI void dllist_iterate_callback(dllist *list, void (*proc)(dllistnode *)) {
  dllistnode *current = dllist_get_head(list);
  while(dllistnode_get_next(current) != dllist_get_head(list)) {
    (*proc)(current = dllistnode_get_next(current));
  }
}
