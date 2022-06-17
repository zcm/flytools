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

#include <stddef.h>
#include <stdint.h>

#include "common.h"

/**
 * Computes a hash using xorshift64*. The implementation uses the variant
 * A_1(12,25,27), multiplier M32.
 *
 * @param x the integer to hash
 * @return the hash of the integer
 */
FLYAPI uint64_t hash_xorshift64s(uint64_t x);

/**
 * Hashes a pointer using xorshift64*. The implementation uses the variant
 * A_1(12,25,27), multiplier M32 and applies a rotation 16 bits to the right to
 * avoid skewing hash distribution when using short bitmasks due to bits that
 * are usually zero (e.g. 48-bit x86-64 addresses) or byte alignment patterns.
 *
 * @param ptr the pointer to hash
 * @return the hash of the pointer
 */
FLYAPI uintptr_t hash_xorshift64s_ptr(uintptr_t ptr);

/**
 * Computes the uncompressed hash of the given string, ignoring null characters,
 * continuing until @c limit characters have been hashed. This function should
 * generally be avoided due to its power of ignoring null characters; it is used
 * internally for some magic regarding pointers. Only use this function if you
 * have a good reason to ignore the null character check.
 * @param s the string to hash
 * @param limit the number of characters to hash
 * @return the hash of the string s as an unsigned integer
 */
FLYAPI size_t blind_bounded_hash_string(
    const char *s,
    const size_t limit);
/**
 * Computes the uncompressed hash of the given string. The hash continues
 * hashing characters one at a time until either a null character is encountered
 * or @c limit characters have been hashed. In general, this is the best
 * function to use when dealing with strings.
 * @param s the string to hash
 * @param limit the maximum number of characters to hash
 * @return the hash of the string s as an unsigned integer
 */
FLYAPI size_t hash_nstring(const char *s, const size_t limit);
/**
 * Computes the uncompressed hash of the given string, returned as an unsigned
 * integer. This hash can be used for many things, such as for verification or
 * for indices into a dictionary or associative list. In general, it is
 * preferrable to bound your message's length; to do this, please see
 * hash_nstring().
 * @param s the string to hash
 * @return the hash of the string s as an unsigned integer
 * @see hash_nstring()
 */
FLYAPI size_t hash_string(const char *s);
/**
 * Bootstraps a pointer and uses a function that is intended to hash a string to
 * instead hash the given pointer. The bootstrapper then returns the hash as an
 * unsigned integer.
 * @param ptr the pointer to hash
 * @param hashfn the callback of the hash function with which to process the
 * pointer
 * @return the hash of the pointer as an unsigned integer
 */
FLYAPI size_t hash_pointer_using(
    const void *ptr,
    size_t (*hashfn)(const char *, const size_t));
/**
 * Computes the uncompressed hash of the given pointer. Pointer hashing runs in
 * constant time.
 * @param ptr the pointer to hash
 * @return the hash of the pointer ptr as an unsigned integer
 */
FLYAPI unsigned int hash_pointer(const void *ptr);

#endif
