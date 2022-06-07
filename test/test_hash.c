#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#include "hash.h"


int hash_setup(void **state) {
  (void) state;

  return 0;
}

int hash_teardown(void **state) {
  (void) state;

  return 0;
}

#define ANSWERS 16
#define INPUT 0
#define EXPECTED 1

void test_hash_xorshift64s(void **state) {
  (void) state;

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
    assert_int_equal(hash_xorshift64s(answers[i][INPUT]), answers[i][EXPECTED]);
  }
}

#undef ANSWERS
#undef INPUT
#undef EXPECTED

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_hash_xorshift64s),
  };

  return cmocka_run_group_tests_name(
      "flytools hash", tests, hash_setup, hash_teardown);
}
