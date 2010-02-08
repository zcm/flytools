/** @file hash.c
 * This is the hash function used in the dictionary type provided by the
 * Flytools. It is used when determining index values for key/value pairs in the
 * dictionary. It is possible to use this hash function to get unsigned integer
 * hashes of char * strings as well.
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

#include "hash.h"

// Turbo C doesn't like "static inline."
// In light of this, if we're using Turbo C, we change this to a macro.
// It should give us the same runtime efficiency.
#ifdef __TURBOC__
#define _rotr1(value) (((value) >> 1) | ((value) << 31))
#else
#ifdef __STRICT_ANSI__
#define inline
#endif
static inline unsigned int _rotr1(unsigned int value) {
#if defined(__GNUC__) && !defined(__STRICT_ANSI__) && !defined(NO_ASM)
    asm ("rorl $1,%1" : "=a" (value) : "0" (value));
    return value;
#else
    return (value >> 1) | (value << 31);
#endif
#ifdef __STRICT_ANSI__
#undef inline
#endif
}
#endif

#define hash_macro_v(constant, itr, itr_body, terminal_case, body) \
    register unsigned int itr = 0; \
    register unsigned int ret = constant; \
    while( terminal_case ) { \
        ret ^= (unsigned int)s[itr]+itr; \
        ret = _rotr1(ret); \
        itr_body \
        i++; \
    } \
    body \
    return ret

#define hash_macro_c(constant, itr, terminal_case) \
    hash_macro_v(constant, itr, , terminal_case, )

#define hash_macro(itr, terminal_case) \
    hash_macro_c(0xFAFAFAFA, itr, terminal_case)

unsigned int blind_bounded_hash_string(const char *s, const size_t limit) {
    hash_macro(i, i < limit);
}

unsigned int hash_nstring(const char *s, const size_t limit) {
    hash_macro(i, s[i] && i < limit);
}

unsigned int hash_string(const char *s) {
    hash_macro(i, s[i]);
}

#undef hash_macro
#undef hash_macro_c
#undef hash_macro_v

unsigned int
hash_pointer_using(const void *ptr,
                   unsigned int (*hashfn)(const char *, const size_t)) {
    return (*hashfn)((char *)&ptr, sizeof(void *)/sizeof(char));
}

unsigned int hash_pointer(const void *ptr) {
    return hash_pointer_using(ptr, &blind_bounded_hash_string);
}

unsigned int compress_hash(const unsigned int k, const unsigned int n) {
    return k % n;
}

#ifdef __TURBOC__
#undef _rotr1
#endif
