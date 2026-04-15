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
void validate_new_arena(arena *a) {
  assert_non_null(a);
  assert_fly_status(FLY_OK);
  assert_ptr_equal(a->next, a->block->data);
  assert_null(a->large);
  assert_null(a->frame);
}

arena *new_test_arena(size_t size) {
  arena *a = arena_new(size);

  validate_new_arena(a);

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

  if (aligned) {
    a = new_test_arena(ARENA_MINIMUM_SIZE + 13);

    assert_non_null(last = arena_alloc(a, ARENA_MINIMUM_SIZE));
    assert_ptr_equal(a->block->data, last);
    assert_int_equal(ARENA_MINIMUM_SIZE, a->next - (uint8_t *) last);
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    uint8_t *data = a->block->data;

    assert_non_null(current = arena_alloc_aligned(a, 13, 1));
    assert_int_equal(ARENA_MINIMUM_SIZE, (char *) current - (char *) last);
    assert_int_equal(13, a->next - (uint8_t *) current);
    assert_null(a->large);
    assert_null(a->block->prev);
    assert_fly_status(FLY_OK);

    assert_non_null(current = arena_alloc_aligned(a, 1, 1));
    assert_ptr_equal(a->block->data, current);
    assert_int_equal(1, a->next - (uint8_t *) current);
    assert_ptr_not_equal(last, current);
    assert_ptr_not_equal(data, a->block->data);
    assert_null(a->large);
    assert_non_null(a->block->prev);
    assert_fly_status(FLY_OK);

    assert_int_equal(2 * (ARENA_MINIMUM_SIZE + 13) - 1, a->end - a->next);

    arena_clear(a);

    assert_ptr_equal(data, a->block->data);
    assert_null(a->block->prev);
    assert_null(a->large);
    assert_int_equal(ARENA_MINIMUM_SIZE + 13, a->end - a->next);
    assert_ptr_equal(data, a->next);

    arena_del(a);
  }
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

size_t count_large_allocs(arena *a) {
  size_t ret = 0;

  for (struct arena_large_alloc *ala = a->large; ala; ala = ala->prev) {
    ++ret;
  }

  return ret;
}

size_t count_arena_frames(arena *a) {
  size_t ret = 0;

  for (struct arena_frame *af = a->frame; af; af = af->prev) {
    ++ret;
  }

  return ret;
}

void test_arena__alloc_entire_block_plus_one(arena *a, size_t expected_large) {
  uint8_t *alloc, *before;

  if (a->end - a->next < (ptrdiff_t) sizeof (struct arena_large_alloc)) {
    // Expect a new block because the struct won't fit
    before = NULL;
  } else {
    before = a->next;
  }

  size_t size = (size_t) (a->end - a->block->data) + 1;
  assert_non_null(alloc = arena_alloc_aligned(a, size, 1));

  if (!before) {
    before = a->block->data;
  }

  assert_ptr_equal(before + sizeof (struct arena_large_alloc), a->next);
  assert_int_equal(expected_large, count_large_allocs(a));
  assert_ptr_equal(a->large->data, alloc);
}

void test_arena__alloc_half_block_after_large(arena *a, size_t expected_large) {
  uint8_t *alloc;

  size_t size = ARENA_MINIMUM_SIZE / 2;
  assert_non_null(alloc = arena_alloc(a, size));
  assert_ptr_equal(
      a->block->data + sizeof (struct arena_large_alloc) * expected_large,
      alloc);
  assert_int_equal(expected_large, count_large_allocs(a));
  assert_ptr_not_equal(a->large->data, alloc);
  assert_int_equal(
      size - sizeof (struct arena_large_alloc) * expected_large,
      a->end - a->next);
  assert_null(a->block->prev);
}

void test_arena__alloc_one_byte_beyond_block_after_half_is_large(
    arena *a, size_t expected_large) {
  uint8_t *alloc;

  // one less expected_large because we haven't allocated yet
  size_t size = ARENA_MINIMUM_SIZE / 2
    - sizeof (struct arena_large_alloc) * (expected_large - 1)
    + 1;

  assert_non_null(alloc = arena_alloc_aligned(a, size, 1));
  assert_ptr_equal(
      a->block->data + ARENA_MINIMUM_SIZE / 2
        + sizeof (struct arena_large_alloc) * expected_large,
      a->next);
  assert_int_equal(expected_large, count_large_allocs(a));
  assert_ptr_equal(a->large->data, alloc);
  assert_null(a->block->prev);
}

void test_arena__alloc_all_but_half_large_struct_remaining(
    arena *a, size_t expected_large) {
  uint8_t *alloc;

  size_t size = ARENA_MINIMUM_SIZE / 2
    - (2 * expected_large + 1) * sizeof (struct arena_large_alloc) / 2;

  assert_non_null(alloc = arena_alloc_aligned(a, size, 1));
  assert_ptr_equal(a->end - sizeof (struct arena_large_alloc) / 2, a->next);
  assert_int_equal(expected_large, count_large_allocs(a));
  assert_null(a->block->prev);
}

void do_test_arena_alloc_large() {
  arena *a = new_test_arena(ARENA_MINIMUM_SIZE);

  uint8_t *alloc;

  test_arena__alloc_entire_block_plus_one(a, 1);
  assert_null(a->block->prev);

  test_arena__alloc_half_block_after_large(a, 1);
  test_arena__alloc_one_byte_beyond_block_after_half_is_large(a, 2);
  test_arena__alloc_all_but_half_large_struct_remaining(a, 2);

  // This should cause a large allocation that also triggers a block allocation
  // to make room for the arena_large_alloc struct
  test_arena__alloc_entire_block_plus_one(a, 3);
  assert_non_null(a->block->prev);
  assert_int_equal(
      2 * ARENA_MINIMUM_SIZE - sizeof (struct arena_large_alloc),
      a->end - a->next);

  size_t size = 2 * ARENA_MINIMUM_SIZE - 3 * sizeof (struct arena_large_alloc);
  assert_non_null(alloc = arena_alloc_aligned(a, size, 1));
  assert_ptr_equal(a->end - 2 * sizeof (struct arena_large_alloc), a->next);
  assert_int_equal(3, count_large_allocs(a));

  size = 2 * ARENA_MINIMUM_SIZE;
  assert_non_null(alloc = arena_alloc_aligned(a, size, 1));
  assert_ptr_equal(a->end - sizeof (struct arena_large_alloc), a->next);
  assert_int_equal(4, count_large_allocs(a));
  assert_ptr_equal(a->large->data, alloc);

  arena_del(a);
}

uint8_t *test__arena_push(
    arena *a, size_t expected_large, size_t expected_frames) {
  uint8_t *before = a->next;
  uint8_t *expected_after = before + sizeof (struct arena_frame);

  arena_push(a);
  assert_fly_status(FLY_OK);
  assert_ptr_equal(expected_after, a->next);
  assert_int_equal(expected_large, count_large_allocs(a));
  assert_null(a->block->prev);
  assert_int_equal(expected_frames, count_arena_frames(a));
  assert_ptr_equal(before, a->frame->context.next);
  assert_ptr_equal(a->large, a->frame->context.large);
  assert_ptr_equal(a->block, a->frame->context.block);

  return before;
}

void do_test_arena_pop_before_push() {
  arena *a = new_test_arena(ARENA_MINIMUM_SIZE);

  arena_pop(a);
  assert_fly_status(FLY_EMPTY);
  fly_status = FLY_OK;
  validate_new_arena(a);

  arena_del(a);
}

void do_test_arena_push_then_pop() {
  arena *a = new_test_arena(ARENA_MINIMUM_SIZE);
  uint8_t *initial_next = a->next;

  arena_push(a);
  assert_ptr_equal(initial_next + sizeof (struct arena_frame), a->next);
  assert_null(a->block->prev);
  assert_null(a->large);
  assert_int_equal(1, count_arena_frames(a));

  arena_pop(a);
  validate_new_arena(a);

  arena_del(a);
}

void do_test_arena_push_and_pop_large() {
  arena *a = new_test_arena(ARENA_MINIMUM_SIZE);

  test_arena__alloc_entire_block_plus_one(a, 1);
  assert_null(a->block->prev);
  assert_int_equal(0, count_arena_frames(a));

  uint8_t *before = test__arena_push(a, 1, 1);

  double *d = arena_alloc_type(a, double);
  assert_non_null(d);
  assert_ptr_not_equal(before, a->next);

  arena_pop(a);
  assert_ptr_equal(before, a->next);
  assert_int_equal(1, count_large_allocs(a));
  assert_null(a->block->prev);
  assert_int_equal(0, count_arena_frames(a));

  before = test__arena_push(a, 1, 1);

  test_arena__alloc_entire_block_plus_one(a, 2);
  assert_null(a->block->prev);
  assert_int_equal(1, count_arena_frames(a));

  arena_pop(a);
  assert_ptr_equal(before, a->next);
  assert_int_equal(1, count_large_allocs(a));
  assert_null(a->block->prev);
  assert_int_equal(0, count_arena_frames(a));

  arena_del(a);
}
#endif

TESTCALL(test_arena_new_default, do_test_arena_new(0))
TESTCALL(test_arena_new_sized, do_test_arena_new(ARENA_DEFAULT_SIZE / 2))

TESTCALL(test_arena_alloc, do_test_arena_alloc(false))
TESTCALL(test_arena_alloc_aligned, do_test_arena_alloc(true))
TESTCALL(test_arena_alloc_type, do_test_arena_alloc_type())
TESTCALL(test_arena_alloc_large, do_test_arena_alloc_large())

TESTCALL(test_arena_pop_before_push, do_test_arena_pop_before_push())
TESTCALL(test_arena_push_then_pop, do_test_arena_push_then_pop())
TESTCALL(test_arena_push_and_pop_large, do_test_arena_push_and_pop_large())

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
