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

#ifdef __TURBOC__
#define ror(value, width) \
  (((value) >> (width)) | ((value) << (32 - width)))
#elif defined(_MSC_VER)
#include <intrin.h>
#define ror(value, width) _rotr64((value), (width))
#else
#ifdef __has_include
#if __has_include(<x86intrin.h>)
#include <x86intrin.h>
#define ror(value, width) __rorq((value), (width))
#endif
#endif
#endif

#ifndef ror
#warning "Using default definition for ror() macro; assuming 64-bit shiftwidth"
#define ror(value, width) \
  (((value) >> (width)) | ((value) << (64 - width)))
#endif

#define _xorshift64s_variant(x, a, b, c, m) \
  (x ^= x >> a, \
   x ^= x << b, \
   x ^= x << c, \
   x * m)

#ifdef __TURBOC__
#define M32 0x2545F4914F6CDD1D
#else
#define M32 0x2545F4914F6CDD1DULL
#endif

FLYAPI uint64_t hash_xorshift64s(uint64_t x) {
  /* Variant A_1(12,25,27), multiplier M32 */
  return _xorshift64s_variant(x, 12, 25, 27, M32);
}

FLYAPI uintptr_t hash_xorshift64s_ptr(uintptr_t ptr) {
#ifdef IS32BIT
  uint64_t x = ror(hash_xorshift64s((uint64_t) ptr), 4);
  x ^= x >> 32;
  return (uintptr_t) x;
#else
  ptr = _xorshift64s_variant(ptr, 12, 25, 27, M32);
  return ror(ptr, 4);
#endif
}

#undef _xorshift64s_variant
#undef M32

#define hash_macro_v(constant, itr, itr_body, terminal_case, body) \
    register size_t itr = 0; \
    register size_t ret = constant; \
    while( terminal_case ) { \
        ret ^= (size_t) s[itr] + itr; \
        ret = ror(ret, 1); \
        itr_body \
        i++; \
    } \
    body \
    return ret

#define hash_macro_c(constant, itr, terminal_case) \
    hash_macro_v(constant, itr, , terminal_case, )

#define hash_macro(itr, terminal_case) \
    hash_macro_c(0xFAFAFAFA, itr, terminal_case)

FLYAPI size_t blind_bounded_hash_string(
    const char *s,
    const size_t limit) {
  hash_macro(i, i < limit);
}

FLYAPI size_t hash_nstring(const char *s, const size_t limit) {
  hash_macro(i, s[i] && i < limit);
}

FLYAPI size_t hash_string(const char *s) {
  hash_macro(i, s[i]);
}

#undef hash_macro
#undef hash_macro_c
#undef hash_macro_v

#undef ror

FLYAPI size_t hash_pointer_using(
    const void *ptr,
    size_t (*hashfn)(const char *, const size_t)) {
  return (*hashfn)((char *)&ptr, sizeof(void *)/sizeof(char));
}

FLYAPI unsigned int hash_pointer(const void *ptr) {
  return hash_pointer_using(ptr, &blind_bounded_hash_string);
}

