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
#include <math.h>
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
#define LOAD_FACTOR 75

static thread_local void *_match_key = NULL;
static thread_local size_t _curr_bitmask = 0;
static thread_local size_t _next_bitmask = 0;
static thread_local dict *_curr_dict = NULL;
static thread_local void *_recycle_bin = NULL;

static thread_local int
(*_match_key_matcher)(const void *, const void *, const void *) = NULL;

static int _ptr_key_matcher(
    const void *key1, const void *key2, const void * restrict expected_func) {
  return key1 == key2 && expected_func == &_ptr_key_matcher;
}

static int _str_key_matcher(
    const void *key1, const void *key2, const void * restrict expected_func) {
  return (key1 == key2 || !strcmp(key1, key2))
    && expected_func == &_str_key_matcher;
}

static int _list_ptr_key_matcher(void *node) {
  return ((dictnode *) node)->key_matcher(
      ((dictnode *) node)->key, _match_key, &_ptr_key_matcher);
}

static int _list_str_key_matcher(void *node) {
  return ((dictnode *) node)->key_matcher(
      ((dictnode *) node)->key, _match_key, &_str_key_matcher);
}

static int _generic_key_matcher(void *node) {
  return ((dictnode *) node)->key_matcher(
      ((dictnode *) node)->key, _match_key, _match_key_matcher);
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
    void *key, void *value, uint64_t hash,
    int (*key_matcher)(const void *, const void *, const void *),
    void *(*alloc)(size_t)) {
  dictnode *ret = dictnode_alloc_with(alloc);
  FLY_ERR_CLEAR;
  if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
  } else {
    ret->key = key;
    ret->value = value;
    ret->hash = hash;
    ret->key_matcher = key_matcher;
  }
  return ret;
}

