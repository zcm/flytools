#ifndef ENABLE_PCG_SPINLOCKS

#if __GNUC__ || __clang__ || __INTEL_LLVM_COMPILER
#define PCG_SPINLOCK_DECLARE(mutex) int dummy __attribute__((unused))
#elif defined(_MSC_VER)
#define PCG_SPINLOCK_DECLARE(mutex) void dummy(void)
#else
#define PCG_SPINLOCK_DECLARE(mutex)
#endif

#define PCG_SPINLOCK_LOCK(mutex)
#define PCG_SPINLOCK_UNLOCK(mutex)

#define PCG_SPINLOCK_H_INCLUDED 1

#endif  // ENABLE_PCG_SPINLOCKS

#if __GNUC__ || __clang__ || __INTEL_LLVM_COMPILER
#define inline __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define inline __forceinline
#endif

#include "pcg-c/extras/entropy.c"

#ifdef inline
#undef inline
#endif
