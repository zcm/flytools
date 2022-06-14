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
#include <threads.h>

#include "dict.h"
#include "internal/dict.h"

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

static thread_local void *match_key = NULL;

static int __ptr_key_matcher(
    const void *key1, const void *key2, const void * restrict expected_func) {
  return key1 == key2 && expected_func == &__ptr_key_matcher;
}

static int __str_key_matcher(
    const void *key1, const void *key2, const void * restrict expected_func) {
  return (key1 == key2 || !strcmp(key1, key2))
    && expected_func == &__str_key_matcher;
}

FLYAPI dictnode *dictnode_alloc_with(void *(*alloc)(size_t)) {
	void *ret = (*alloc)(sizeof(dictnode));
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
    void *key, void *value,
    int (*key_matcher)(const void *, const void *, const void *),
    void *(*alloc)(size_t)) {
  dictnode *ret = dictnode_alloc_with(alloc);
  FLY_ERR_CLEAR;
  if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
  } else {
    ret->key = key;
    ret->value = value;
    ret->key_matcher = key_matcher;
  }
  return ret;
}

FLYAPI void dictnode_del(dictnode *dnode, void (*del)(void *)) {
  FLY_ERR_CLEAR;
  if (dnode != NULL) {
    if (dnode->key_matcher == &__str_key_matcher) {
      free(dnode->key);
    }
    (*del)(dnode);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI dict *dict_alloc_with(void *(*alloc)(size_t)) {
	void *ret = (*alloc)(sizeof(dict));
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
    void *(*alloc)(size_t), void (*del)(void *)) {
  if (d != NULL) {
    if (alloc == &malloc) {
      d->buckets = (struct dictbucket *)
        calloc(size, sizeof (struct dictbucket));
    } else {
      d->buckets = (struct dictbucket *)
        memset((*alloc)(size * sizeof(struct dictbucket)), 0, size);
    }

    if(d->buckets == NULL) {
      FLY_ERR(EFLYNOMEM);
    } else {
      d->size = 0;
      d->capacity = size;
      d->alloc = alloc;
      d->del = del;
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
    void *(*alloc)(size_t), void (*del)(void *)) {
  if (__check_power_of_two(size)) {
    __dict_init_with(d, size, alloc, del);
  }
}

FLYAPI void dict_init(dict *d, const size_t size) {
	dict_init_with(d, size, &malloc, &free);
}

FLYAPI dict *dict_new_with(
    const size_t size,
    void *(*alloc)(size_t), void (*del)(void *)) {
	dict *d = NULL;

  if (__check_power_of_two(size)) {
    if ((d = dict_alloc_with(alloc))) {
      __dict_init_with(d, size, alloc, del);
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
    while(i < d->capacity) {
      if (d->buckets[i].flags & 0x1) {
        while (list_size(d->buckets[i].data) > 0) {
          dictnode_del(list_pop(d->buckets[i].data), d->del);
        }

        assert(list_size(d->buckets[i].data) == 0);
        list_del(d->buckets[i].data);
      } else if (d->buckets[i].data) {
        dictnode_del(d->buckets[i].data, d->del);
      }

      i++;
    }
    d->del(d->buckets);
    d->del(d);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_set_freeproc(dict *d, void (*del)(void *)) {
  size_t i = 0;

  FLY_ERR_CLEAR;

  if (d != NULL) {
    d->del = del;
    while(i < d->capacity) {
      if (d->buckets[i].flags & 0x1) {
        list_set_freeproc(d->buckets[i].data, del);
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
    return hash_string(key) % d->capacity;
  } else {
    FLY_ERR(EFLYBADARG);
  }
	return 0;
}

#define WITH_NEW_NODE_OR_DIE(operation) \
  if (!(node = dictnode_new( \
          key_matcher == &__str_key_matcher ? strdup(key) : key, \
          value, key_matcher, d->alloc))) { \
    return; \
  } \
  operation

static void __dict_set_bucket_atomic(
    dict *d, struct dictbucket *bucket, void *key, void *value,
    int (*key_matcher)(const void *, const void *, const void *)) {
  dictnode *node;  /* Used in macro expansion */

  if (bucket->data) {
    if (!(bucket->flags & 0x1)) {
      dictnode *existing = bucket->data;

      if (existing->key_matcher(existing->key, key, key_matcher)) {
        existing->value = value;
        return;
      }

      list *bucket_list;

      if (!(bucket_list = list_new_kind_with(
              LISTKIND_SLINK, d->alloc, d->del))) {
        return;
      }

      list_push(bucket_list, existing);
      bucket->data = bucket_list;
      bucket->flags |= 0x1;
    }

    WITH_NEW_NODE_OR_DIE(list_push(bucket->data, node));
  } else {
    WITH_NEW_NODE_OR_DIE(bucket->data = node);
  }

  /* Will be skipped if there's an error inside WITH_NEW_NODE_OR_DIE */
  d->size++;
}

#undef WITH_NEW_NODE_OR_DIE

FLYAPI void dict_set(dict * restrict d, void *key, void *value) {
  if (d) {
    FLY_ERR_CLEAR;

    __dict_set_bucket_atomic(
        d, d->buckets + hash_xorshift64s((uint64_t) key) % d->capacity,
        key, value, &__ptr_key_matcher);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_sets(dict * restrict d, char *key, void *value) {
  if (d && key) {
    FLY_ERR_CLEAR;

    __dict_set_bucket_atomic(
        d, d->buckets + hash_string(key) % d->capacity,
        key, value, &__str_key_matcher);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

int __list_ptr_key_matcher(void *node) {
  return ((dictnode *) node)->key_matcher(
      ((dictnode *) node)->key, match_key, &__ptr_key_matcher);
}

int __list_str_key_matcher(void *node) {
  return ((dictnode *) node)->key_matcher(
      ((dictnode *) node)->key, match_key, &__str_key_matcher);
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
      match_key = key;
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
      d->buckets + hash_xorshift64s((uint64_t) key) % d->capacity,
      key, &__list_ptr_key_matcher);
}

static dictnode *__dict_remove_keyed_str(dict * restrict d, void *key) {
  return __dict_remove_from_bucket(
      d->buckets + dict_get_hash_index(d, key),
      key, &__list_str_key_matcher);
}

#define __dict_remove_using(proc) \
  dictnode *node = __dict_lookup_using(d, key, proc); \
  if (node) { \
    void *value = node->value; \
    d->size--; \
    dictnode_del(node, d->del); \
    return value; \
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
    match_key = key;
    return (dictnode *)
      list_find_first((list *) bucket->data, matcher);
  }

  return (dictnode *) bucket->data;
}

static dictnode *__dict_find_keyed_ptr(dict * restrict d, void *key) {
  return __dict_find_in_bucket(
      d->buckets + hash_xorshift64s((uint64_t) key) % d->capacity,
      key, &__list_ptr_key_matcher);
}

static dictnode *__dict_find_keyed_str(dict * restrict d, void *key) {
  return __dict_find_in_bucket(
      d->buckets + dict_get_hash_index(d, key),
      key, &__list_str_key_matcher);
}

FLYAPI void *dict_get(dict * restrict d, void *key) {
  dictnode *node = __dict_lookup_using(d, key, &__dict_find_keyed_ptr);
  return node ? node->value : NULL;
}

FLYAPI void *dict_gets(dict * restrict d, char *key) {
  dictnode *node = __dict_lookup_using(d, key, &__dict_find_keyed_str);
  return node ? node->value : NULL;
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