FLYAPI void dictnode_del(dictnode *dnode, void (*del)(void *)) {
  FLY_ERR_CLEAR;
  if (dnode != NULL) {
    if (dnode->key_matcher == &_str_key_matcher) {
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

static dict *_dict_init_with(
    dict *d, const size_t size,
    void *(*alloc)(size_t), void (*del)(void *)) {
  if (d != NULL) {
    if (alloc == &malloc) {
      d->buckets = (struct dbucket *)
        calloc(size, sizeof (struct dbucket));
    } else {
      d->buckets = (struct dbucket *)
        memset((*alloc)(size * sizeof(struct dbucket)), 0, size);
    }

    if(d->buckets == NULL) {
      FLY_ERR(EFLYNOMEM);
    } else {
      d->size = 0;
      d->exponent = llogb((double) size);
      d->alloc = alloc;
      d->del = del;

      FLY_ERR_CLEAR;
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }

  return d;
}

static inline int _check_power_of_two(const size_t size) {
  if (size <= 1 || (size & size - 1)) {
    FLY_ERR(EFLYBADARG);
    return 0;
  }

  return 1;
}

FLYAPI void dict_init_with(
    dict *d, const size_t size,
    void *(*alloc)(size_t), void (*del)(void *)) {
  if (_check_power_of_two(size)) {
    _dict_init_with(d, size, alloc, del);
  }
}

FLYAPI void dict_init(dict *d, const size_t size) {
	dict_init_with(d, size, &malloc, &free);
}

FLYAPI dict *dict_new_with(
    const size_t size,
    void *(*alloc)(size_t), void (*del)(void *)) {
	dict *d;

  if (_check_power_of_two(size)) {
    if ((d = dict_alloc_with(alloc))) {
      return _dict_init_with(d, size, alloc, del);
    }

    FLY_ERR(EFLYNOMEM);
  }

	return NULL;
}

FLYAPI dict *dict_new_of_size(const size_t size) {
	return dict_new_with(size, &malloc, &free);
}

FLYAPI dict *dict_new() {
  dict *d = dict_alloc_with(&malloc);

  if (d) {
    return _dict_init_with(d, DEFAULT_SIZE, &malloc, &free);
  }

  FLY_ERR(EFLYNOMEM);
  return d;
}

FLYAPI void dict_del(dict *d) /*@-compdestroy@*/ {
  if (d != NULL) {
    register size_t i = 0;
    const size_t capacity = 1 << d->exponent;

    FLY_ERR_CLEAR;

    while (i < capacity) {
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

static int _should_relocate_node(void *node) {
  return (((dictnode *) node)->hash & _curr_bitmask) != 0;
}

static int _relocate_node(void *node, size_t unused_size) {
  struct dbucket * restrict bucket =
    _curr_dict->buckets + (((dictnode *) node)->hash & _next_bitmask);

  if (bucket->data) {
    if (!(bucket->flags & 0x1)) {
      list *bucket_list;

      if (_recycle_bin) {
        bucket_list = _recycle_bin;
        _recycle_bin = NULL;
      } else if (!(bucket_list =
            list_new_kind_with(
              LISTKIND_SLINK, _curr_dict->alloc, _curr_dict->del))) {
        FLY_ERR(EFLYNOMEM);
        return 1;
      }

      list_push(bucket_list, bucket->data);
      bucket->data = bucket_list;
      bucket->flags |= 0x1;
    }

    list_push(bucket->data, node);
  } else {
    bucket->data = node;
  }

  return 0;
}

static int _dict_resize(dict *d) {
  register size_t i;
  struct dbucket * restrict buckets;
  const size_t og_capacity = _curr_bitmask = 1 << d->exponent;

  _curr_dict = d;
  _next_bitmask = (_curr_bitmask << 1) - 1;

  if (d->alloc == &malloc) {
    buckets = d->buckets =
      realloc(d->buckets, (1 << ++(d->exponent)) * sizeof (struct dbucket));

    if (!buckets) {
      return 0;
    }
  } else {
    buckets = d->alloc((1 << ++(d->exponent)) * sizeof (struct dbucket));

    if (!buckets) {
      return 0;
    }

    memcpy(buckets, d->buckets, og_capacity * sizeof (struct dbucket));
    d->del(d->buckets);
    d->buckets = buckets;
  }

  memset(buckets + og_capacity, 0, og_capacity * sizeof (struct dbucket));

  FLY_ERR_CLEAR;

  for (i = 0; i < og_capacity; ++i) {
    if (buckets[i].data) {
      if (buckets[i].flags & 0x1) {
        list_remove_all(
            buckets[i].data, &_should_relocate_node, &_relocate_node);

        size_t size;

        if ((size = list_size(buckets[i].data)) <= 1) {
          list *ptr_save;
          buckets[i].data = list_pop(ptr_save = buckets[i].data);
          buckets[i].flags = size ? buckets[i].flags & ~0x1 : 0x0;

          if (_recycle_bin) {
            list_del(_recycle_bin);
          }
          _recycle_bin = ptr_save;
        }
      } else if (_should_relocate_node(buckets[i].data)) {
        if (_relocate_node(buckets[i].data, 0)) {
          break;  /* out of memory */
        }
        buckets[i] = (const struct dbucket) { 0 };
      }
    }
  }

  _curr_dict = NULL;

  int ret = flytools_last_error();

  if (_recycle_bin) {
    list_del(_recycle_bin);
    _recycle_bin = NULL;
  }

  return ret;
}

#define WITH_NEW_NODE_OR_DIE(operation) \
  if (!(node = dictnode_new( \
          key_matcher == &_str_key_matcher ? strdup(key) : key, \
          value, hash, key_matcher, d->alloc))) { \
    return; \
  } \
  operation

#define RESIZE_AND_RESTART_ON_LOAD_FACTOR_BREACH(d, amount) \
  if (100 * (d->size + amount) >> d->exponent > LOAD_FACTOR) { \
    if (_dict_resize(d)) { \
      FLY_ERR(EFLYNOMEM); \
      return; \
    } \
    goto start; \
  } \

static void _dict_set_bucket_atomic(
    dict * restrict d, void *key, void *value, uint64_t hash,
    int (*key_matcher)(const void *, const void *, const void *)) {
  struct dbucket * restrict bucket;
  dictnode *node;  /* Also used implicitly in macro expansion */

start:
  bucket = d->buckets + (hash & (1 << d->exponent) - 1);

  if (!bucket->data) {
    RESIZE_AND_RESTART_ON_LOAD_FACTOR_BREACH(d, 1);
    WITH_NEW_NODE_OR_DIE(bucket->data = node);

    d->size++;
    return;
  }

  if (bucket->flags & 0x1) {
    _match_key = key;
    _match_key_matcher = key_matcher;

    node = list_find_first(bucket->data, _generic_key_matcher);

    _match_key = NULL;
    _match_key_matcher = NULL;

    if (node) {
      node->value = value;
      return;
    }

    RESIZE_AND_RESTART_ON_LOAD_FACTOR_BREACH(d, 1);
  } else {
    node = bucket->data;

    if (node->key_matcher(node->key, key, key_matcher)) {
      node->value = value;
      return;
    }

    RESIZE_AND_RESTART_ON_LOAD_FACTOR_BREACH(d, 1);

    list *bucket_list;

    if (_recycle_bin) {
      bucket_list = _recycle_bin;
      _recycle_bin = NULL;
    } else if (!(bucket_list =
          list_new_kind_with(LISTKIND_SLINK, d->alloc, d->del))) {
      FLY_ERR(EFLYNOMEM);
      return;
    }

    list_push(bucket_list, node);
    bucket->data = bucket_list;
    bucket->flags |= 0x1;
  }

  WITH_NEW_NODE_OR_DIE(list_push(bucket->data, node));

  /* Will be skipped if there's an error inside WITH_NEW_NODE_OR_DIE */
  d->size++;
}

#undef WITH_NEW_NODE_OR_DIE
#undef RESIZE_AND_RESTART_ON_LOAD_FACTOR_BREACH

FLYAPI void dict_set(dict * restrict d, void *key, void *value) {
  if (d) {
    FLY_ERR_CLEAR;

    _dict_set_bucket_atomic(
        d, key, value, hash_xorshift64s((uint64_t) key), &_ptr_key_matcher);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_sets(dict * restrict d, char *key, void *value) {
  if (d && key) {
    FLY_ERR_CLEAR;

    _dict_set_bucket_atomic(
        d, key, value, hash_string(key), &_str_key_matcher);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

static inline dictnode *_dict_lookup_using(
    dict * restrict d, void *key,
    dictnode *(*lookup_proc)(dict * restrict, void *)) {
  FLY_ERR_CLEAR;

  if (!d) {
    FLY_ERR(EFLYBADARG);
    return NULL;
  }

  _match_key = key;
  dictnode *found = lookup_proc(d, key);
  _match_key = NULL;
  return found;
}

static dictnode *_dict_remove_from_bucket(
    struct dbucket * restrict bucket, void *key, int (*matcher)(void *)) {
  dictnode *node;

  if (bucket->flags & 0x1) {
    if (list_size((list *) bucket->data) > 1) {
      _match_key = key;
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

#define BUCKET_PTR_INDEX(d, key) \
  (hash_xorshift64s((uint64_t) key) & (1 << d->exponent) - 1)
#define BUCKET_STR_INDEX(d, key) \
  (hash_string(key) & (1 << d->exponent) - 1)

static dictnode *_dict_remove_keyed_ptr(dict * restrict d, void *key) {
  return _dict_remove_from_bucket(
      d->buckets + BUCKET_PTR_INDEX(d, key), key, &_list_ptr_key_matcher);
}

static dictnode *_dict_remove_keyed_str(dict * restrict d, void *key) {
  return _dict_remove_from_bucket(
      d->buckets + BUCKET_STR_INDEX(d, key), key, &_list_str_key_matcher);
}

#define _dict_remove_using(proc) \
  dictnode *node = _dict_lookup_using(d, key, proc); \
  if (node) { \
    void *value = node->value; \
    d->size--; \
    dictnode_del(node, d->del); \
    return value; \
  } \
  return NULL;

FLYAPI void *dict_remove(dict * restrict d, void *key) {
  _dict_remove_using(&_dict_remove_keyed_ptr);
}

FLYAPI void *dict_removes(dict * restrict d, char *key) {
  _dict_remove_using(&_dict_remove_keyed_str);
}

#undef _dict_remove_using

static inline dictnode *_dict_find_in_bucket(
    struct dbucket * restrict bucket, void *key, int (*matcher)(void *)) {
  if (bucket->flags & 0x1) {
    _match_key = key;
    return (dictnode *)
      list_find_first((list *) bucket->data, matcher);
  }

  return (dictnode *) bucket->data;
}

static dictnode *_dict_find_keyed_ptr(dict * restrict d, void *key) {
  return _dict_find_in_bucket(
      d->buckets + BUCKET_PTR_INDEX(d, key), key, &_list_ptr_key_matcher);
}

static dictnode *_dict_find_keyed_str(dict * restrict d, void *key) {
  return _dict_find_in_bucket(
      d->buckets + BUCKET_STR_INDEX(d, key), key, &_list_str_key_matcher);
}

FLYAPI void *dict_get(dict * restrict d, void *key) {
  dictnode *node = _dict_lookup_using(d, key, &_dict_find_keyed_ptr);
  return node ? node->value : NULL;
}

FLYAPI void *dict_gets(dict * restrict d, char *key) {
  dictnode *node = _dict_lookup_using(d, key, &_dict_find_keyed_str);
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
