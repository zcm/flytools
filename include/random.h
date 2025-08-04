#ifndef ZCM_RANDOM_H
#define ZCM_RANDOM_H

#include <inttypes.h>

#include "common.h"
#include "fastrange.h"
#include "pcg_variants.h"

typedef struct rng32 {
  uint64_t state;
  uint64_t inc;
} rng32;

typedef struct rng64 {
#ifdef __SIZEOF_INT128__
  union {
    struct {
      __uint128_t state;
      __uint128_t inc;
    };
    struct {
#endif
      struct rng32 low;
      struct rng32 high;
#ifdef __SIZEOF_INT128__
    };
  };
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
FLYAPI void rng64_seed(rng64 *rng);

static inline void rng32_set_seed(rng32 *rng, union rng_seed32 seed) {
  pcg32_srandom_r((pcg32_random_t *) rng, seed.init.state, seed.init.seq);
}

static inline uint32_t rng32_next(rng32 *rng) {
  return pcg32_random_r((pcg32_random_t *) rng);
}

static inline uint32_t rng32_next_thunk(void *rng) {
  return rng32_next((rng32 *) rng);
}

static inline uint32_t rng32_next_in(rng32 *rng, uint32_t bound) {
  return fastrange32_unbiased(rng32_next(rng), bound, &rng32_next_thunk, rng);
}

static inline uint32_t rng32_next_in_biased(rng32 *rng, uint32_t bound) {
  return fastrange32(rng32_next(rng), bound);
}

static inline void rng64_set_seed(rng64 *rng, union rng_seed64 seed) {
#ifdef __SIZEOF_INT128__
  pcg64_srandom_r((pcg64_random_t *) rng, seed.init.state, seed.init.seq);
#else
  rng32_set_seed(&rng->low, seed.rng32.low);
  rng32_set_seed(&rng->high, seed.rng32.high);
#endif
}

static inline uint64_t rng64_next(rng64 *rng) {
#ifdef __SIZEOF_INT128__
  return pcg64_random_r((pcg64_random_t *) rng);
#else
  return ((uint64_t) pcg32_random_r((pcg32_random_t *) &rng->low)) << 32
       | ((uint64_t) pcg32_random_r((pcg32_random_t *) &rng->high));
#endif
}

static inline uint64_t rng64_next_thunk(void *rng) {
  return rng64_next((rng64 *) rng);
}

static inline uint64_t rng64_next_in(rng64 *rng, uint64_t bound) {
  return fastrange64_unbiased(rng64_next(rng), bound, &rng64_next_thunk, rng);
}

static inline uint64_t rng64_next_in_biased(rng64 *rng, uint64_t bound) {
  return fastrange64(rng64_next(rng), bound);
}

static inline union rng_seed32
rng_seed32_make(uint64_t state, uint64_t seq) {
  union rng_seed32 seed;

  seed.init.state = state;
  seed.init.seq = seq;

  return seed;
}

#ifdef __SIZEOF_INT128__
static inline union rng_seed64
rng_seed64_make(__uint128_t state, __uint128_t seq) {
  union rng_seed64 seed;

  seed.init.state = state;
  seed.init.seq = seq;

  return seed;
}
#endif

static inline union rng_seed64
rng_seed64_make64(uint64_t low_state, uint64_t high_state,
                  uint64_t low_seq, uint64_t high_seq) {
  union rng_seed64 seed;

  seed.init64.low.state = low_state;
  seed.init64.high.state = high_state;
  seed.init64.low.seq = low_seq;
  seed.init64.high.seq = high_seq;

  return seed;
}

#endif
