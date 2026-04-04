#include "tests.h"

#include "arena.h"


#if !defined(_WINDLL) && !defined(METHODS_ONLY)
int arena_setup(void **state) {
  (void) state;

  return 0;
}

int arena_teardown(void **state) {
  (void) state;

  return 0;
}
#endif

#ifndef METHODS_ONLY
void do_test_arena_new(size_t size) {
  arena *a = arena_new(size);

  assert_non_null(a);
  assert_fly_status(FLY_OK);
  assert_ptr_equal(a->next, a->data);

  arena_del(a);
}
#endif

TESTCALL(test_arena_new_default, do_test_arena_new(0))
TESTCALL(test_arena_new_sized, do_test_arena_new(ARENA_DEFAULT_SIZE / 2))

#ifndef _WINDLL
#ifndef METHODS_ONLY
#define METHODS_ONLY
#undef TEST
#define TEST(name, def) cmocka_unit_test(name),
int main(void) {
  const struct CMUnitTest tests[] = {
#include "test_arena.c"
  };

  return cmocka_run_group_tests_name(
      "flytools arena", tests, arena_setup, arena_teardown);
}
#endif  // METHODS_ONLY
#endif
