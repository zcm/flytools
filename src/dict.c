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

#define DEFAULT_SIZE 16

static void * match_key = NULL;

int dictnode_key_str_matches(
    const struct dictnode * restrict node,
    const void * restrict expected_func) {

  return expected_func == (void *) &dictnode_key_str_matches
    && strcmp((char *) node->key, (char *) match_key) == 0;
}

int dictnode_key_ptr_matches(
    const struct dictnode * restrict node,
    const void * restrict expected_func) {
  return expected_func == (void *) &dictnode_key_ptr_matches
    && node->key == match_key;
}

FLYAPI dictnode *dictnode_alloc_with(void *(*alloc_callback)(size_t)) {
	void *ret = (*alloc_callback)(sizeof(dictnode));
  FLY_ERR_CLEAR;
	if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
	}
	return (dictnode *)ret;
}

FLYAPI dictnode *dictnode_alloc() {
	return dictnode_alloc_with(&malloc);
}

FLYAPI dictnode *dictnode_new(
    void *key, void *data,
    int (*matches)(const struct dictnode *, const void *),
    void *(*alloc_callback)(size_t)) {
  dictnode *ret = dictnode_alloc_with(alloc_callback);
  FLY_ERR_CLEAR;
  if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
  } else {
    ret->key = key;
    ret->data = data;
    ret->matches = matches;
  }
  return ret;
}

