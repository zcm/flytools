#include "tests.h"

#include "random.h"

#ifndef _WINDLL
int random_setup(void **state) {
  (void) state;

  return 0;
}

int random_teardown(void **state) {
  (void) state;

  return 0;
}
#endif

#define MAX_GEN_ATTEMPTS 100

#define DO_NOT_SEED 0
#define USE_ENTROPY 1
#define SET_SEED 2

#ifndef METHODS_ONLY
void next32_compare(uint32_t r, rng32 *before, rng32 *after, uint32_t bound) {
  uint32_t r2;
  uint8_t i = 0;

  do {
    if (!bound) {
      r2 = rng32_next(after);
    } else {
      r2 = rng32_next_in(after, bound);
    }
  } while(r == r2 && ++i < MAX_GEN_ATTEMPTS);

  if (bound) {
    assert_in_range(r2, 0, bound);
  }

  // This is hypothetically possible to fail even in working code but in
  // practice is astronomically improbable.
  assert_int_not_equal(r, r2);

  assert_int_not_equal(before->state, after->state);
  assert_int_equal(before->inc, after->inc);
}

void next64_compare(uint64_t r, rng64 *before, rng64 *after, uint64_t bound) {
  uint64_t r2;
  uint8_t i = 0;

  do {
    if (!bound) {
      r2 = rng64_next(after);
    } else {
      r2 = rng64_next_in(after, bound);
    }
  } while(r == r2 && ++i < MAX_GEN_ATTEMPTS);

  assert_int_not_equal(r, r2);

#ifdef __SIZEOF_INT128__
  assert_true(before->state != after->state);
  assert_true(before->inc == after->inc);
#else
  assert_int_not_equal(before->low.state, after->low.state);
  assert_int_equal(before->low.inc, after->low.inc);
  assert_int_not_equal(before->high.state, after->high.state);
  assert_int_equal(before->high.inc, after->high.inc);
#endif
}

void do_test_rng32_next(uint8_t mode, uint32_t bound) {
  rng32 rng, before;
  uint32_t r;
  union rng_seed32 seed;

  switch (mode) {
    case DO_NOT_SEED:
      // AddressSanitizer leaves zeroes on the stack, so use address instead
      rng.state = rng.inc = (uint64_t) (uintptr_t) &rng;
      break;
    case USE_ENTROPY:
      rng32_seed(&rng);
      break;
    case SET_SEED:
      seed.init.state = 1;  // doesn't matter as long as it's static
      seed.init.seq = 1;
      rng32_set_seed(&rng, seed);
      break;
    default:
      _fail(__FILE__, __LINE__);
      break;
  }

  if (!bound) {
    r = rng32_next(&rng);
  } else {
    r = rng32_next_in(&rng, bound);

    assert_in_range(r, 0, bound);
  }

  before.state = rng.state;
  before.inc = rng.inc;

  next32_compare(r, &before, &rng, bound);
}

void do_test_rng64_next(uint8_t mode, uint64_t bound) {
  rng64 rng, before;
  uint64_t r;
  union rng_seed64 seed;

  switch (mode) {
    case DO_NOT_SEED:
#ifdef __SIZEOF_INT128__
      rng.state = rng.inc = (__uint128_t) (uintptr_t) &rng;
#else
      rng.low.state = rng.low.inc = (uint64_t) (uintptr_t) &rng;
      rng.high = rng.low;
#endif
      break;
    case USE_ENTROPY:
      rng64_seed(&rng);
      break;
    case SET_SEED:
#ifdef __SIZEOF_INT128__
      seed.init.state = 1;
      seed.init.seq = 1;
#else
      seed.rng32.low.init.state = 1;
      seed.rng32.low.init.seq = 1;
      seed.rng32.high.init.state = 1;
      seed.rng32.high.init.seq = 1;
#endif
      rng64_set_seed(&rng, seed);
      break;
    default:
      _fail(__FILE__, __LINE__);
      break;
  }

  if (!bound) {
    r = rng64_next(&rng);
  } else {
    r = rng64_next_in(&rng, bound);

    assert_in_range(r, 0, bound);
  }

#ifdef __SIZEOF_INT128__
  before.state = rng.state;
  before.inc = rng.inc;
#else
  before.low.state = rng.low.state;
  before.low.inc = rng.low.inc;
  before.high.state = rng.high.state;
  before.high.inc = rng.high.inc;
#endif

  next64_compare(r, &before, &rng, bound);
}
#endif

// Test that you can generate without seeding first.
// Stack garbage is technically a seed - isolate next() from seed() this way.
TESTCALL(test_rng32_next_uninitialized, do_test_rng32_next(DO_NOT_SEED, 0))
TESTCALL(test_rng64_next_uninitialized, do_test_rng64_next(DO_NOT_SEED, 0))

TESTCALL(test_rng32_next_seed, do_test_rng32_next(USE_ENTROPY, 0))
TESTCALL(test_rng64_next_seed, do_test_rng64_next(USE_ENTROPY, 0))

TESTCALL(test_rng32_next_set_seed, do_test_rng32_next(SET_SEED, 0))
TESTCALL(test_rng64_next_set_seed, do_test_rng64_next(SET_SEED, 0))

TESTCALL(test_rng32_next_in, do_test_rng32_next(DO_NOT_SEED, 12345))
TESTCALL(test_rng64_next_in, do_test_rng64_next(DO_NOT_SEED, 12345))

#undef DO_NOT_SEED
#undef USE_ENTROPY
#undef SET_SEED

#undef MAX_GEN_ATTEMPTS

#ifndef _WINDLL
int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_rng32_next_uninitialized),
    cmocka_unit_test(test_rng64_next_uninitialized),
    cmocka_unit_test(test_rng32_next_seed),
    cmocka_unit_test(test_rng64_next_seed),
    cmocka_unit_test(test_rng32_next_set_seed),
    cmocka_unit_test(test_rng64_next_set_seed),
    cmocka_unit_test(test_rng32_next_in),
    cmocka_unit_test(test_rng64_next_in),
  };

  return cmocka_run_group_tests_name(
      "flytools random", tests, random_setup, random_teardown);
}
#endif
