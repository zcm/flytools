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
#include "fugue.h"

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

#define hash_macro_v(constant, itr, itr_body, terminal_case, body) \
    register unsigned int itr = 0; \
    register unsigned int ret = constant; \
    while( terminal_case ) { \
        ret ^= (unsigned int)s[itr]+itr; \
        ret = _rotr(ret, 1); \
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

#define FUGUE_OUT_SIZE 80

unsigned int fugue_hash_wrapper(const char *s, const size_t limit) {
    const size_t uioutsize = FUGUE_OUT_SIZE*sizeof(char)/sizeof(unsigned int);
    unsigned char out[FUGUE_OUT_SIZE] = {0};
    int i = 1;
    unsigned int ret = 0, *u_out;
    fugue_hash(out, (unsigned char *)s, (unsigned long long)limit);
    u_out = (unsigned int *)out;
    ret = *u_out;
    for(i = 1; i < uioutsize; i++) {
        ret ^= u_out[i];
    }
    return ret;
}

#undef FUGUE_OUT_SIZE

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
#undef _rotr
#endif
