#include "tests.h"

#include "hash.h"


#if !defined(_WINDLL) && !defined(METHODS_ONLY)
int hash_setup(void **state) {
  (void) state;

  return 0;
}

int hash_teardown(void **state) {
  (void) state;

  return 0;
}
#endif

#ifndef METHODS_ONLY
#define ANSWERS 26
#define INPUT 0
#define EXPECTED 1

void do_test_hash_xorshift64s() {
  static uint64_t answers[ANSWERS][2] = {
    {0,            0x0000000000000000ULL},
    {1,            0xA4303534716CDD1DULL},
    {2,            0x48606A68E2D9BA3AULL},
    {3,            0xEC909F9D54469757ULL},
    {4,            0x90C0D4D1C5B37474ULL},
    {5,            0x1FFA3C3467205191ULL},
    {10,           0x3FF47868CE40A322ULL},
    {16,           0x4303534716CDD1D0ULL},
    {31,           0x4F16D09D0C2EC683ULL},
    {32,           0x8606A68E2D9BA3A0ULL},
    {33,           0x2A36DBC29F0880BDULL},
    {63,           0x2D67089CB9CA6A23ULL},
    {64,           0x0C0D4D1C5B374740ULL},
    {65,           0xB03D8250CCA4245DULL},
    {UINT64_MAX-1, 0x2EC03534716CDD1DULL},
    {UINT64_MAX,   0x2E30000000000000ULL},
  };

  for (int i = 0; i < ANSWERS; ++i) {
    assert_int_equal(answers[i][EXPECTED], hash_xorshift64s(answers[i][INPUT]));
  }
}

void do_test_hash_xorshift64s_ptr() {
  static uintptr_t answers[ANSWERS][4] = {
    {0,            0x0000000000000000ULL,  0,  0},
    {1,            0xDA4303534716CDD1ULL,  1, 17},
    {2,            0xA48606A68E2D9BA3ULL,  3,  3},
    {3,            0x7EC909F9D5446975ULL,  5, 21},
    {4,            0x490C0D4D1C5B3747ULL,  7,  7},
    {5,            0x11FFA3C346720519ULL,  9, 25},
    {6,            0xED9213F3AA88D2EAULL, 10, 10},
    {7,            0xB685AA69D49FA0BCULL, 12, 28},
    {8,            0x82181A9A38B66E8EULL, 14, 14},
    {9,            0x5C5B1DED7FCD3C60ULL,  0,  0},
    {10,           0x23FF47868CE40A32ULL,  2, 18},
    {11,           0xFE424AD9D3FAD803ULL,  3,  3},
    {12,           0xCB2427E75511A5D5ULL,  5, 21},
    {13,           0x9417BE5D7F2873A7ULL,  7,  7},
    {14,           0x6D0B54D3A93F4179ULL,  9, 25},
    {15,           0x35FEEB49D3560F4BULL, 11, 11},
    {16,           0x04303534716CDD1DULL, 13, 29},
    {17,           0xDE733887B883AAEEULL, 14, 14},
    {31,           0x34F16D09D0C2EC68ULL,  8,  8},
    {32,           0x08606A68E2D9BA3AULL, 10, 26},
    {33,           0xD2A36DBC29F0880BULL, 11, 11},
    {63,           0x32D67089CB9CA6A2ULL,  2,  2},
    {64,           0x00C0D4D1C5B37474ULL,  4, 20},
    {65,           0xDB03D8250CCA4245ULL,  5,  5},
    {UINT64_MAX-1, 0xD2EC03534716CDD1ULL,  1, 17},
    {UINT64_MAX,   0x02E3000000000000ULL,  0,  0},
  };

#if 0  /* print a full table of hash results */
  for (int i = 0; i < ANSWERS; ++i) {
    uintptr_t x = hash_xorshift64s_ptr(answers[i][INPUT]);
    print_message(
        "%lu\t%lx\t%lu\t%lu\n",
        answers[i][INPUT], x, x & 0b1111, x & 0b11111);
  }
#endif

  for (int i = 0; i < ANSWERS; ++i) {
    assert_int_equal(
        hash_xorshift64s_ptr(answers[i][INPUT]), answers[i][EXPECTED]);
  }
}
#endif

TEST(test_hash_xorshift64s, {
  (void) state;

  do_test_hash_xorshift64s();
})

