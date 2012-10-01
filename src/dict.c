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
  FLY_ERR_CLEAR;
	if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
	}
	return (dictnode *)ret;
}

FLYAPI dictnode *dictnode_alloc() {
	return dictnode_alloc_with(&malloc);
}

FLYAPI dictnode *dictnode_create_with(const char * restrict key, void *data, void *(*alloc_callback)(size_t)) {
  dictnode *ret = dictnode_alloc_with(alloc_callback);
  FLY_ERR_CLEAR;
  if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
  } else {
    ret->key = strdup(key);
    ret->data = data;
  }
  return ret;
}

FLYAPI dictnode *dictnode_create(const char * restrict key, void *data) {
	return dictnode_create_with(key, data, &malloc);
}

FLYAPI void dictnode_destroy_with(dictnode *dnode, void (*free_callback)(void *)) {
  FLY_ERR_CLEAR;
  if (dnode != NULL) {
    (*free_callback)(dnode->key);
    (*free_callback)(dnode);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dictnode_destroy(dictnode *dnode) {
	dictnode_destroy_with(dnode, &free);
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
      while(i < size) {
        d->buckets[i] = list_create_with(alloc_callback);
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

FLYAPI dict *dict_create_with(const unsigned int size, void *(*alloc_callback)(size_t)) {
	dict *ret = dict_alloc_with(alloc_callback);
  FLY_ERR_CLEAR;
	if(ret == NULL) {
    FLY_ERR(EFLYNOMEM);
	} else {
    dict_init_with(ret, size, alloc_callback);
  }
	return ret;
}

FLYAPI dict *dict_create(const unsigned int size) {
	return dict_create_with(size, &malloc);
}

FLYAPI void dict_destroy_with(dict *d, void (*free_callback)(void *)) /*@-compdestroy@*/ {
  unsigned int i = 0;
  FLY_ERR_CLEAR;
  if (d != NULL) {
    while(i < d->maxsize) {
      while(list_get_size(d->buckets[i]) > 0) {
        dictnode *this_node = list_pop(d->buckets[i]);
        dictnode_destroy(this_node);
      }
      assert(list_get_size(d->buckets[i]) == 0);
      list_destroy(d->buckets[i]);
      i++;
    }
    (*free_callback)(d->buckets);
    (*free_callback)(d);
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void dict_destroy(dict *d) {
	dict_destroy_with(d, !d->free_callback ? &free : d->free_callback);
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
  unsigned int hash = 0;
  FLY_ERR_CLEAR;
  if (d != NULL && key != NULL) {
    hash = compress_hash(hash_string(key), d->maxsize);
  } else {
    FLY_ERR(EFLYBADARG);
  }
	return hash;
}

FLYAPI void dict_insert(dict * restrict d, const char * restrict key, void *value) {
	unsigned int index;
  FLY_ERR_CLEAR;
  if (d != NULL && key != NULL) {
    index = dict_get_hash_index(d, key);
    dictnode *newnode = dictnode_create_with(key, value, d->alloc_callback);
    list_push(d->buckets[index], newnode);
    d->size++;
  } else {
    FLY_ERR(EFLYBADARG);
  }
}

FLYAPI void *dict_remove(dict * restrict d, const char * restrict key) {
  unsigned int index;
  void *ret = NULL;
  unsigned int checked = 0;
  int found = 0;
  FLY_ERR_CLEAR;
  if (d != NULL && key != NULL) {
    index = dict_get_hash_index(d, key);
    while (!found && checked < list_get_size(d->buckets[index])) {
      dictnode *dnode = (dictnode *)list_pop(d->buckets[index]);
      if (strcmp(dnode->key, key) == 0) {
        ret = dnode->data;
        found = 1;
        dictnode_destroy(dnode);
        d->size--;
      } else {
        list_unshift(d->buckets[index], (void *)dnode);
      }
      checked++;
    }
  } else {
    FLY_ERR(EFLYBADARG);
  }
  return ret;
}

FLYAPI void *dict_find(dict *d, const char * restrict key) {
  unsigned int index;
  void *ret = NULL;
  unsigned int checked = 0;
  int found = 0;
  FLY_ERR_CLEAR;
  if (d != NULL && key != NULL) {
    index = dict_get_hash_index(d, key);
    while (!found && checked < list_get_size(d->buckets[index])) {
      dictnode *dnode = (dictnode *)list_pop(d->buckets[index]);
      if (strcmp(dnode->key, key) == 0) {
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
