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

extern inline uint32_t rng32_next(rng32 *rng);
extern inline uint32_t rng32_next_thunk(void *rng);
extern inline uint32_t rng32_next_in(rng32 *rng, uint32_t bound);
extern inline uint32_t rng32_next_in_biased(rng32 *rng, uint32_t bound);

extern inline uint64_t rng64_next(rng64 *rng);
extern inline uint64_t rng64_next_thunk(void *rng);
extern inline uint64_t rng64_next_in(rng64 *rng, uint64_t bound);
extern inline uint64_t rng64_next_in_biased(rng64 *rng, uint64_t bound);
