#include "fastrange.h"

extern inline uint32_t fastrange32(uint32_t word, uint32_t p);
extern inline uint64_t fastrange64(uint64_t word, uint64_t p);
extern inline size_t fastrangesize(size_t word, size_t p);
extern inline int fastrangeint(int word, int p);

extern inline uint32_t fastrange32_unbiased(
    void *rng, const uint32_t p, uint32_t (*rand_next)(void *));
extern inline uint64_t fastrange64_unbiased(
    void *rng, const uint64_t p, uint64_t (*rand_next)(void *));
