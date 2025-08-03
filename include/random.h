#ifndef ZCM_RANDOM_H
#define ZCM_RANDOM_H

#include <inttypes.h>

#include "common.h"

typedef struct rng32 {
  uint64_t state;
  uint64_t inc;
} rng32;

typedef struct rng64 {
#ifdef __SIZEOF_INT128__
  __uint128_t state;
  __uint128_t inc;
#else
  struct rng32 low;
  struct rng32 high;
#endif
} rng64;

struct rng_init32 {
  uint32_t state;
  uint32_t seq;
};

struct rng_init64 {
  uint64_t state;
  uint64_t seq;
};

#ifdef __SIZEOF_INT128__
struct rng_init128 {
  __uint128_t state;
  __uint128_t seq;
};
#endif

struct split_rng_init64 {
  struct rng_init32 low;
  struct rng_init32 high;
};

struct split_rng_init128 {
  struct rng_init64 low;
  struct rng_init64 high;
};

union rng_seed32 {
  uint8_t bytes[16];
  struct rng_init64 init;
  struct split_rng_init64 init32;
};

struct split_rng_seed64 {
  union rng_seed32 low;
  union rng_seed32 high;
};

union rng_seed64 {
  uint8_t bytes[32];
#ifdef __SIZEOF_INT128__
  struct rng_init128 init;
#endif
  struct split_rng_init128 init64;
  struct split_rng_seed64 rng32;
};

FLYAPI void rng32_seed(rng32 *rng);
FLYAPI void rng32_set_seed(rng32 *rng, union rng_seed32 seed);
FLYAPI uint32_t rng32_next(rng32 *rng);
FLYAPI uint32_t rng32_next_in(rng32 *rng, uint32_t bound);
FLYAPI uint32_t rng32_next_in_biased(rng32 *rng, uint32_t bound);

FLYAPI void rng64_seed(rng64 *rng);
FLYAPI void rng64_set_seed(rng64 *rng, union rng_seed64 seed);
FLYAPI uint64_t rng64_next(rng64 *rng);
FLYAPI uint64_t rng64_next_in(rng64 *rng, uint64_t bound);
FLYAPI uint64_t rng64_next_in_biased(rng64 *rng, uint64_t bound);

FLYAPI union rng_seed32 rng_seed32_make(uint64_t state, uint64_t seq);
#ifdef __SIZEOF_INT128__
FLYAPI union rng_seed64 rng_seed64_make(__uint128_t state, __uint128_t seq);
#endif

FLYAPI union rng_seed64 rng_seed64_make64(
    uint64_t low_state, uint64_t high_state,
    uint64_t low_seq, uint64_t high_seq);

#endif
