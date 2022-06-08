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

/**
 * A structure that contains the information necessary for a single element in
 * a dictionary. This node represents a key/value pair for the dictionary.
 * @see dict
 */
typedef struct dictnode {
	void *data; //!< The data pointer for this node.
	void *key; //!< The key string/object for this node.
  int (*matches)(
      const struct dictnode *, const void *); //!< Key matcher for this node.
} dictnode;

struct dictbucket {
  uint_fast8_t flags;
  void *data;
};

/**
 * A structure that represents a dictionary abstract data type (ADT). This
 * structure should be used explicitly with the provided dictionary-specific
 * API functions.
 */
typedef struct dict {
	/**
	 * An array of list instances that represent the buckets in this
	 * dictionary. The size of this array is stored in the maxsize member.
	 * @see maxsize
	 */
	struct dictbucket *buckets;
	size_t size; //!< The number of elements stored in this dictionary.
	/**
	 * The number of buckets in this dictionary. Named because it is the maximum
	 * size for hash compression (and for array indices).
	 */
	size_t maxsize;
	/**
	 * The stored allocation routine for allocating new nodes. This member is
	 * also stored in the buckets for this dictionary.
	 * @see list
	 */
	void *(*alloc_callback)(size_t);
	/**
	 * The stored freeing routine for destroying the structure and nodes. If
	 * null, the standard free() will be used. This member should not be
	 * directly referenced -- it should be used only with the
	 * dict_set_destructor() function.
	 * @see dict_set_destructor()
	 */
	void (*free_callback)(void *);
} dict;

/**
 * Allocates a new dictionary node with the specified allocation callback. Does
 * no initialization.
 * @param alloc_callback the callback with which to allocate the new node
 * @return a pointer to the newly allocated dictionary node
 */
FLYAPI dictnode *dictnode_alloc_with(void *(*alloc_callback)(size_t));
/**
 * Allocates a new node for a dictionary. Does no initialization.
 * @return a pointer to the newly allocated dictionary node
 */
FLYAPI dictnode *dictnode_alloc();
/**
 * Creates and initializes a new dictionary node with the specified key/value
 * pair and the allocation callback. No hashing is done at this time.
 * @param key the key string used for this element in the dictionary
 * @param data the generic pointer to the element stored in this node
 * @param matches the matcher used to compare keys
 * @param alloc_callback the callback with which to allocate the new node
 * @return a pointer to the newly allocated dictionary node
 */
FLYAPI dictnode *dictnode_new(
    void *key, void *data,
    int (*matches)(const struct dictnode *, const void *),
    void *(*alloc_callback)(size_t));
/**
 * Frees the specified dictionary node with the given callback function. Since
 * the callback function cannot be set with a destructor, this function must be
 * called when freeing with a non-standard freeing routine.
 * @param dnode the dictionary node to destroy
 * @param free_callback the callback function used to destroy the node
 */
FLYAPI void dictnode_del(dictnode *dnode, void (*free_callback)(void *));

/**
 * Allocates a new dictionary using the specified callback function. Does no
 * initialization.
 * @param alloc_callback the callback with which to allocate the new node
 * @return a pointer to the newly allocated dictionary
 */
FLYAPI dict *dict_alloc_with(void *(*alloc_callback)(size_t));
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
 * Initializes the specified dictionary with a bucket array of size, using the
 * given allocation callback. The bucket array is an array of doubly linked
 * lists (list *). The performance of this dictionary is much, much better
 * when the number of buckets chosen is a prime number. This is due to the
 * inherent inefficiencies in non-prime hash compression.
 * @param d the dictionary to initialize
 * @param size the number of buckets for this dictionary
 * @param alloc_callback the callback function for allocating this dictionary
 * @see list
 */
FLYAPI void dict_init_with(
    dict *d, const size_t size,
    void *(*alloc_callback)(size_t), void (*free_callback)(void *));
/**
 * Initializes the specified dictionary with a bucket array of size. The bucket
 * array is an array of doubly linked lists (list *). The performance of this
 * dictionary is much, much better when the number of buckets chosen is a prime
 * number. This is due to the inherent inefficiencies in non-prime hash
 * compression.
 * @param d the dictionary to initialize
 * @param size the number of buckets for this dictionary
 * @see list
 */
FLYAPI void dict_init(dict *d, const size_t size);
/**
 * Allocates and initializes a new dictionary with the specified number of
 * buckets (size) and the given allocation callback. The performance of this
 * dictionary is much, much better when the number of buckets chosen is a prime
 * number. This is due to the inherent inefficiencies in non-prime hash
 * compression.
 * @param size the number of buckets for this dictionary 
 * @param alloc_callback the callback function for allocating this dictionary
 * @return a pointer to the newly created dictionary
 */
FLYAPI dict *dict_new_with(
    const size_t size,
    void *(*alloc_callback)(size_t), void (*free_callback)(void *));
/**
 * Allocates and initializes a new dictionary with the specified number of
 * buckets (size). The performance of this dictionary is much, much better when
 * the number of buckets chosen is a prime number. This is due to the inherent
 * inefficiencies in non-prime hash compression.
 * @param size the number of buckets for this dictionary
 * @return a pointer to the newly created dictionary
 */
FLYAPI dict *dict_new(const size_t size);
/**
 * Sets the freeing routine for the specified dictionary. This function should
 * be called at creation time or soon after so that the correct destructor is
 * used to free any memory that may be destroyed. If the destructor is not
 * properly set, segmentation faults may occur.
 * @param d the dictionary for which we are setting the destructor
 * @param free_callback the freeing routine to be set as this dictionary's
 * destructor
 * @see dict_remove()
 * @see dict_del()
 */
FLYAPI void dict_set_destructor(dict *d, void (*free_callback)(void *));
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
FLYAPI void dict_put(dict * restrict d, void *key, void *value);
/**
 * Inserts a value into the specified dictionary with the given string key. That
 * is, the value argument is associated with the given key in the dictionary.
 * @param d the dictionary in which to associate the value with the key
 * @param key the key string to associate with the value
 * @param value the value that is being inserted into the dictionary
 */
FLYAPI void dict_puts(dict * restrict d, char *key, void *value);
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
 * each node in the dictionary.
 * @param d the dictionary through which to iterate
 * @param callback the callback function to apply to all of the nodes
 */
FLYAPI void dict_iterate_callback(dict *d, void (*callback)(dictnode *));

#if __STDC_VERSION__ < 199901L
#undef restrict
#endif

#endif