FLYAPI void dictnode_del(dictnode *dnode, void (*free_callback)(void *)) {
  FLY_ERR_CLEAR;
  if (dnode != NULL) {
    if (dnode->matches == &dictnode_key_str_matches) {
      free(dnode->key);
    }
    (*free_callback)(dnode);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI dict *dict_alloc_with(void *(*alloc_callback)(size_t)) {
	void *ret = (*alloc_callback)(sizeof(dict));
  FLY_ERR_CLEAR;
	if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
	}
	return (dict *)ret;
}

FLYAPI dict *dict_alloc() {
	return dict_alloc_with(&malloc);
}

static void __dict_init_with(
    dict *d, const size_t size,
    void *(*alloc_callback)(size_t), void (*free_callback)(void *)) {
  if (d != NULL) {
    if (alloc_callback == &malloc) {
      d->buckets = (struct dictbucket *)
        calloc(size, sizeof (struct dictbucket));
    } else {
      d->buckets = (struct dictbucket *)
        memset((*alloc_callback)(size * sizeof(struct dictbucket)), 0, size);
    }

    if(d->buckets == NULL) {
      FLY_ERR(EFLYNOMEM);
    } else {
      d->size = 0;
      d->maxsize = size;
      d->alloc_callback = alloc_callback;
      d->free_callback = free_callback;
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

static inline int __check_power_of_two(const size_t size) {
  if (size <= 1 || (size & size - 1)) {
    FLY_ERR(EFLYBADARG);
    return 0;
  }

  FLY_ERR_CLEAR;
  return 1;
}

FLYAPI void dict_init_with(
    dict *d, const size_t size,
    void *(*alloc_callback)(size_t), void (*free_callback)(void *)) {
  if (__check_power_of_two(size)) {
    __dict_init_with(d, size, alloc_callback, free_callback);
  }
}

FLYAPI void dict_init(dict *d, const size_t size) {
	dict_init_with(d, size, &malloc, &free);
}

FLYAPI dict *dict_new_with(
    const size_t size,
    void *(*alloc_callback)(size_t), void (*free_callback)(void *)) {
	dict *d = NULL;

  if (__check_power_of_two(size)) {
    if ((d = dict_alloc_with(alloc_callback))) {
      __dict_init_with(d, size, alloc_callback, free_callback);
    }
  }

	return d;
}

FLYAPI dict *dict_new_of_size(const size_t size) {
	return dict_new_with(size, &malloc, &free);
}

FLYAPI dict *dict_new() {
  dict *d = dict_alloc_with(&malloc);

  if (d) {
    __dict_init_with(d, DEFAULT_SIZE, &malloc, &free);
  }

  return d;
}

FLYAPI void dict_del(dict *d) /*@-compdestroy@*/ {
  size_t i = 0;

  FLY_ERR_CLEAR;

  if (d != NULL) {
    while(i < d->maxsize) {
      if (d->buckets[i].flags & 0x1) {
        while (list_size(d->buckets[i].data) > 0) {
          dictnode_del(list_pop(d->buckets[i].data), d->free_callback);
        }

        assert(list_size(d->buckets[i].data) == 0);
        list_del(d->buckets[i].data);
      } else if (d->buckets[i].data) {
        dictnode_del(d->buckets[i].data, d->free_callback);
      }

      i++;
    }
    d->free_callback(d->buckets);
    d->free_callback(d);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_set_freeproc(dict *d, void (*free_callback)(void *)) {
  size_t i = 0;

  FLY_ERR_CLEAR;

  if (d != NULL) {
    d->free_callback = free_callback;
    while(i < d->maxsize) {
      if (d->buckets[i].flags & 0x1) {
        list_set_freeproc(d->buckets[i].data, free_callback);
      }
      i++;
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI unsigned int dict_get_hash_index(dict * restrict d, const char *key) {
  FLY_ERR_CLEAR;
  if (d != NULL && key != NULL) {
    return hash_string(key) % d->maxsize;
  } else {
    FLY_ERR(EFLYBADARG);
  }
	return 0;
}

static void __dict_put_to_bucket_atomic(
    dict *d, struct dictbucket *bucket, dictnode *node) {
  if (bucket->data) {
    if (!(bucket->flags & 0x1)) {
      void *existing = bucket->data;

      bucket->data =
        list_new_kind_with(LISTKIND_SLINK, d->alloc_callback, d->free_callback);

      bucket->flags |= 0x1;
      list_push((list *) bucket->data, existing);
    }

    list_push((list *) bucket->data, node);
  } else {
    bucket->data = node;
  }

  d->size++;
}

FLYAPI void dict_put(dict * restrict d, void *key, void *value) {
  FLY_ERR_CLEAR;

  if (d) {
    __dict_put_to_bucket_atomic(
        d, d->buckets + hash_xorshift64s((uint64_t) key) % d->maxsize,
        dictnode_new(
            key, value, &dictnode_key_ptr_matches, d->alloc_callback));
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_puts(dict * restrict d, char *key, void *value) {
  FLY_ERR_CLEAR;

  if (d != NULL && key != NULL) {
    __dict_put_to_bucket_atomic(
        d, d->buckets + hash_string(key) % d->maxsize,
        dictnode_new(
            strdup(key), value, &dictnode_key_str_matches, d->alloc_callback));
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

int __key_ptr_matches(void *node) {
  return ((dictnode *) node)->matches(
      (dictnode *) node, &dictnode_key_ptr_matches);
}

int __key_str_matches(void *node) {
  return ((dictnode *) node)->matches(
      (dictnode *) node, &dictnode_key_str_matches);
}

static inline dictnode *__dict_lookup_using(
    dict * restrict d, void *key,
    dictnode *(*lookup_proc)(dict * restrict, void *)) {
  FLY_ERR_CLEAR;

  if (!d) {
    FLY_ERR(EFLYBADARG);
    return NULL;
  }

  match_key = key;
  dictnode *found = lookup_proc(d, key);
  match_key = NULL;
  return found;
}

static dictnode *__dict_remove_from_bucket(
    struct dictbucket * restrict bucket, void *key, int (*matcher)(void *)) {
  dictnode *node;

  if (bucket->flags & 0x1) {
    if (list_size((list *) bucket->data) > 1) {
      return (dictnode *)
        list_remove_first((list *) bucket->data, matcher);
    }

    node = list_pop((list *) bucket->data);
    list_del((list *) bucket->data);
    bucket->flags = 0x0;
  } else {
    node = (dictnode *) bucket->data;
  }

  bucket->data = 0x0;
  return node;
}

static dictnode *__dict_remove_keyed_ptr(dict * restrict d, void *key) {
  return __dict_remove_from_bucket(
      d->buckets + hash_xorshift64s((uint64_t) key) % d->maxsize,
      key, &__key_ptr_matches);
}

static dictnode *__dict_remove_keyed_str(dict * restrict d, void *key) {
  return __dict_remove_from_bucket(
      d->buckets + dict_get_hash_index(d, key),
      key, &__key_str_matches);
}

#define __dict_remove_using(proc) \
  dictnode *node = __dict_lookup_using(d, key, proc); \
  if (node) { \
    void *data = node->data; \
    d->size--; \
    dictnode_del(node, d->free_callback); \
    return data; \
  } \
  return NULL;

FLYAPI void *dict_remove(dict * restrict d, void *key) {
  __dict_remove_using(&__dict_remove_keyed_ptr);
}

FLYAPI void *dict_removes(dict * restrict d, char *key) {
  __dict_remove_using(&__dict_remove_keyed_str);
}

#undef __dict_remove_using

static inline dictnode *__dict_find_in_bucket(
    struct dictbucket * restrict bucket, void *key, int (*matcher)(void *)) {
  if (bucket->flags & 0x1) {
    return (dictnode *)
      list_find_first((list *) bucket->data, matcher);
  }

  return (dictnode *) bucket->data;
}

static dictnode *__dict_find_keyed_ptr(dict * restrict d, void *key) {
  return __dict_find_in_bucket(
      d->buckets + hash_xorshift64s((uint64_t) key) % d->maxsize,
      key, &__key_ptr_matches);
}

static dictnode *__dict_find_keyed_str(dict * restrict d, void *key) {
  return __dict_find_in_bucket(
      d->buckets + dict_get_hash_index(d, key),
      key, &__key_str_matches);
}

FLYAPI void *dict_get(dict * restrict d, void *key) {
  dictnode *node = __dict_lookup_using(d, key, &__dict_find_keyed_ptr);
  return node ? node->data : NULL;
}

FLYAPI void *dict_gets(dict * restrict d, char *key) {
  dictnode *node = __dict_lookup_using(d, key, &__dict_find_keyed_str);
  return node ? node->data : NULL;
}

/*FLYAPI void dict_iterate_callback(dict *d, void (*callback)(dictnode *)) {
	unsigned int i = 0;
	while(i < d->maxsize) {
		dllistnode *current = d->buckets[i]->head->next;
		while(d->buckets[i]->head != current) {
			(*callback)(current->data);
			current = current->next;
		}
		i++;
	}
}*/

#if __STDC_VERSION__ < 199901L
#undef restrict
#endif
