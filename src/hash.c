/** @file hash.c
 * This is the hash function used in the dictionary type provided by the Flytools. It is used
 * when determining index values for key/value pairs in the dictionary. It is possible to use
 * this hash function to get unsigned integer hashes of char * strings as well.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009 Zachary Murray,
 * all rights reserved. This source file may not be redistributed, in whole or in part. By
 * viewing and/or using this file, you agree not to redistribute the source files without the
 * express written permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 */

#include "hash.h"

// Turbo C doesn't like "static inline."
// In light of this, if we're using Turbo C, we change this to a macro.
// It should give us the same runtime efficiency.
#ifdef __TURBOC__
#define _rotr(value, shift) \
	(((value) >> ((shift) & 31)) | ((value) << (32 - ((shift) & 31))))
#else
static inline unsigned int _rotr(unsigned int value, int shift) {
	shift &= 31;
	return (value >> shift) | (value << (32 - shift));
}
#endif

unsigned int hash_string(const char *s) {
	register unsigned int i = 0;
	register unsigned int ret = 0xFAFAFAFA;
	while(s[i]) {
		ret ^= (unsigned int)s[i]+i;
		ret = _rotr(ret, 1);
		i++;
	}
	return ret;
}

unsigned int compress_hash(const unsigned int k, const unsigned int n) {
	return k % n;
}

#ifdef __TURBOC__
#undef _rotr
#endif
