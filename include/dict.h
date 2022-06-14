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

#if __STDC_VERSION__ < 199901L
#define restrict
#endif

/** \defgroup Dictionaries
 * The \ref dict type defines dictionaries in the Flytools API.
 * @{
 */

//! Single bucket for a \ref dict.
struct dictbucket;

/**
 * A structure that represents a dictionary abstract data type (ADT). This
 * structure should be used explicitly with the provided dictionary-specific
 * API functions.
 */
typedef struct dict {
	size_t size;                //!< Number of elements stored in this \ref dict.
	size_t capacity;            //!< Number of buckets in this \ref dict.
	struct dictbucket *buckets; //!< Array of containers for \ref dict elements.
	void *(*alloc)(size_t);     //!< Allocator for internal structures.
	void (*del)(void *);        //!< Deallocator for internal structures.
} dict;

/**
 * Allocates a new dictionary using the specified callback function. Does no
 * initialization.
 * @param alloc the callback with which to allocate the new node
 * @return a pointer to the newly allocated dictionary
 */
FLYAPI dict *dict_alloc_with(void *(*alloc)(size_t));
/**
 * Allocates a new dictionary. Does no initialization.
 * @return a pointer to the newly allocated dictionary
 */
FLYAPI dict *dict_alloc();
/**
 * Frees the specified dictionary. Uses the default freeing routine, free(),
 * unless a destructor callback has been set for the given dictionary. In that
 * case, that callback function is used instead of free().
 * @param d the dictionary to destroy
 * @see dict_set_destructor()
 */
FLYAPI void dict_del(dict *d);

/**
 * Initializes a dictionary with a bucket array of size `size` and the given
 * allocator. `size` must be a power of 2 greater than 1; otherwise this
 * method sets the `EFLYBADARG` error and returns null.
 *
 * @param d the dictionary to initialize
 * @param size the number of buckets for this dictionary
 * @param alloc the callback function for allocating this dictionary
 * @see list
 */
FLYAPI void dict_init_with(
    dict *d, const size_t size,
    void *(*alloc)(size_t), void (*del)(void *));

/**
 * Initializes a dictionary with a bucket array of size `size`. `size` must be a
 * power of 2 greater than 1; otherwise this method sets the `EFLYBADARG` error
 * and returns null.
 *
 * @param d the dictionary to initialize
 * @param size the number of buckets for this dictionary
 * @see list
 */
FLYAPI void dict_init(dict *d, const size_t size);

/**
 * Allocates and initializes a new dictionary with the specified number of
 * buckets and the given allocator. `size` must be a power of 2 greater than 1;
 * otherwise this method sets the `EFLYBADARG` error and returns null.
 *
 * @param size the number of buckets for this dictionary
 * @param alloc the callback function for allocating this dictionary
 * @return a pointer to the newly created dictionary
 */
FLYAPI dict *dict_new_with(
    const size_t size,
    void *(*alloc)(size_t), void (*del)(void *));

/**
 * Allocates and initializes a new dictionary with the specified number of
 * buckets. `size` must be a power of 2 greater than 1; otherwise this method
 * sets the `EFLYBADARG` error and returns null.
 *
 * @param size the number of buckets for this dictionary
 * @return a pointer to the newly created dictionary
 */
FLYAPI dict *dict_new_of_size(const size_t size);

/**
 * Creates a new dictionary using all defaults.
 *
 * @return a pointer to the newly created dictionary
 */
FLYAPI dict *dict_new();

/**
 * Sets the freeing routine for the specified dictionary. This function should
 * be called at creation time or soon after so that the correct destructor is
 * used to free any memory that may be destroyed. If the destructor is not
 * properly set, segmentation faults may occur.
 * @param d the dictionary for which we are setting the destructor
 * @param del the freeing routine to be set as this dictionary's
 * destructor
 * @see dict_remove()
 * @see dict_del()
 */
FLYAPI void dict_set_destructor(dict *d, void (*del)(void *));
/**
 * Gets the index in a dictionary d for the specified key. That is, the key is
 * hashed and then compressed based on the parameters for the given dictionary.
 * @param d the dictionary for which to calculate this hash index
 * @param key the key for which to calculate the index
 * @return the calculated hash index for the specified key
 */
FLYAPI unsigned int dict_get_hash_index(dict * restrict d, const char *key);
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
 * each item in the dictionary.
 * @param d the dictionary through which to iterate
 * @param callback the callback function to apply to all of the nodes
 */
FLYAPI void dict_iterate_callback(dict *d, void (*callback)(void *));

/** @} */

#if __STDC_VERSION__ < 199901L
#undef restrict
#endif

#endif
