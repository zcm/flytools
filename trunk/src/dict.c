/** @file dict.c
 * This file contains the dictionary type for the Flytools. The dictionary may
 * contain any pointer type (i.e., a void * pointer), and depends on the list
 * Flytools structure.
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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dict.h"

#if __STDC_VERSION__ < 199901L
#define restrict
#endif

#ifdef __TURBOC__
// this is here to fix a bug in tlib... I have no idea why
// but without it it causes tlib to hang after it puts dict.obj
// into dict.lib
// DO NOT MAKE THIS FUNCTION STATIC! It breaks the whole hack-fix-thing.
// You can probably try to remove this function after you add another
// function, as this appears to be a corner case with the number of functions
// and their respective names.
void *__this_function_does_nothing(void *nothing) {
	return nothing;
}
#endif

FLYAPI dictnode *dictnode_alloc_with(void *(*alloc_callback)(size_t)) {
	void *ret = (*alloc_callback)(sizeof(dictnode));
	if(ret == NULL) {
		fprintf(stderr, "Out of memory: %s, line %d.\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	return (dictnode *)ret;
}

FLYAPI dictnode *dictnode_alloc() {
	return dictnode_alloc_with(&malloc);
}

FLYAPI dictnode *dictnode_create_with(
    const char * restrict key, void *data,
    void *(*alloc_callback)(size_t)) {
	dictnode *ret = dictnode_alloc_with(alloc_callback);
	if(ret == NULL) {
		fprintf(stderr, "alloc failed: in " __FUNCTION__ " with key=\"%s\"", key);
		exit(EXIT_FAILURE);
	}
	ret->key = strdup(key);
	ret->data = data;
	return ret;
}

FLYAPI dictnode *dictnode_create(const char * restrict key, void *data) {
	return dictnode_create_with(key, data, &malloc);
}

FLYAPI void dictnode_destroy_with(
    dictnode *dnode,
    void (*free_callback)(void *)) {
	(*free_callback)(dnode->key);
	(*free_callback)(dnode);
}

FLYAPI void dictnode_destroy(dictnode *dnode) {
	dictnode_destroy_with(dnode, &free);
}

FLYAPI dict *dict_alloc_with(void *(*alloc_callback)(size_t)) {
	void *ret = (*alloc_callback)(sizeof(dict));
	if(ret == NULL) {
		fprintf(stderr, "Out of memory: %s, line %d.\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	return (dict *)ret;
}

FLYAPI dict *dict_alloc() {
	return dict_alloc_with(&malloc);
}

FLYAPI void dict_init_with(
    dict *d, const unsigned int size,
    void *(*alloc_callback)(size_t)) {
	register unsigned int i = 0;
	d->buckets = (list **)(*alloc_callback)(size * sizeof(list *));
	if(d->buckets == NULL) {
		fprintf(stderr, "Out of memory: %s, line %d.\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	d->size = 0;
	d->maxsize = size;
	d->alloc_callback = alloc_callback;
	while(i < size) {
		d->buckets[i] = list_alloc_with(alloc_callback);
		list_init_with(d->buckets[i], alloc_callback);
		i++;
	}
}

FLYAPI void dict_init(dict *d, const unsigned int size) {
	dict_init_with(d, size, &malloc);
}

FLYAPI dict *dict_create_with(
    const unsigned int size,
    void *(*alloc_callback)(size_t)) {
	dict *ret = dict_alloc_with(alloc_callback);
	if(ret == NULL) {
		fprintf(stderr, "alloc failed: in "__FUNCTION__ " with size=%u", size);
		exit(EXIT_FAILURE);
	}
	dict_init_with(ret, size, alloc_callback);
	return ret;
}

FLYAPI dict *dict_create(const unsigned int size) {
	return dict_create_with(size, &malloc);
}

void __callback_dictnode_destroy(/*@only@*/ void *data) {
	dictnode_destroy((dictnode *)data);
}

FLYAPI void dict_destroy_with(
    dict *d,
    void (*free_callback)(void *)) /*@-compdestroy@*/ {
	unsigned int i = 0;
	while(i < d->maxsize) {
		list_empty_callback(d->buckets[i], &__callback_dictnode_destroy);
		assert(d->buckets[i]->size == 0);
		list_destroy(d->buckets[i]);
		i++;
	}
	(*free_callback)(d->buckets);
	(*free_callback)(d);
}

FLYAPI void dict_destroy(dict *d) {
	dict_destroy_with(d, !d->free_callback ? &free : d->free_callback);
}

FLYAPI void dict_set_destructor(dict *d, void (*free_callback)(void *)) {
	unsigned int i = 0;
	d->free_callback = free_callback;
	while(i < d->maxsize) {
		list_set_destructor(d->buckets[i], free_callback);
		i++;
	}
}

FLYAPI unsigned int dict_get_hash_index(
    dict * restrict d,
    const char * restrict key) {
	return compress_hash(hash_string(key), d->maxsize);
}

FLYAPI void dict_insert(
    dict * restrict d,
    const char * restrict key,
    void *value) {
	unsigned int index = dict_get_hash_index(d, key);
	dictnode *newnode = dictnode_create_with(key, value, d->alloc_callback);
	list_push(d->buckets[index], newnode);
	d->size++;
}

FLYAPI void *dict_remove(
    dict * restrict d,
    const char * restrict key) {
	unsigned int index = dict_get_hash_index(d, key);
	dllistnode *current = d->buckets[index]->head->next;
	void *ret;
	while(d->buckets[index]->head != current) {
		if(strcmp(((dictnode *)current->data)->key, key) == 0) {
			dictnode *dnode = ((dictnode *)current->data);
			ret = dnode->data;
			(void)list_remove_node(d->buckets[index], current);
			dictnode_destroy(dnode);
			return ret;
		}
		current = current->next;
	}
	return NULL;
}

FLYAPI void *dict_find(dict *d, const char * restrict key) {
	unsigned int index = dict_get_hash_index(d, key);
	dllistnode *current = d->buckets[index]->head->next;
	void *ret;
	while(d->buckets[index]->head != current) {
		if(strcmp(((dictnode *)current->data)->key, key) == 0) {
			dictnode *dnode = ((dictnode *)current->data);
			ret = dnode->data;
			return ret;
		}
		current = current->next;
	}
	return NULL;
}

FLYAPI void dict_iterate_callback(dict *d, void (*callback)(dictnode *)) {
	unsigned int i = 0;
	while(i < d->maxsize) {
		dllistnode *current = d->buckets[i]->head->next;
		while(d->buckets[i]->head != current) {
			(*callback)(current->data);
			current = current->next;
		}
		i++;
	}
}

#if __STDC_VERSION__ < 199901L
#undef restrict
#endif
