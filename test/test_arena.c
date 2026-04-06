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
  assert_ptr_equal(a->next, a->block->data);

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
  assert_true(a->next - first >= (ptrdiff_t) size);
  assert_true((uintptr_t) first % align == 0);

  if (!aligned) {
    second = (uint8_t *) arena_alloc(a, size = sizeof (struct test_obj));
    align = alignof (max_align_t);
  } else {
    second = arena_alloc_aligned(
        a, size = sizeof (struct test_obj), align = alignof (struct test_obj));
  }

  assert_non_null(second);
  assert_true(a->next - second >= (ptrdiff_t) size);
  assert_true((uintptr_t) second % align == 0);

  arena_del(a);

  a = new_test_arena(ARENA_MINIMUM_SIZE);

  if (!aligned) {
    assert_non_null(arena_alloc(a, ARENA_MINIMUM_SIZE));
    assert_null(a->block->prev);  // If this fails, arena_block is misaligned
    assert_non_null(arena_alloc(a, ARENA_MINIMUM_SIZE));
    assert_non_null(a->block->prev);
  } else {
    assert_non_null(arena_alloc_aligned(a, ARENA_MINIMUM_SIZE, 1));
    assert_null(a->block->prev);
    assert_non_null(arena_alloc_aligned(a, ARENA_MINIMUM_SIZE, 1));
    assert_non_null(a->block->prev);
  }

  arena_del(a);

  a = new_test_arena(2 * ARENA_MINIMUM_SIZE);

  void *last, *current;

  if (!aligned) {
    assert_non_null(last = arena_alloc(a, ARENA_MINIMUM_SIZE));
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    assert_non_null(current = arena_alloc(a, ARENA_MINIMUM_SIZE - 7));
    assert_ptr_not_equal(last, current);
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    assert_non_null(current = arena_alloc(a, 7));
    assert_ptr_not_equal(last, current);
    assert_null(a->large);
    assert_non_null(a->block->prev);
    assert_fly_status(FLY_OK);
  } else {
    assert_non_null(last = arena_alloc_aligned(a, 1, 1));
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    assert_non_null(current = arena_alloc_aligned(a, 2, 2));
    assert_int_equal(2, (char *) current - (char *) last);
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    last = current;
    assert_non_null(current = arena_alloc_aligned(a, 3, 4));
    assert_int_equal(2, (char *) current - (char *) last);
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    last = current;
    assert_non_null(current = arena_alloc_aligned(a, 5, alignof (max_align_t)));
    assert_int_equal(
        alignof (max_align_t) - 4,
        (char *) current - (char *) last);
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    last = current;
    assert_non_null(current = arena_alloc_aligned(
          a, 2 * (ARENA_MINIMUM_SIZE - alignof (max_align_t)), 1));
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    for (size_t i = 6; i <= alignof (max_align_t); i++) {
      assert_non_null(arena_alloc_aligned(a, 1, 1));
      assert_null(a->large);
      assert_null(a->block->prev);
      assert_fly_status(FLY_OK);
    }

    assert_non_null(arena_alloc_aligned(a, 1, 1));
    assert_null(a->large);
    assert_non_null(a->block->prev);
    assert_fly_status(FLY_OK);
  }

  arena_del(a);
}

void do_test_arena_alloc_type() {
  arena *a = new_test_arena(0);

  int *i = arena_alloc_type(a, int);
  double *d = arena_alloc_type(a, double);
  void **p = arena_alloc_type(a, void *);
  unsigned short *u = arena_alloc_type(a, unsigned short);
  struct test_obj *o = arena_alloc_type(a, struct test_obj);

  *i = 1234;
  *d = 56.789;
  *p = (void *) 0xF0F0;
  *u = 108u;
  o->_1 = (void *) 0x1;
  o->_2 = (void *) 0x2;
  o->_3 = (void *) 0x3;
  o->_4 = (void *) 0x4;

  assert_int_equal(1234, *i);
  assert_true(*d == 56.789);
  assert_ptr_equal((void *) 0xF0F0, *p);
  assert_int_equal(108u, *u);
  assert_ptr_equal((void *) 0x1, o->_1);
  assert_ptr_equal((void *) 0x2, o->_2);
  assert_ptr_equal((void *) 0x3, o->_3);
  assert_ptr_equal((void *) 0x4, o->_4);

  arena_del(a);
}
#endif

TESTCALL(test_arena_new_default, do_test_arena_new(0))
TESTCALL(test_arena_new_sized, do_test_arena_new(ARENA_DEFAULT_SIZE / 2))

TESTCALL(test_arena_alloc, do_test_arena_alloc(false))
TESTCALL(test_arena_alloc_aligned, do_test_arena_alloc(true))
TESTCALL(test_arena_alloc_type, do_test_arena_alloc_type())

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
