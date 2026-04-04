#include <stdalign.h>
#include <stdbool.h>

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
arena *new_test_arena(size_t size) {
  arena *a = arena_new(size);

  assert_non_null(a);
  assert_fly_status(FLY_OK);
  assert_ptr_equal(a->next, a->data);

  return a;
}

void do_test_arena_new(size_t size) {
  arena *a = new_test_arena(size);

  arena_del(a);
}

struct test_obj {
  void *_1, *_2, *_3, *_4;
};

void do_test_arena_alloc(bool aligned) {
  arena *a = new_test_arena(0);

  uint8_t *first, *second;
  size_t size, align;

  if (!aligned) {
    first = (uint8_t *) arena_alloc(a, size = sizeof (uint8_t));
    align = alignof (max_align_t);
  } else {
    first = arena_alloc_aligned(
        a, size = sizeof (uint8_t), align = alignof (uint8_t));
  }

  assert_non_null(first);
  assert_true(a->next - first >= size);
  assert_true((uintptr_t) first % align == 0);

  if (!aligned) {
    second = (uint8_t *) arena_alloc(a, size = sizeof (struct test_obj));
    align = alignof (max_align_t);
  } else {
    second = arena_alloc_aligned(
        a, size = sizeof (struct test_obj), align = alignof (struct test_obj));
  }

  assert_non_null(second);
  assert_true(a->next - second >= size);
  assert_true((uintptr_t) second % align == 0);

  arena_del(a);
}
#endif

TESTCALL(test_arena_new_default, do_test_arena_new(0))
TESTCALL(test_arena_new_sized, do_test_arena_new(ARENA_DEFAULT_SIZE / 2))

TESTCALL(test_arena_alloc, do_test_arena_alloc(false))
TESTCALL(test_arena_alloc_aligned, do_test_arena_alloc(true))

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
