#include "random.h"

#include "fastrange.h"

#ifdef __GNUC__
#define inline __attribute__((always_inline)) inline
#endif

#include "pcg_variants.h"
#include "extras/entropy.h"

#ifdef inline
#undef inline
#endif

FLYAPI void rng32_seed(rng32 *rng) {
  union rng_seed32 seed;
  entropy_getbytes((void *) &seed, sizeof (union rng_seed32));
  rng32_set_seed(rng, seed);
}

FLYAPI union rng_seed32 rng_seed32_make(uint64_t state, uint64_t seq) {
  union rng_seed32 seed;

  seed.init.state = state;
  seed.init.seq = seq;

  return seed;
}

FLYAPI void rng32_set_seed(rng32 *rng, union rng_seed32 seed) {
  pcg32_srandom_r((pcg32_random_t *) rng, seed.init.state, seed.init.seq);
}

FLYAPI uint32_t rng32_next(rng32 *rng) {
  return pcg32_random_r((pcg32_random_t *) rng);
}

FLYAPI uint32_t rng32_next_in_biased(rng32 *rng, uint32_t bound) {
  return fastrange32(rng32_next(rng), bound);
}

FLYAPI void rng64_seed(rng64 *rng) {
  union rng_seed64 seed;
  entropy_getbytes((void *) &seed, sizeof (union rng_seed64));
  rng64_set_seed(rng, seed);
}

FLYAPI union rng_seed64 rng_seed64_make64(
    uint64_t low_state, uint64_t high_state,
    uint64_t low_seq, uint64_t high_seq) {
  union rng_seed64 seed;

  seed.init64.low.state = low_state;
  seed.init64.high.state = high_state;
  seed.init64.low.seq = low_seq;
  seed.init64.high.seq = high_seq;

  return seed;
}

#ifdef __SIZEOF_INT128__

FLYAPI union rng_seed64 rng_seed64_make(
    __uint128_t state, __uint128_t seq) {
  union rng_seed64 seed;

  seed.init.state = state;
  seed.init.seq = seq;

  return seed;
}

FLYAPI void rng64_set_seed(rng64 *rng, union rng_seed64 seed) {
  pcg64_srandom_r((pcg64_random_t *) rng, seed.init.state, seed.init.seq);
}

FLYAPI uint64_t rng64_next(rng64 *rng) {
  return pcg64_random_r((pcg64_random_t *) rng);
}

#else

FLYAPI void rng64_set_seed(rng64 *rng, union rng_seed64 seed) {
  rng32_set_seed(&rng->low, seed.rng32.low);
  rng32_set_seed(&rng->high, seed.rng32.high);
}

FLYAPI uint64_t rng64_next(rng64 *rng) {
  return ((uint64_t) pcg32_random_r((pcg32_random_t *) &rng->low)) << 32
       | ((uint64_t) pcg32_random_r((pcg32_random_t *) &rng->high));
}

#endif

FLYAPI uint64_t rng64_next_in_biased(rng64 *rng, uint64_t bound) {
  return fastrange64(rng64_next(rng), bound);
}

FLYAPI uint32_t rng32_next_in(rng32 *rng, uint32_t bound) {
  return fastrange32_unbiased(rng32_next(rng), bound, rng);
}

FLYAPI uint64_t rng64_next_in(rng64 *rng, uint64_t bound) {
  return fastrange64_unbiased(rng64_next(rng), bound, rng);
}

