#include "tests.h"

#include "random.h"

#if !defined(_WINDLL) && !defined(METHODS_ONLY)
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
void next32_compare(
    uint32_t r, rng32 *before, rng32 *after, uint32_t bound, int expect_equal) {
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

  // Pick a seed such that this doesn't fail (no matter how unlikely).
  assert_int_not_equal(r, r2);

  assert_int_not_equal(before->state, after->state);
  assert_int_equal(before->inc, after->inc);
}

void next64_compare(
    uint64_t r, rng64 *before, rng64 *after, uint64_t bound, int expect_equal) {
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
  assert_true(before->inc == after->inc);

  if (expect_equal) {
    assert_true(before->state == after->state);
  } else {
    assert_false(before->state == after->state);
  }
#else
  assert_int_equal(before->low.inc, after->low.inc);
  assert_int_equal(before->high.inc, after->high.inc);

  if (expect_equal) {
    assert_int_equal(before->low.state, after->low.state);
    assert_int_equal(before->high.state, after->high.state);
  } else {
    assert_int_not_equal(before->low.state, after->low.state);
    assert_int_not_equal(before->high.state, after->high.state);
  }
#endif
}

void do_test_rng32_next(uint8_t mode, uint32_t bound, int bias) {
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

redraw:
  if (!bound) {
    r = rng32_next(&rng);
  } else {
    if (bias) {
      r = rng32_next_in_biased(&rng, bound);
    } else {
      r = rng32_next_in(&rng, bound);
    }

    assert_in_range(r, 0, bound);
  }

  before.state = rng.state;
  before.inc = rng.inc;

  next32_compare(r, &before, &rng, bound, 0);

  if (r == 0) {
    // 0 is a valid value and seems to be generated when state == inc.
    // We want to make sure other values can be generated too, however.
    goto redraw;
  }
}

