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

FLYAPI void rng32_seed(struct rng32 *rng) {
  union rng_seed32 seed;
  entropy_getbytes((void *) &seed, sizeof (union rng_seed32));
  rng32_set_seed(rng, seed);
}

FLYAPI void rng32_set_seed(struct rng32 *rng, union rng_seed32 seed) {
  pcg32_srandom_r((pcg32_random_t *) rng, seed.init.state, seed.init.seq);
}

FLYAPI uint32_t rng32_next(struct rng32 *rng) {
  return pcg32_random_r((pcg32_random_t *) rng);
}

FLYAPI uint32_t rng32_next_in(struct rng32 *rng, uint32_t bound) {
  return fastrange32(rng32_next(rng), bound);
}

FLYAPI void rng64_seed(struct rng64 *rng) {
  union rng_seed64 seed;
  entropy_getbytes((void *) &seed, sizeof (union rng_seed64));
  rng64_set_seed(rng, seed);
}

#ifdef __SIZEOF_INT128__

FLYAPI void rng64_set_seed(struct rng64 *rng, union rng_seed64 seed) {
  pcg64_srandom_r((pcg64_random_t *) rng, seed.init.state, seed.init.seq);
}

FLYAPI uint64_t rng64_next(struct rng64 *rng) {
  return pcg64_random_r((pcg64_random_t *) rng);
}

#else

FLYAPI void rng64_set_seed(struct rng64 *rng, union rng_seed64 seed) {
  rng32_set_seed(&rng->low, seed.rng32.low);
  rng32_set_seed(&rng->high, seed.rng32.high);
}

FLYAPI uint64_t rng64_next(struct rng64 *rng) {
  return ((uint64_t) pcg32_random_r((pcg32_random_t *) &rng->low)) << 32
       | ((uint64_t) pcg32_random_r((pcg32_random_t *) &rng->high));
}

#endif

FLYAPI uint64_t rng64_next_in(struct rng64 *rng, uint64_t bound) {
  return fastrange64(rng64_next(rng), bound);
}
