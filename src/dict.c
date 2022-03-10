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

int dictnode_key_str_matches(
    const struct dictnode * restrict node, const void * restrict key,
    const void * restrict expected_func) {

  return expected_func == (void *) &dictnode_key_str_matches
    && strcmp((char *) node->key, (char *) key) == 0;
}

int dictnode_key_ptr_matches(
    const struct dictnode * restrict node, const void * restrict key,
    const void * restrict expected_func) {
  return expected_func == (void *) &dictnode_key_ptr_matches
    && node->key == key;
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
    void * restrict key, void *data,
    int (*matches)(const struct dictnode *, const void *, const void *),
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

FLYAPI void dictnode_del_with(dictnode *dnode, void (*free_callback)(void *)) {
  FLY_ERR_CLEAR;
  if (dnode != NULL) {
    if (dnode->matches == &dictnode_key_str_matches) {
      (*free_callback)(dnode->key);
    }
    (*free_callback)(dnode);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dictnode_del(dictnode *dnode) {
	dictnode_del_with(dnode, &free);
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

FLYAPI void dict_init_with(dict *d, const unsigned int size, void *(*alloc_callback)(size_t)) {
	register unsigned int i = 0;
  FLY_ERR_CLEAR;
  if (d != NULL) {
    d->buckets = (list **)(*alloc_callback)(size * sizeof(list *));
    if(d->buckets == NULL) {
      FLY_ERR(EFLYNOMEM);
    } else {
      d->size = 0;
      d->maxsize = size;
      d->alloc_callback = alloc_callback;
      d->free_callback = NULL;
      while(i < size) {
        d->buckets[i] = list_new_with(alloc_callback);
        if (d->buckets[i] == NULL) {
          FLY_ERR(EFLYNOMEM);
          break;
        }
        i++;
      }
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_init(dict *d, const unsigned int size) {
	dict_init_with(d, size, &malloc);
}

FLYAPI dict *dict_new_with(const size_t size, void *(*alloc_callback)(size_t)) {
	dict *ret = dict_alloc_with(alloc_callback);
  FLY_ERR_CLEAR;
	if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
	} else {
    dict_init_with(ret, size, alloc_callback);
  }
	return ret;
}

FLYAPI dict *dict_new(const size_t size) {
	return dict_new_with(size, &malloc);
}

FLYAPI void dict_del_with(dict *d, void (*free_callback)(void *)) /*@-compdestroy@*/ {
  unsigned int i = 0;
  FLY_ERR_CLEAR;
  if (d != NULL) {
    while(i < d->maxsize) {
      while(list_size(d->buckets[i]) > 0) {
        dictnode *this_node = list_pop(d->buckets[i]);
        dictnode_del(this_node);
      }
      assert(list_size(d->buckets[i]) == 0);
      list_del(d->buckets[i]);
      i++;
    }
    (*free_callback)(d->buckets);
    (*free_callback)(d);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_del(dict *d) {
	dict_del_with(d, !d->free_callback ? &free : d->free_callback);
}

FLYAPI void dict_set_freeproc(dict *d, void (*free_callback)(void *)) {
  unsigned int i = 0;
  FLY_ERR_CLEAR;
  if (d != NULL) {
    d->free_callback = free_callback;
    while(i < d->maxsize) {
      list_set_freeproc(d->buckets[i], free_callback);
      i++;
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI unsigned int dict_get_hash_index(dict * restrict d, const char * restrict key) {
  FLY_ERR_CLEAR;
  if (d != NULL && key != NULL) {
    return hash_string(key) % d->maxsize;
  } else {
    FLY_ERR(EFLYBADARG);
  }
	return 0;
}

static inline void __dict_put_to_bucket_atomic(
    list *bucket, dictnode *node, size_t *size) {
  list_push(bucket, node);
  ++*size;
}

FLYAPI void dict_put(dict * restrict d, void * restrict key, void *value) {
  FLY_ERR_CLEAR;

  if (d) {
    __dict_put_to_bucket_atomic(
        d->buckets[(size_t) key % d->maxsize],
        dictnode_new(
            key, value, &dictnode_key_ptr_matches, d->alloc_callback),
        &d->size);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_puts(dict * restrict d, char * restrict key, void *value) {
  FLY_ERR_CLEAR;

  if (d != NULL && key != NULL) {
    __dict_put_to_bucket_atomic(
        d->buckets[hash_string(key) % d->maxsize],
        dictnode_new(
            strdup(key), value, &dictnode_key_str_matches, d->alloc_callback),
        &d->size);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void *dict_remove(dict * restrict d, const void * restrict key) {
  FLY_ERR_CLEAR;

  if (!d) {
    FLY_ERR(EFLYBADARG);
    return NULL;
  }

  int __key_ptr_matches(void *node) {
    return ((dictnode *) node)->matches(
        (dictnode *) node, key, &dictnode_key_ptr_matches);
  }

  dictnode *found =
    (dictnode *) list_remove_first(
        d->buckets[(size_t) key % d->maxsize], &__key_ptr_matches);

  if (found) {
    d->size--;
    return found->data;
  }

  return NULL;
}

FLYAPI void *dict_removes(dict * restrict d, const char * restrict key) {
  FLY_ERR_CLEAR;

  if (!(d && key)) {
    FLY_ERR(EFLYBADARG);
  }

  int __key_str_matches(void *node) {
    return ((dictnode *) node)->matches(
        (dictnode *) node, key, &dictnode_key_str_matches);
  }

  dictnode *found =
    (dictnode *) list_remove_first(
        d->buckets[dict_get_hash_index(d, key)], &__key_str_matches);

  if (found) {
    d->size--;
    return found->data;
  }

  return NULL;
}

FLYAPI void *dict_get(dict * restrict d, void * restrict key) {
  size_t index;
  unsigned int checked = 0;
  FLY_ERR_CLEAR;
  if (d) {
    index = (size_t) key % d->maxsize;
    while (checked < list_size(d->buckets[index])) {
      dictnode *dnode = (dictnode *)list_pop(d->buckets[index]);
      if (dnode->matches(dnode, key, &dictnode_key_ptr_matches)) {
        list_unshift(d->buckets[index], (void *)dnode);
        return dnode->data;
      }
      checked++;
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
  return NULL;
}

FLYAPI void *dict_gets(dict * restrict d, const char * restrict key) {
  unsigned int index;
  void *ret = NULL;
  unsigned int checked = 0;
  int found = 0;
  FLY_ERR_CLEAR;
  if (d != NULL && key != NULL) {
    index = dict_get_hash_index(d, key);
    while (!found && checked < list_size(d->buckets[index])) {
      dictnode *dnode = (dictnode *)list_pop(d->buckets[index]);
      if (dnode->matches(dnode, key, &dictnode_key_str_matches)) {
        ret = dnode->data;
        found = 1;
        list_unshift(d->buckets[index], (void *)dnode);
      }
      checked++;
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
  return ret;
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