void do_test_rng64_next(uint8_t mode, uint64_t bound, int bias) {
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

redraw:
  if (!bound) {
    r = rng64_next(&rng);
  } else {
    if (bias) {
      r = rng64_next_in_biased(&rng, bound);
    } else {
      r = rng64_next_in(&rng, bound);
    }

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

  next64_compare(r, &before, &rng, bound, 0);

  if (r == 0) {
    // Same thing as with rng32. Verify we can also generate non-zero values.
    goto redraw;
  }
}
#endif

// Test that you can generate without seeding first.
// Stack garbage is technically a seed - isolate next() from seed() this way.
TESTCALL(test_rng32_next_uninitialized, do_test_rng32_next(DO_NOT_SEED, 0, 0))
TESTCALL(test_rng64_next_uninitialized, do_test_rng64_next(DO_NOT_SEED, 0, 0))

TESTCALL(test_rng32_next_seed, do_test_rng32_next(USE_ENTROPY, 0, 0))
TESTCALL(test_rng64_next_seed, do_test_rng64_next(USE_ENTROPY, 0, 0))

TESTCALL(test_rng32_next_set_seed, do_test_rng32_next(SET_SEED, 0, 0))
TESTCALL(test_rng64_next_set_seed, do_test_rng64_next(SET_SEED, 0, 0))

TESTCALL(test_rng32_next_in, do_test_rng32_next(DO_NOT_SEED, 12345, 0))
TESTCALL(test_rng64_next_in, do_test_rng64_next(DO_NOT_SEED, 12345, 0))

TESTCALL(test_rng32_next_in_biased, do_test_rng32_next(DO_NOT_SEED, 12345, 1))
TESTCALL(test_rng64_next_in_biased, do_test_rng64_next(DO_NOT_SEED, 12345, 1))

#ifndef METHODS_ONLY

#define BIASED_BOUND_32 (UINT32_MAX / 2 + UINT32_MAX / 4 + 999979)
#define BIASED_BOUND_64 (UINT64_MAX / 2 + UINT64_MAX / 4 + 6223707953)

void do_test_rng32_next_in_unbiased_redraw() {
  rng32 u, b;
  uint32_t ru;

  u.state = 1694877417741926800ULL;
  u.inc = 533521648ULL;
  b = u;

  // First two in a row (!!) in this sequence should be redrawn due to bias
  assert_int_not_equal(
      rng32_next_in_biased(&b, BIASED_BOUND_32),
      ru = rng32_next_in(&u, BIASED_BOUND_32));

  // Next biased draw was also redrawn
  assert_int_not_equal(ru, rng32_next_in_biased(&b, BIASED_BOUND_32));

  // The next biased number is the unbiased one we skipped to
  assert_int_equal(ru, rng32_next_in_biased(&b, BIASED_BOUND_32));

  // And then one should be redrawn this time
  assert_int_not_equal(
      rng32_next_in_biased(&b, BIASED_BOUND_32),
      ru = rng32_next_in(&u, BIASED_BOUND_32));

  // The unbiased one we skipped to on the second time around
  assert_int_equal(ru, rng32_next_in_biased(&b, BIASED_BOUND_32));

  // Next is out of range so is not redrawn
  assert_int_equal(
      rng32_next_in_biased(&b, BIASED_BOUND_32),
      rng32_next_in(&u, BIASED_BOUND_32));

  // Just jump to a state that we know will be in range but over the threshold
  u.state = b.state = 8530570146876432800ULL;

  // Finally we draw one within range but not redrawn due to over threshold
  assert_int_equal(
      rng32_next_in_biased(&b, BIASED_BOUND_32),
      rng32_next_in(&u, BIASED_BOUND_32));
}

void do_test_rng64_next_in_unbiased_redraw() {
  rng64 u, b;
  uint64_t ru;

  // These values happen to pass for both the 128-bit and dual 64-bit methods
  u.low.state = 7722718773909012224ULL;
  u.low.inc = 533718128ULL;
  u.high = u.low;
  b = u;

  // First one in this sequence should be redrawn due to bias
  assert_int_not_equal(
      rng64_next_in_biased(&b, BIASED_BOUND_64),
      ru = rng64_next_in(&u, BIASED_BOUND_64));

  // The next biased number is the unbiased one we skipped to
  assert_int_equal(ru, rng64_next_in_biased(&b, BIASED_BOUND_64));

  // Next is within range but not redrawn due to being over the threshold
  assert_int_equal(
      rng64_next_in_biased(&b, BIASED_BOUND_64),
      rng64_next_in(&u, BIASED_BOUND_64));

  // Finally we draw a number out of range and again do not redraw
  assert_int_equal(
      rng64_next_in_biased(&b, BIASED_BOUND_64),
      rng64_next_in(&u, BIASED_BOUND_64));
}

#undef BIASED_BOUND_32
#undef BIASED_BOUND_64

#endif

TESTCALL(
    test_rng32_next_in_unbiased_redraw,
    do_test_rng32_next_in_unbiased_redraw())
TESTCALL(
    test_rng64_next_in_unbiased_redraw,
    do_test_rng64_next_in_unbiased_redraw())

#ifndef METHODS_ONLY

#define INC_UNCHANGED 0
#define INC_CHANGED   1

static void assert_rng32_equal(rng32 expected, rng32 actual) {
  assert_int_equal(expected.state, actual.state);
  assert_int_equal(expected.inc, actual.inc);
}

static void assert_rng64_equal(rng64 expected, rng64 actual) {
#ifdef __SIZEOF_INT128__
  assert_true(expected.state == actual.state);
  assert_true(expected.inc == actual.inc);
#else
  assert_int_equal(expected.low.state, actual.low.state);
  assert_int_equal(expected.high.state, actual.high.state);
  assert_int_equal(expected.low.inc, actual.low.inc);
  assert_int_equal(expected.high.inc, actual.high.inc);
#endif
}

static void assert_rng32_not_equal(
    rng32 expected, rng32 actual, int expect_inc_changed) {
  assert_int_not_equal(expected.state, actual.state);

  if (expect_inc_changed) {
    assert_int_not_equal(expected.inc, actual.inc);
  } else {
    assert_int_equal(expected.inc, actual.inc);
  }
}

static void assert_rng64_not_equal(
    rng64 expected, rng64 actual, int expect_inc_changed) {
#ifdef __SIZEOF_INT128__
  assert_false(expected.state == actual.state);

  if (expect_inc_changed) {
    assert_false(expected.inc == actual.inc);
  } else {
    assert_true(expected.inc == actual.inc);
  }
#else
  assert_int_not_equal(expected.low.state, actual.low.state);
  assert_int_not_equal(expected.high.state, actual.high.state);

  if (expect_inc_changed) {
    assert_int_not_equal(expected.low.inc, actual.low.inc);
    assert_int_not_equal(expected.high.inc, actual.high.inc);
  } else {
    assert_int_equal(expected.low.inc, actual.low.inc);
    assert_int_equal(expected.high.inc, actual.high.inc);
  }
#endif
}

void do_test_rng32_idempotency() {
  uint32_t left, right;
  rng32 before, after;
  union rng_seed32 seed = rng_seed32_make(304, 206);

  rng32_set_seed(&before, seed);
  rng32_set_seed(&after, seed);

  assert_rng32_equal(before, after);

  rng32_set_seed(&after, seed);

  assert_rng32_equal(before, after);

  left = rng32_next(&before);
  right = rng32_next(&after);

  assert_int_equal(left, right);
  assert_rng32_equal(before, after);

  left = rng32_next_in(&before, 12345);
  right = rng32_next_in(&after, 12345);

  assert_int_equal(left, right);
  assert_rng32_equal(before, after);

  right = rng32_next(&after);

  assert_int_not_equal(left, right);
  assert_rng32_not_equal(before, after, INC_UNCHANGED);
}

void do_test_rng64_idempotency() {
  uint64_t left, right;
  rng64 before, after;
  union rng_seed64 seed = rng_seed64_make64(519, 26, 550, 13);

  rng64_set_seed(&before, seed);
  rng64_set_seed(&after, seed);

  assert_rng64_equal(before, after);

  rng64_set_seed(&after, seed);

  assert_rng64_equal(before, after);

  left = rng64_next(&before);
  right = rng64_next(&after);

  assert_int_equal(left, right);
  assert_rng64_equal(before, after);

  left = rng64_next_in(&before, 12345);
  right = rng64_next_in(&after, 12345);

  assert_int_equal(left, right);
  assert_rng64_equal(before, after);

  right = rng64_next(&after);

  assert_int_not_equal(left, right);
  assert_rng64_not_equal(before, after, INC_UNCHANGED);
}
#endif

TESTCALL(test_rng32_idempotency, do_test_rng32_idempotency())
TESTCALL(test_rng64_idempotency, do_test_rng64_idempotency())

#if __STDC_VERSION__ >= 201112L || defined(_MSC_VER)
#ifndef METHODS_ONLY
#define ulll(high, low) (((__uint128_t) high << 64) | (__uint128_t) low)

void do_test_rng() {
  union rng_seed32 seed32 = rng_seed_make(32ull, 32ull);
#ifdef __SIZEOF_INT128__
  union rng_seed64 seed64 = rng_seed_make(ulll(128, 64), ulll(128, 64));
#endif
  union rng_seed64 seed64m = rng_seed64_make64(64, 64, 64, 64);

  rng32 auto32, static32;
  rng64 auto64, auto64m, static64, static64m;

  rng_seed(&auto32);
  static32 = auto32;

  rng_seed(&auto64);
  static64 = auto64;

  assert_int_equal(rng32_next(&static32), rng_next(&auto32));
  assert_int_equal(rng32_next(&static32), rng_next(&auto32));
  assert_int_equal(rng64_next(&static64), rng_next(&auto64));
  assert_int_equal(rng64_next(&static64), rng_next(&auto64));

  rng_set_seed(&auto32, seed32);
  rng32_set_seed(&static32, seed32);
  assert_rng32_equal(static32, auto32);

#ifdef __SIZEOF_INT128__
  rng_set_seed(&auto64, seed64);
  rng64_set_seed(&static64, seed64);
  assert_rng64_equal(static64, auto64);
#endif

  rng_set_seed(&auto64m, seed64m);
  rng64_set_seed(&static64m, seed64m);
  assert_rng64_equal(static64m, auto64m);

  assert_int_equal(rng32_next(&static32), rng_next(&auto32));
  assert_int_equal(rng32_next(&static32), rng_next(&auto32));
#ifdef __SIZEOF_INT128__
  assert_int_equal(rng64_next(&static64), rng_next(&auto64));
  assert_int_equal(rng64_next(&static64), rng_next(&auto64));
#endif
  assert_int_equal(rng64_next(&static64m), rng_next(&auto64m));
  assert_int_equal(rng64_next(&static64m), rng_next(&auto64m));

  uint32_t r32;
  uint64_t r64;

  assert_int_equal(
      r32 = rng32_next_in(&static32, 1024), rng_next_in(&auto32, 1024));
  assert_in_range(r32, 0, 1024);
  assert_int_equal(
      r32 = rng32_next_in(&static32, 1024), rng_next_in(&auto32, 1024));
  assert_in_range(r32, 0, 1024);
#ifdef __SIZEOF_INT128__
  assert_int_equal(
      r64 = rng64_next_in(&static64, 1024), rng_next_in(&auto64, 1024));
  assert_in_range(r64, 0, 1024);
  assert_int_equal(
      r64 = rng64_next_in(&static64, 1024), rng_next_in(&auto64, 1024));
  assert_in_range(r64, 0, 1024);
#endif
  assert_int_equal(
      r64 = rng64_next_in(&static64m, 1024), rng_next_in(&auto64m, 1024));
  assert_in_range(r64, 0, 1024);
  assert_int_equal(
      r64 = rng64_next_in(&static64m, 1024), rng_next_in(&auto64m, 1024));
  assert_in_range(r64, 0, 1024);

  assert_int_equal(
      r32 = rng32_next_in_biased(&static32, 1024),
      rng_next_in_biased(&auto32, 1024));
  assert_in_range(r32, 0, 1024);
  assert_int_equal(
      r32 = rng32_next_in_biased(&static32, 1024),
      rng_next_in_biased(&auto32, 1024));
  assert_in_range(r32, 0, 1024);
#ifdef __SIZEOF_INT128__
  assert_int_equal(
      r64 = rng64_next_in_biased(&static64, 1024),
      rng_next_in_biased(&auto64, 1024));
  assert_in_range(r64, 0, 1024);
  assert_int_equal(
      r64 = rng64_next_in_biased(&static64, 1024),
      rng_next_in_biased(&auto64, 1024));
  assert_in_range(r64, 0, 1024);
#endif
  assert_int_equal(
      r64 = rng64_next_in_biased(&static64m, 1024),
      rng_next_in_biased(&auto64m, 1024));
  assert_in_range(r64, 0, 1024);
  assert_int_equal(
      r64 = rng64_next_in_biased(&static64m, 1024),
      rng_next_in_biased(&auto64m, 1024));
  assert_in_range(r64, 0, 1024);
}

#undef ulll
#endif

TESTCALL(test_rng, do_test_rng())

#endif  // __STDC_VERSION__ >= 201112L

#undef DO_NOT_SEED
#undef USE_ENTROPY
#undef SET_SEED

#undef INC_UNCHANGED
#undef INC_CHANGED

#undef MAX_GEN_ATTEMPTS

#ifndef _WINDLL
#ifndef METHODS_ONLY
#define METHODS_ONLY
#undef TEST
#define TEST(name, def) cmocka_unit_test(name),
int main(void) {
  const struct CMUnitTest tests[] = {
#include "test_random.c"
  };

  return cmocka_run_group_tests_name(
      "flytools random", tests, random_setup, random_teardown);
}
#endif  // METHODS_ONLY
#endif
