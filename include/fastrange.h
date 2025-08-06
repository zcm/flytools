/**
* Fair maps to intervals without division.
* Reference : http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
*
* (c) Daniel Lemire
* Modifications (c) Zachary Murray
* Apache License 2.0
*/
#ifndef ZCM_UNBIASED_FASTRANGE_H
#define ZCM_UNBIASED_FASTRANGE_H

#ifndef fastrangemax
#if (SIZE_MAX == UINT32_MAX)
#define fastrangemax fastrange32
#else
#define fastrangemax fastrange64
#endif
#endif

// If another fastrange has been included elsewhere, we'll use it instead.
#ifndef INCLUDE_FASTRANGE_H
#define INCLUDE_FASTRANGE_H

#include <iso646.h> // mostly for Microsoft compilers
#include <stdint.h> // part of Visual Studio 2010 and better
#include <stddef.h> // for size_t in C
#include <limits.h> // for size_t in C

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4146)

#ifdef _WIN64
#include <intrin.h>// should be part of all recent Visual Studio
#pragma intrinsic(_umul128)
#endif  // _WIN64
#endif  // _MSC_VER

#include "common.h"
#include "jargon.h"

#ifndef UINT32_MAX
#define UINT32_MAX  (0xffffffff)
#endif // UINT32_MAX

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
FLYAPI inline uint32_t fastrange32(uint32_t word, uint32_t p) {
	return (uint32_t) (((uint64_t) word * (uint64_t) p) >> 32);
}

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
FLYAPI inline uint64_t fastrange64(uint64_t word, uint64_t p) {
#ifdef __SIZEOF_INT128__ // then we know we have a 128-bit int
	return (uint64_t) (((__uint128_t) word * (__uint128_t) p) >> 64);
#elif defined(_MSC_VER) && defined(_WIN64)
	// supported in Visual Studio 2005 and better
	uint64_t highProduct;
	_umul128(word, p, &highProduct); // ignore output
	return highProduct;
#else
	return word % p; // fallback
#endif // __SIZEOF_INT128__
}

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
__attribute__((artificial))
FLYAPI inline size_t fastrangesize(size_t word, size_t p) {
	return (size_t) fastrangemax(word, p);
}

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
__attribute__((artificial))
FLYAPI inline int fastrangeint(int word, int p) {
	return (int) fastrangemax(word, p);
}

#endif  // INCLUDE_FASTRANGE_H

__attribute__((callback (3, 1)))
FLYAPI inline uint32_t fastrange32_unbiased(
    void *rng, const uint32_t p, uint32_t (*rand_next)(void *)) {
  uint32_t word = rand_next(rng);
  uint64_t product = (uint64_t) word * (uint64_t) p;
  uint32_t threshold, leftover = (uint32_t) product;

  if (leftover < p) {
    threshold = -p % p;

    while (leftover < threshold) {
      word = rand_next(rng);
      product = (uint64_t) word * (uint64_t) p;
      leftover = (uint32_t) product;
    }
  }

  return product >> 32;
}

__attribute__((callback (3, 1)))
FLYAPI inline uint64_t fastrange64_unbiased(
    void *rng, const uint64_t p, uint64_t (*rand_next)(void *)) {
  uint64_t threshold, low, word = rand_next(rng);

#ifdef __SIZEOF_INT128__
  __uint128_t product = (__uint128_t) word * (__uint128_t) p;
  low = (uint64_t) product;

  if (low < p) {
    threshold = -p % p;

    while (low < threshold) {
      word = rand_next(rng);
      product = (__uint128_t) word * (__uint128_t) p;
      low = (uint64_t) product;
    }
  }

  return product >> 64;
#elif defined(_MSC_VER) && defined(_WIN64)
  uint64_t high;
  low = _umul128(word, p, &high);

  if (low < p) {
    threshold = -p % p;

    while (low < threshold) {
      word = rand_next(rng);
      low = _umul128(word, p, &high);
    }
  }

  return high;
#else
  low = word * p;

  if (low < p) {
    threshold = -p % p;

    while (low < threshold) {
      word = rand_next(rng);
      low = word * p;
    }
  }

  return word % p;
#endif
}

#undef fastrangemax

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "unjargon.h"

#endif  // ZCM_UNBIASED_FASTRANGE_H
