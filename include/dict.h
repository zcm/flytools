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

#include "hash.h"
#include "dict.h"

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
	char *key; //!< The key string for this node.
} dictnode;

/**
 * A structure that represents a dictionary abstract data type (ADT). This
 * structure should be used explicitly with the provided dictionary-specific
 * API functions.
 */
typedef struct dict {
	/**
	 * An array of dllist instances that represent the buckets in this
	 * dictionary. The size of this array is stored in the maxsize member.
	 * @see maxsize
	 */
	dllist **buckets;
	unsigned int size; //!< The number of elements stored in this dictionary.
	/**
	 * The number of buckets in this dictionary. Named because it is the maximum
	 * size for hash compression (and for array indices).
	 */
	unsigned int maxsize;
	/**
	 * The stored allocation routine for allocating new nodes. This member is
	 * also stored in the buckets for this dictionary.
	 * @see dllist
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
dictnode *dictnode_alloc_with(void *(*alloc_callback)(size_t));
/**
 * Allocates a new node for a dictionary. Does no initialization.
 * @return a pointer to the newly allocated dictionary node
 */
dictnode *dictnode_alloc();
/**
 * Creates and initializes a new dictionary node with the specified key/value
 * pair and the allocation callback. No hashing is done at this time.
 * @param key the key string used for this element in the dictionary
 * @param data the generic pointer to the element stored in this node
 * @param alloc_callback the callback with which to allocate the new node
 * @return a pointer to the newly allocated dictionary node
 */
dictnode *dictnode_create_with(const char * restrict key, void *data, void *(*alloc_callback)(size_t));
/**
 * Creates and initializes a new dictionary node with the specified key/value
 * pair. No hashing is done at this time.
 * @param key the key string used for this element in the dictionary
 * @param data the generic pointer to the element stored in this node
 * @return a pointer to the newly allocated dictionary node
 */
dictnode *dictnode_create(const char * restrict key, void *data);
/**
 * Frees the specified dictionary node with the given callback function. Since
 * the callback function cannot be set with a destructor, this function must be
 * called when freeing with a non-standard freeing routine.
 * @param dnode the dictionary node to destroy
 * @param free_callback the callback function used to destroy the node
 */
void dictnode_destroy_with(dictnode *dnode, void (*free_callback)(void *));
/**
 * Frees the specified dictionary node. Uses the default freeing routine,
 * free().
 * @param dnode the dictionary node to destroy
 */
void dictnode_destroy(dictnode *dnode);

/**
 * Allocates a new dictionary using the specified callback function. Does no
 * initialization.
 * @param alloc_callback the callback with which to allocate the new node
 * @return a pointer to the newly allocated dictionary
 */
dict *dict_alloc_with(void *(*alloc_callback)(size_t));
/**
 * Allocates a new dictionary. Does no initialization.
 * @return a pointer to the newly allocated dictionary
 */
dict *dict_alloc();
/**
 * Frees the specified dictionary using the given callback function. Normally,
 * one does not need to call this function since the destructor should be set at
 * the time the dictionary is created. This function is typically called
 * internally when the dictionary is destroyed.
 * @param d the dictionary to destroy
 * @param free_callback the callback function used to free the dictionary
 * @see dict_set_destructor()
 */
void dict_destroy_with(dict *d, void (*free_callback)(void *));
/**
 * Frees the specified dictionary. Uses the default freeing routine, free(),
 * unless a destructor callback has been set for the given dictionary. In that
 * case, that callback function is used instead of free().
 * @param d the dictionary to destroy
 * @see dict_set_destructor()
 */
void dict_destroy(dict *d);
/**
 * Initializes the specified dictionary with a bucket array of size, using the
 * given allocation callback. The bucket array is an array of doubly linked
 * lists (dllist *). The performance of this dictionary is much, much better
 * when the number of buckets chosen is a prime number. This is due to the
 * inherent inefficiencies in non-prime hash compression.
 * @param d the dictionary to initialize
 * @param size the number of buckets for this dictionary
 * @param alloc_callback the callback function for allocating this dictionary
 * @see dllist
 */
void dict_init_with(dict *d, const unsigned int size, void *(*alloc_callback)(size_t));
/**
 * Initializes the specified dictionary with a bucket array of size. The bucket
 * array is an array of doubly linked lists (dllist *). The performance of this
 * dictionary is much, much better when the number of buckets chosen is a prime
 * number. This is due to the inherent inefficiencies in non-prime hash
 * compression.
 * @param d the dictionary to initialize
 * @param size the number of buckets for this dictionary
 * @see dllist
 */
void dict_init(dict *d, const unsigned int size);
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
dict *dict_create_with(const unsigned int size, void *(*alloc_callback)(size_t));
/**
 * Allocates and initializes a new dictionary with the specified number of
 * buckets (size). The performance of this dictionary is much, much better when
 * the number of buckets chosen is a prime number. This is due to the inherent
 * inefficiencies in non-prime hash compression.
 * @param size the number of buckets for this dictionary
 * @return a pointer to the newly created dictionary
 */
dict *dict_create(const unsigned int size);
/**
 * Sets the freeing routine for the specified dictionary. This function should
 * be called at creation time or soon after so that the correct destructor is
 * used to free any memory that may be destroyed. If the destructor is not
 * properly set, segmentation faults may occur.
 * @param d the dictionary for which we are setting the destructor
 * @param free_callback the freeing routine to be set as this dictionary's
 * destructor
 * @see dict_remove()
 * @see dict_destroy()
 */
void dict_set_destructor(dict *d, void (*free_callback)(void *));
/**
 * Gets the index in a dictionary d for the specified key. That is, the key is
 * hashed and then compressed based on the parameters for the given dictionary.
 * @param d the dictionary for which to calculate this hash index
 * @param key the key for which to calculate the index
 * @return the calculated hash index for the specified key
 */
unsigned int dict_get_hash_index(dict * restrict d, const char * restrict key);
/**
 * Inserts a value into the specified dictionary with they given key. That is,
 * the value argument is associated with the given key in the dictionary.
 * @param d the dictionary in which to associate the value with the key
 * @param key the key string to associate with the value
 * @param value the value that is being inserted into the dictionary
 */
void dict_insert(dict * restrict d, const char * restrict key, void *value);
/**
 * Finds and removes a value for the given key from the specified dictionary.
 * The value found is the value returned. In the event that there are multiple
 * values with the same key in the dictionary, only one of them is returned.
 * Specifically, the first one found is returned, which should be the first one
 * inserted, but technically this behavior is undefined and can change in a
 * later version. If the value is not found, no value is removed from the list
 * and this function returns NULL.
 * @param d the dictionary from which to remove the value with the given key
 * @param key the key string for the value desired
 * @return NULL if the value is not found; otherwise, a pointer to that value
 */
void *dict_remove(dict * restrict d, const char * restrict key);
/**
 * Finds a value for the given key from the specified dictionary. The value
 * found is the value returned. In the event that there are multple values with
 * the smae key in the dictioanry, only one of them is returned. Specifically,
 * the first one found is returned, which should be the first one inserted, but
 * technically this behavior is undefined and can change in a later version.
 * This also means that one can only ever find the first value returned until
 * the multiple values are removed from the dictionary.
 * @param d the dictionary to search for the value with the given key
 * @param key the key string for the value desired
 * @return NULL if the value is not found; otherwise a pointer to that value
 */
void *dict_find(dict * restrict d, const char * restrict key);
/**
 * Iterates through the dictionary, applying the specified callback function to
 * each node in the dictionary.
 * @param d the dictionary through which to iterate
 * @param callback the callback function to apply to all of the nodes
 */
void dict_iterate_callback(dict *d, void (*callback)(dictnode *));

#if __STDC_VERSION__ < 199901L
#undef restrict
#endif

#endif
