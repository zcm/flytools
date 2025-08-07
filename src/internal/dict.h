#ifndef __ZCM_INTERNAL_DICT_H__
#define __ZCM_INTERNAL_DICT_H__

struct dbucket {
  uint_fast8_t flags;
  void *data;
};

//! Record container for a single `\ref dict` key-value pair.
typedef struct dictnode {
	void *key;     //!< Key pointer. Can be `char *` or generic `void *`.
	void *value;   //!< Data pointer.
  uint64_t hash; //!< Full uncompressed hash of `key`.
  size_t index;  //!< Index into parent dict's `items` array.

  //! Callback that compares keys for equality in lookup operations.
  int (*key_matcher)(const void *, const void *, const void *);
} dictnode;

/**
 * Creates and initializes a new dictionary node with the specified key/value
 * pair and the allocation callback. No hashing is done at this time.
 * @param key the key string used for this element in the dictionary
 * @param data the generic pointer to the element stored in this node
 * @param hash the precomputed uncompressed hash of the key
 * @param matches the matcher used to compare keys
 * @param alloc the callback with which to allocate the new node
 * @return a pointer to the newly allocated dictionary node
 */
dictnode *dictnode_new(
    void *key, void *data, uint64_t hash,
    int (*matches)(const void *, const void *, const void *));

/**
 * Frees the specified dictionary node with the given callback function. Since
 * the callback function cannot be set with a destructor, this function must be
 * called when freeing with a non-standard freeing routine.
 * @param dnode the dictionary node to destroy
 * @param del the callback function used to destroy the node
 */
void dictnode_del(dictnode *dnode);

#endif
