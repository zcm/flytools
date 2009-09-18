/** @file hash.h
 * This is the header file for the hash functions used in the dictionary type
 * provided by the Flytools. To use it the functions herein, simply include this
 * file in your source.
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

#ifndef __ZCM_HASH_CS_410_H
#define __ZCM_HASH_CS_410_H

#include <stdio.h>

/**
 * Computes the uncompressed hash of the given string, returned as an unsigned
 * integer. This hash can be used for many things, such as for verification or
 * for indices into a dictionary or associative list.
 * @param s the string to hash
 * @return the hash of the string s as an unsigned integer
 */
unsigned int hash_string(const char *s);
/**
 * Compresses a given hash key k over the possible range n. In other words, this
 * function ensures that a hash k is suitable for use as an index into an array
 * of size n.
 * @param k the hash key to compress
 * @param n the range over which to compress the given hash key
 * @return the compressed hash as an unsigned integer
 */
unsigned int compress_hash(const unsigned int k, const unsigned int n);

#endif
