#ifndef ENABLE_PCG_SPINLOCKS

#define PCG_SPINLOCK_DECLARE(mutex)
#define PCG_SPINLOCK_LOCK(mutex)
#define PCG_SPINLOCK_UNLOCK(mutex)

#define PCG_SPINLOCK_H_INCLUDED 1

#endif  // ENABLE_PCG_SPINLOCKS

#ifdef __GNUC__
#define inline __attribute__((always_inline)) inline
#endif

#include "pcg-c/extras/entropy.c"

#ifdef inline
#undef inline
#endif
