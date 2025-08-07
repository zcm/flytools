/** @file dict.h
 * This is the header file for the dictionary type contained in the Flytools.
 * Simply include this file with in your source code to access the Flytools
 * dictionary abstract data type.
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

#ifndef __ZCM_DICT_H__
#define __ZCM_DICT_H__

#include "common.h"
#include "hash.h"
#include "list.h"

#include "jargon.h"

/** \defgroup Dictionaries
 * The \ref dict type defines dictionaries in the Flytools API.
 * @{
 */

struct dbucket;  //!< Single bucket for a \ref dict.
struct dictnode;

/**
 * A structure that represents a dictionary abstract data type (ADT). This
 * structure should be used explicitly with the provided dictionary-specific
 * API functions.
 */
typedef struct dict {
  size_t size;             //!< Number of elements stored in this \ref dict.
  size_t exponent;         //!< Capacity in log<sub>2</sub>(# of buckets) terms.
  struct dbucket *buckets; //!< Array of containers for \ref dict elements.
  struct dictnode **items; //!< Linear array of elements for iteration, etc.
} dict;

/**
 * Frees the given dictionary.
 *
 * @param d the dictionary to destroy
 */
FLYAPI void dict_del(dict *d);

/**
 * Initializes a dictionary with a bucket array of size `size`. `size` must be a
 * power of 2 greater than 1; otherwise this method sets the `EFLYBADARG` error
 * and returns null.
 *
 * @param d the dictionary to initialize
 * @param size the number of buckets for this dictionary
 * @return the parameter `d` unmodified on success, `NULL` otherwise
 */
FLYAPI dict *dict_init(dict *d, const size_t size);

/**
 * Allocates and initializes a new dictionary with the specified number of
 * buckets. `size` must be a power of 2 greater than 1; otherwise this method
 * sets the `EFLYBADARG` error and returns null.
 *
 * @param size the number of buckets for this dictionary
 * @return a pointer to the newly created dictionary
 */
FLYAPI dict *dict_new_of_size(const size_t size);

#define DICT_DEFAULT_SIZE 16

/**
 * Creates a new dictionary using all defaults.
 *
 * @return a pointer to the newly created dictionary
 */
__attribute__((artificial))
FLYAPI inline dict *dict_new() {
	return dict_new_of_size(DICT_DEFAULT_SIZE);
}

/**
 * Inserts a value into the specified dictionary with the given object key. That
 * is, the value argument is associated with the pointer in the dictionary.
 * @param d the dictionary in which to associate the value with the key
 * @param key the object key pointer to associate with the value
 * @param value the value that is being inserted into the dictionary
 */
FLYAPI void dict_set(dict * restrict d, void *key, void *value);
/**
 * Inserts a value into the specified dictionary with the given string key. That
 * is, the value argument is associated with the given key in the dictionary.
 * @param d the dictionary in which to associate the value with the key
 * @param key the key string to associate with the value
 * @param value the value that is being inserted into the dictionary
 */
FLYAPI void dict_sets(dict * restrict d, char *key, void *value);
/**
 * Finds and removes a value for the given object key from the specified
 * dictionary.  The value found is the value returned. In the event that there
 * are multiple values with the same key in the dictionary, only one of them is
 * returned.  Specifically, the first one found is returned, which should be the
 * first one inserted, but technically this behavior is undefined and can change
 * in a later version. If the value is not found, no value is removed from the
 * list and this function returns NULL.
 * @param d the dictionary from which to remove the value with the given key
 * @param key the key string for the value desired
 * @return NULL if the value is not found; otherwise, a pointer to that value
 */
FLYAPI void *dict_remove(dict * restrict d, void *key);
/**
 * Finds and removes a value for the given string key from the specified
 * dictionary.  The value found is the value returned. In the event that there
 * are multiple values with the same key in the dictionary, only one of them is
 * returned.  Specifically, the first one found is returned, which should be the
 * first one inserted, but technically this behavior is undefined and can change
 * in a later version. If the value is not found, no value is removed from the
 * list and this function returns NULL.
 * @param d the dictionary from which to remove the value with the given key
 * @param key the key string for the value desired
 * @return NULL if the value is not found; otherwise, a pointer to that value
 */
FLYAPI void *dict_removes(dict * restrict d, char *key);
/**
 * Finds a value for the given object key from the specified dictionary. The
 * value found is the value returned. In the event that there are multple values
 * with the smae key in the dictioanry, only one of them is returned.
 * Specifically, the first one found is returned, which should be the first one
 * inserted, but technically this behavior is undefined and can change in a
 * later version.  This also means that one can only ever find the first value
 * returned until the multiple values are removed from the dictionary.
 * @param d the dictionary to search for the value with the given key
 * @param key the key string for the value desired
 * @return NULL if the value is not found; otherwise a pointer to that value
 */
FLYAPI void *dict_get(dict * restrict d, void *key);
/**
 * Finds a value for the given string key from the specified dictionary. The
 * value found is the value returned. In the event that there are multple values
 * with the smae key in the dictioanry, only one of them is returned.
 * Specifically, the first one found is returned, which should be the first one
 * inserted, but technically this behavior is undefined and can change in a
 * later version.  This also means that one can only ever find the first value
 * returned until the multiple values are removed from the dictionary.
 * @param d the dictionary to search for the value with the given key
 * @param key the key string for the value desired
 * @return NULL if the value is not found; otherwise a pointer to that value
 */
FLYAPI void *dict_gets(dict * restrict d, char *key);

/**
 * Iterates through the dictionary, applying the specified callback function to
 * each item.
 * @param d the dictionary through which to iterate
 * @param fn the callback function to apply to all of the nodes
 */
FLYAPI void dict_foreach(dict *d, int (*fn)(void *, size_t));

/** @} */

#include "unjargon.h"

#endif