TEST(test_hash_xorshift64s_ptr, {
  (void) state;

  do_test_hash_xorshift64s_ptr();
})


#ifndef METHODS_ONLY
void dump_totals(size_t *totals, const size_t max_value) {
#if 0  /* for manually debugging tests */
  size_t s;

  for (s = 0; s < max_value; ++s) {
    if (s == max_value / 2) {
      print_message("\n");
    }
    print_message("% 3lu", s);
  }
  print_message("\n");
  for (s = 0; s < max_value; ++s) {
    if (s == max_value / 2) {
      print_message("\n");
    }
    print_message("% 3lu", totals[s]);
  }
  print_message("\n\n");
#endif
}

// This is a pointer I pulled from gdb so close enough
#define EXAMPLE_PTR 0x617000000300
#define FAR         0x07F00000A000

void do_test_hash_xorshift64s_ptr_no_pattern() {
  size_t totals[/* 64 */ 1 << 6] = { 0 };

  uintptr_t p;
  size_t s;

  for (uintptr_t i = 4; i <= 6; ++i) {
    const size_t max_value = (size_t) 1 << i,
                 bitmask = max_value - 1;
    uintptr_t last = 0;

    // For nearby (or sequential) pointers, we want as flat a distribution as
    // possible to ensure values end up in different buckets...

    for (p = 0; p < max_value; ++p) {
      ++totals[hash_xorshift64s_ptr(EXAMPLE_PTR + p * 0x10) & bitmask];
    }

    assert_int_not_equal(0, totals[0]);
    last = totals[0];

    for (s = 1; s < max_value; ++s) {
      assert_int_equal(totals[0], totals[s]);
    }

    for (; p < max_value * 2; ++p) {
      ++totals[hash_xorshift64s_ptr(EXAMPLE_PTR + p * 0x10) & bitmask];
    }

    assert_int_not_equal(last, totals[0]);
    last = totals[0];

    for (s = 1; s < max_value; ++s) {
      assert_int_equal(totals[0], totals[s]);
    }

    memset(totals, 0, sizeof (size_t) * max_value);

    // But for distant, scattered pointers, we just want to ensure there are no
    // huge clusters of values, i.e., no more than 2*iterations and no fewer
    // than iterations/4 of values end up in any one bucket after power-of-two
    // compression.

    const size_t iterations = 8;

    for (p = 0; p < max_value * iterations; ++p) {
      ++totals[hash_xorshift64s_ptr(EXAMPLE_PTR - FAR + p * 0x1230) & bitmask];
    }

    dump_totals(totals, max_value);

    assert_int_not_equal(0, totals[0]);

    for (s = 0; s < max_value; ++s) {
      assert_in_range(totals[s], iterations / 4, iterations * 2);
    }

    memset(totals, 0, sizeof (size_t) * max_value);

    // Repeat the test but with pseudorandom values instead.

    last = hash_xorshift64s(~EXAMPLE_PTR ^ (FAR + i));

    for (p = 0; p < max_value * iterations; ++p) {
      last = hash_xorshift64s(last);
      ++totals[hash_xorshift64s_ptr(last) & bitmask];
    }

    dump_totals(totals, max_value);

    assert_int_not_equal(0, totals[0]);

    for (s = 0; s < max_value; ++s) {
      assert_in_range(totals[s], iterations / 4, iterations * 2);
    }

    memset(totals, 0, sizeof (size_t) * max_value);

    // A footnote: you can definitely make this test fail by picking different
    // constants. However, I'm happy to just pass it with a sufficiently large
    // sample size and call it a day.
  }
}

#undef EXAMPLE_PTR

#undef ANSWERS
#undef INPUT
#undef EXPECTED

#endif

TEST(test_hash_xorshift64s_ptr_no_pattern, {
  (void) state;

  do_test_hash_xorshift64s_ptr_no_pattern();
})

#ifndef _WINDLL
#ifndef METHODS_ONLY
#define METHODS_ONLY
#undef TEST
#define TEST(name, def) cmocka_unit_test(name),
int main(void) {
  const struct CMUnitTest tests[] = {
#include "test_hash.c"
  };

  return cmocka_run_group_tests_name(
      "flytools hash", tests, hash_setup, hash_teardown);
}
#endif  // METHODS_ONLY
#endif
