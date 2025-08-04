#include "random.h"
#include "entropy.h"

FLYAPI void rng32_seed(rng32 *rng) {
  union rng_seed32 seed;
  entropy_getbytes((void *) &seed, sizeof (union rng_seed32));
  rng32_set_seed(rng, seed);
}

FLYAPI void rng64_seed(rng64 *rng) {
  union rng_seed64 seed;
  entropy_getbytes((void *) &seed, sizeof (union rng_seed64));
  rng64_set_seed(rng, seed);
}

