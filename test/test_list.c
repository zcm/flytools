#include "tests.h"

#include "list.h"
#include "internal/common.h"

#ifndef ARLIST_DEFAULT_CAPACITY
#define ARLIST_DEFAULT_CAPACITY 8
#endif

#ifndef _WINDLL
int list_setup(void **state) {
  (void) state;

  return 0;
}

int list_teardown(void **state) {
  (void) state;

  return 0;
}
#endif

TEST(test_list_new, {
  (void) state;

  list *l = list_new();
  assert_non_null(l);
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);
  list_del(l);
})

#ifndef METHODS_ONLY
void do_test_list_push_pop(listkind *kind) {
  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new_kind(kind);
  assert_non_null(l);
  assert_fly_status(FLY_OK);

  list_push(l, data[0]);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);
  list_push(l, data[1]);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);
  list_push(l, data[2]);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);
  list_push(l, data[3]);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);
  list_push(l, data[4]);
  assert_int_equal(5, l->size);
  assert_fly_status(FLY_OK);

  char *value;

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("fifth", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("fourth", value);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("third", value);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("second", value);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("first", value);
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  list_del(l);
}

void do_test_list_unshift_shift(listkind *kind) {
  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new_kind(kind);
  assert_non_null(l);
  assert_fly_status(FLY_OK);

  list_unshift(l, data[0]);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);
  list_unshift(l, data[1]);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);
  list_unshift(l, data[2]);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);
  list_unshift(l, data[3]);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);
  list_unshift(l, data[4]);
  assert_int_equal(5, l->size);
  assert_fly_status(FLY_OK);

  char *value;

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("fifth", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("fourth", value);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("third", value);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("second", value);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("first", value);
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  list_del(l);
}

void do_test_list_push_shift(listkind *kind) {
  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new_kind(kind);
  assert_non_null(l);
  assert_fly_status(FLY_OK);

  list_push(l, data[0]);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);
  list_push(l, data[1]);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);
  list_push(l, data[2]);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);
  list_push(l, data[3]);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);
  list_push(l, data[4]);
  assert_int_equal(5, l->size);
  assert_fly_status(FLY_OK);

  char *value;

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("first", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("second", value);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("third", value);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("fourth", value);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("fifth", value);
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  list_del(l);
}

void do_test_list_unshift_pop(listkind *kind) {
  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new_kind(kind);
  assert_non_null(l);
  assert_fly_status(FLY_OK);

  list_unshift(l, data[0]);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);
  list_unshift(l, data[1]);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);
  list_unshift(l, data[2]);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);
  list_unshift(l, data[3]);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);
  list_unshift(l, data[4]);
  assert_int_equal(5, l->size);
  assert_fly_status(FLY_OK);

  char *value;

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("first", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("second", value);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("third", value);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("fourth", value);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("fifth", value);
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  list_del(l);
}
#endif

TESTCALL(test_arlist_push_pop, do_test_list_push_pop(LISTKIND_ARRAY))
TESTCALL(test_arlist_unshift_shift, do_test_list_unshift_shift(LISTKIND_ARRAY))
TESTCALL(test_arlist_push_shift, do_test_list_push_shift(LISTKIND_ARRAY))
TESTCALL(test_arlist_unshift_pop, do_test_list_unshift_pop(LISTKIND_ARRAY))

TESTCALL(test_deque_push_pop, do_test_list_push_pop(LISTKIND_DEQUE))
TESTCALL(test_deque_unshift_shift, do_test_list_unshift_shift(LISTKIND_DEQUE))
TESTCALL(test_deque_push_shift, do_test_list_push_shift(LISTKIND_DEQUE))
TESTCALL(test_deque_unshift_pop, do_test_list_unshift_pop(LISTKIND_DEQUE))

TESTCALL(test_dllist_push_pop, do_test_list_push_pop(LISTKIND_DLINK))
TESTCALL(test_dllist_unshift_shift, do_test_list_unshift_shift(LISTKIND_DLINK))
TESTCALL(test_dllist_push_shift, do_test_list_push_shift(LISTKIND_DLINK))
TESTCALL(test_dllist_unshift_pop, do_test_list_unshift_pop(LISTKIND_DLINK))

TESTCALL(test_sllist_push_pop, do_test_list_push_pop(LISTKIND_SLINK))
TESTCALL(test_sllist_unshift_shift, do_test_list_unshift_shift(LISTKIND_SLINK))
TESTCALL(test_sllist_push_shift, do_test_list_push_shift(LISTKIND_SLINK))
TESTCALL(test_sllist_unshift_pop, do_test_list_unshift_pop(LISTKIND_SLINK))

#ifndef METHODS_ONLY
void do_test_list_pop_empty(listkind *kind) {
  list *l = list_new_kind(kind);
  assert_non_null(l);
  assert_fly_status(FLY_OK);

  assert_int_equal(0, l->size);
  assert_null(list_pop(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_EMPTY);

  list_push(l, l);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  assert_ptr_equal(l, list_pop(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_pop(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_EMPTY);

  list_push(l, l);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  assert_ptr_equal(l, list_pop(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_pop(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_EMPTY);

  list_del(l);
}

void do_test_list_shift_empty(listkind *kind) {
  list *l = list_new_kind(kind);
  assert_non_null(l);
  assert_fly_status(FLY_OK);

  assert_int_equal(0, l->size);
  assert_null(list_shift(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_EMPTY);

  list_unshift(l, l);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  assert_ptr_equal(l, list_shift(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_shift(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_EMPTY);

  list_unshift(l, l);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  assert_ptr_equal(l, list_shift(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_shift(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_EMPTY);

  list_del(l);
}

void do_test_list_del_nonempty(listkind *kind) {
  list *l = list_new_kind(kind);
  assert_non_null(l);

  list_push(l, l);
  list_push(l, l);
  list_push(l, l);

  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  list_del(l);
}
#endif

TESTCALL(test_arlist_pop_empty, do_test_list_pop_empty(LISTKIND_ARRAY))
TESTCALL(test_arlist_shift_empty, do_test_list_shift_empty(LISTKIND_ARRAY))
TESTCALL(test_arlist_del_nonempty, do_test_list_del_nonempty(LISTKIND_ARRAY))

TESTCALL(test_deque_pop_empty, do_test_list_pop_empty(LISTKIND_DEQUE))
TESTCALL(test_deque_shift_empty, do_test_list_shift_empty(LISTKIND_DEQUE))
TESTCALL(test_deque_del_nonempty, do_test_list_del_nonempty(LISTKIND_DEQUE))

TESTCALL(test_dllist_pop_empty, do_test_list_pop_empty(LISTKIND_DLINK))
TESTCALL(test_dllist_shift_empty, do_test_list_shift_empty(LISTKIND_DLINK))
TESTCALL(test_dllist_del_nonempty, do_test_list_del_nonempty(LISTKIND_DLINK))

TESTCALL(test_sllist_pop_empty, do_test_list_pop_empty(LISTKIND_SLINK))
TESTCALL(test_sllist_shift_empty, do_test_list_shift_empty(LISTKIND_SLINK))
TESTCALL(test_sllist_del_nonempty, do_test_list_del_nonempty(LISTKIND_SLINK))

#ifndef METHODS_ONLY
void do_test_list_get(listkind *kind) {
  list *l;

  assert_non_null(l = list_new_kind(kind));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_get(l, 0));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_OK;

  assert_null(list_get(l, 1));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_OK;

  assert_null(list_get(l, -1));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_OK;

  assert_null(list_get(l, PTRDIFF_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_OK;

  assert_null(list_get(l, PTRDIFF_MIN));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_OK;

  assert_null(list_get(l, PTRINDEX_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_OK;

  assert_null(list_get(l, l->size));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_OK;

  if (l->kind == LISTKIND_ARRAY) {
    assert_null(list_get(l, ((arlist *) l)->capacity));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
  }

  fly_status = FLY_NOT_FOUND;

  list_push(l, (void *) 123);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);
  fly_status = FLY_NOT_FOUND;

  assert_int_equal(123, (uintptr_t) list_get(l, 0));
  assert_fly_status(FLY_OK);
  fly_status = FLY_E_TOO_BIG;

  assert_int_equal(123, (uintptr_t) list_get(l, -1));
  assert_fly_status(FLY_OK);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, 1));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, -2));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, PTRDIFF_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, PTRDIFF_MIN));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, PTRINDEX_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_OK;

  assert_null(list_get(l, l->size));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  if (l->kind == LISTKIND_ARRAY) {
    assert_null(list_get(l, ((arlist *) l)->capacity));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
  }

  list_unshift(l, (void *) 456);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_get(l, 2));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_int_equal(456, (uintptr_t) list_get(l, 0));
  assert_fly_status(FLY_OK);
  fly_status = FLY_E_TOO_BIG;

  assert_int_equal(456, (uintptr_t) list_get(l, -2));
  assert_fly_status(FLY_OK);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, PTRDIFF_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, PTRDIFF_MIN));
  assert_null(list_get(l, PTRINDEX_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_int_equal(123, (uintptr_t) list_get(l, 1));
  assert_fly_status(FLY_OK);
  fly_status = FLY_E_TOO_BIG;

  assert_int_equal(123, (uintptr_t) list_get(l, -1));
  assert_fly_status(FLY_OK);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, l->size));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  if (l->kind == LISTKIND_ARRAY) {
    assert_null(list_get(l, ((arlist *) l)->capacity));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
    fly_status = FLY_E_TOO_BIG;

    while (l->size < ((arlist *) l)->capacity) {
      list_unshift(l, (void *) 10664);
      assert_fly_status(FLY_OK);
      fly_status = FLY_E_TOO_BIG;
    }

    assert_int_equal(10664, (uintptr_t) list_get(l, 0));
    assert_fly_status(FLY_OK);
    fly_status = FLY_E_TOO_BIG;

    assert_int_equal(456, (uintptr_t) list_get(l, l->size - 2));
    assert_fly_status(FLY_OK);
    fly_status = FLY_E_TOO_BIG;

    assert_int_equal(456, (uintptr_t) list_get(l, -2));
    assert_fly_status(FLY_OK);
    fly_status = FLY_E_TOO_BIG;

    assert_int_equal(123, (uintptr_t) list_get(l, l->size - 1));
    assert_fly_status(FLY_OK);
    fly_status = FLY_E_TOO_BIG;

    assert_int_equal(123, (uintptr_t) list_get(l, -1));
    assert_fly_status(FLY_OK);
    fly_status = FLY_E_TOO_BIG;

    assert_null(list_get(l, PTRDIFF_MAX));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
    fly_status = FLY_E_TOO_BIG;

    assert_null(list_get(l, PTRDIFF_MIN));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
    fly_status = FLY_E_TOO_BIG;

    assert_null(list_get(l, PTRINDEX_MAX));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
    fly_status = FLY_E_TOO_BIG;

    assert_null(list_get(l, l->size));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
    fly_status = FLY_E_TOO_BIG;

    assert_null(list_get(l, l->size + 1));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
    fly_status = FLY_E_TOO_BIG;

  }

  while (l->size) {
    list_pop(l);
    assert_fly_status(FLY_OK);
    fly_status = FLY_E_TOO_BIG;
  }

  assert_null(list_get(l, 0));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, 1));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, -1));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, SIZE_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, PTRDIFF_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, PTRINDEX_MAX));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  assert_null(list_get(l, l->size));
  assert_fly_status(FLY_E_OUT_OF_RANGE);
  fly_status = FLY_E_TOO_BIG;

  if (l->kind == LISTKIND_ARRAY) {
    assert_null(list_get(l, ((arlist *) l)->capacity));
    assert_fly_status(FLY_E_OUT_OF_RANGE);
    fly_status = FLY_E_TOO_BIG;
  }

  list_del(l);
}
#endif

TESTCALL(test_arlist_get, do_test_list_get(LISTKIND_ARRAY))
TESTCALL(test_deque_get, do_test_list_get(LISTKIND_DEQUE))
TESTCALL(test_dllist_get, do_test_list_get(LISTKIND_DLINK))
TESTCALL(test_sllist_get, do_test_list_get(LISTKIND_SLINK))

#ifndef METHODS_ONLY
void do_test_list_capacity(listkind *kind) {
  list *l;
  size_t i = 0;

  assert_non_null(l = list_new_kind(kind));

#define al ((arlist *) l)

  while (i < ARLIST_DEFAULT_CAPACITY) {
    list_push(l, (void *) i++);
  }

  assert_int_equal(ARLIST_DEFAULT_CAPACITY, l->size);

  for (i = 0; i < ARLIST_DEFAULT_CAPACITY; i++) {
    assert_int_equal(i, (size_t) al->items[i]);
  }

  assert_int_equal(ARLIST_DEFAULT_CAPACITY, al->capacity);

  if (kind == LISTKIND_DEQUE) {
    assert_int_equal(0, ((deque *) l)->start);
    assert_int_equal(0, ((deque *) l)->end);
  }

  list_push(l, (void *) i);
  assert_int_equal(i, (size_t) al->items[ARLIST_DEFAULT_CAPACITY]);
  assert_int_equal(
      ARLIST_DEFAULT_CAPACITY + (ARLIST_DEFAULT_CAPACITY >> 1),
      al->capacity);

  if (kind == LISTKIND_DEQUE) {
    assert_int_equal(0, ((deque *) l)->start);
    assert_int_equal(9, ((deque *) l)->end);
  }

  list_del(l);
  assert_non_null(l = list_new_kind(kind));

  i = 0;

  while (i < ARLIST_DEFAULT_CAPACITY) {
    list_push(l, (void *) i++);
  }

  if (kind == LISTKIND_DEQUE) {
    assert_int_equal(0, ((deque *) l)->start);
    assert_int_equal(0, ((deque *) l)->end);
  }

  i >>= 1;

  while (i++ < ARLIST_DEFAULT_CAPACITY) {
    list_push(l, (void *) ((size_t) list_shift(l) * 10));
  }

  if (kind == LISTKIND_DEQUE) {
    assert_int_equal(4, ((deque *) l)->start);
    assert_int_equal(4, ((deque *) l)->end);
  }

  list_push(l, (void *) 666);

  assert_int_equal(
      ARLIST_DEFAULT_CAPACITY + (ARLIST_DEFAULT_CAPACITY >> 1),
      al->capacity);

  if (kind == LISTKIND_DEQUE) {
    assert_int_equal(4, ((deque *) l)->start);
    assert_int_equal(1, ((deque *) l)->end);
  }

#undef al

  list_del(l);
}
#endif

TESTCALL(test_arlist_capacity, do_test_list_capacity(LISTKIND_ARRAY))
TESTCALL(test_deque_capacity, do_test_list_capacity(LISTKIND_DEQUE))

#ifndef METHODS_ONLY
void do_test_list_concat() {
  static size_t data[] = {
    0x101, 0x102, 0x103, 0x201, 0x202, 0x203, 0x204
  };

  static int len_data = sizeof (data) / sizeof (size_t);

  list *l1, *l2;
  assert_non_null(l1 = list_new());
  assert_non_null(l2 = list_new());

  int i;

  for (i = 0; i < len_data / 2; i++) {
    list_push(l1, (void *) data[i]);
  }
  for (; i < len_data; i++) {
    list_push(l2, (void *) data[i]);
  }

  assert_int_equal(3, l1->size);
  assert_int_equal(4, l2->size);

  list_concat(l1, l2);

  while (i > 0) {
    assert_int_equal(i, l1->size);
    assert_ptr_equal((void *) data[--i], list_pop(l1));
  }

  assert_int_equal(0, l1->size);

  list_del(l1);
}

void do_test_list_concat_from_empty() {
  static size_t data[] = {
    0x101, 0x102, 0x103
  };

  static int len_data = sizeof (data) / sizeof (size_t);

  list *l1, *l2;
  assert_non_null(l1 = list_new());
  assert_non_null(l2 = list_new());

  int i;

  for (i = 0; i < len_data; i++) {
    list_unshift(l1, (void *) data[i]);
  }

  assert_int_equal(3, l1->size);
  assert_int_equal(0, l2->size);

  list_concat(l1, l2);

  while (i > 0) {
    assert_int_equal(i, l1->size);
    assert_ptr_equal((void *) data[--i], list_shift(l1));
  }

  assert_int_equal(0, l1->size);

  list_del(l1);
}

void do_test_list_concat_into_empty() {
  static size_t data[] = {
    0x101, 0x102, 0x103
  };

  static int len_data = sizeof (data) / sizeof (size_t);

  list *l1, *l2;
  assert_non_null(l1 = list_new());
  assert_non_null(l2 = list_new());

  int i;

  for (i = 0; i < len_data; i++) {
    list_unshift(l2, (void *) data[i]);
  }

  assert_int_equal(0, l1->size);
  assert_int_equal(3, l2->size);

  list_concat(l1, l2);

  while (i > 0) {
    assert_int_equal(i, l1->size);
    assert_ptr_equal((void *) data[--i], list_shift(l1));
  }

  assert_int_equal(0, l1->size);

  list_del(l1);
}

void do_test_list_concat_both_empty() {
  list *l1, *l2;
  assert_non_null(l1 = list_new());
  assert_non_null(l2 = list_new());

  assert_int_equal(0, l1->size);
  assert_int_equal(0, l2->size);

  list_concat(l1, l2);

  assert_int_equal(0, l1->size);
  assert_null(list_pop(l1));
  assert_int_equal(0, l1->size);

  list_del(l1);
}
#endif

TEST(test_list_concat, {
  (void) state;

  do_test_list_concat();
})

TEST(test_list_concat_from_empty, {
  (void) state;

  do_test_list_concat_from_empty();
})

TEST(test_list_concat_into_empty, {
  (void) state;

  do_test_list_concat_into_empty();
})

TEST(test_list_concat_both_empty, {
  (void) state;

  do_test_list_concat_both_empty();
})

#ifndef METHODS_ONLY
static void do_test_list_append_array(listkind *kind) {
  list *l;
  ptrdiff_t i;
  void *items[] = { (void *) 1, (void *) 2, (void *) 3, (void *) 4,
                    (void *) 5, (void *) 6, (void *) 7, (void *) 8 };

  assert_non_null(l = list_new_kind(kind));
  assert_fly_status(FLY_OK);

  list_append_array(NULL, 666, NULL);
  assert_fly_status(FLY_E_NULL_PTR);

  fly_status = FLY_OK;

  list_append_array(NULL, 666, items);
  assert_fly_status(FLY_E_NULL_PTR);

  list_append_array(l, 0, items);
  assert_fly_status(FLY_OK);
  assert_int_equal(0, l->size);

  if (kind == LISTKIND_ARRAY || kind == LISTKIND_DEQUE) {
    assert_null(((arlist *) l)->items);
    assert_int_equal(0, ((arlist *) l)->capacity);
  }

  list_append_array(l, 666, NULL);
  assert_fly_status(FLY_E_NULL_PTR);
  assert_int_equal(0, l->size);

  if (kind == LISTKIND_ARRAY || kind == LISTKIND_DEQUE) {
    assert_null(((arlist *) l)->items);
    assert_int_equal(0, ((arlist *) l)->capacity);
  }

  list_append_array(l, 8, items);
  assert_fly_status(FLY_OK);
  assert_int_equal(8, l->size);

  if (kind == LISTKIND_ARRAY || kind == LISTKIND_DEQUE) {
    assert_int_equal(8, ((arlist *) l)->capacity);
  }

  for (i = 0; i < 8; i++) {
    assert_int_equal(i + 1, (size_t) list_get(l, i));
  }
  for (i = -1; i >= -8; i--) {
    assert_int_equal(i + 9, (size_t) list_get(l, i));
  }

  fly_status = FLY_NOT_FOUND;

  list_append_array(l, 3, items);
  assert_fly_status(FLY_OK);
  assert_int_equal(11, l->size);

  if (kind == LISTKIND_ARRAY || kind == LISTKIND_DEQUE) {
    assert_int_equal(12, ((arlist *) l)->capacity);
  }

  for (i = 8; i < 11; i++) {
    assert_int_equal(i - 7, (size_t) list_get(l, i));
  }
  for (i = 0; i < 8; i++) {
    assert_int_equal(i + 1, (size_t) list_get(l, i));
  }

  const ptrdiff_t lower_limit = PTRINDEX_MAX - l->size;

  for (i = PTRINDEX_MAX; i > lower_limit; i--) {
    list_append_array(l, i, items);
    assert_fly_status(FLY_E_TOO_BIG);
    assert_int_equal(11, l->size);

    if (kind == LISTKIND_ARRAY || kind == LISTKIND_DEQUE) {
      assert_int_equal(12, ((arlist *) l)->capacity);
    }
  }

  if (kind == LISTKIND_DEQUE) {
    assert_int_equal(1, (size_t) list_shift(l));
    assert_int_equal(10, l->size);
    assert_fly_status(FLY_OK);
    assert_int_equal(2, (size_t) list_shift(l));
    assert_int_equal(9, l->size);
    assert_fly_status(FLY_OK);

    fly_status = FLY_NOT_FOUND;

    list_append_array(l, 3, items);
    assert_int_equal(12, l->size);
    assert_int_equal(12, ((arlist *) l)->capacity);
    assert_fly_status(FLY_OK);

    for (i = 0; i < 6; i++) {
      assert_int_equal(i + 3, (size_t) list_get(l, i));
    }
    for (; i < 9; i++) {
      assert_int_equal(i - 5, (size_t) list_get(l, i));
    }
    for (; i < 12; i++) {
      assert_int_equal(i - 8, (size_t) list_get(l, i));
    }
  }

  list_del(l);
}
#endif

TESTCALL(test_arlist_append_array, do_test_list_append_array(LISTKIND_ARRAY))
TESTCALL(test_deque_append_array, do_test_list_append_array(LISTKIND_DEQUE))
TESTCALL(test_dllist_append_array, do_test_list_append_array(LISTKIND_DLINK))
TESTCALL(test_sllist_append_array, do_test_list_append_array(LISTKIND_SLINK))

#ifndef METHODS_ONLY
#define DEFINE_FUNC_CHAR_IS(i, c) \
  int char##i##_is_##c(void *value) { \
    return value && ((char *) value)[i] == *#c; \
  }

DEFINE_FUNC_CHAR_IS(0, f);
DEFINE_FUNC_CHAR_IS(1, e);
DEFINE_FUNC_CHAR_IS(3, r);
DEFINE_FUNC_CHAR_IS(2, x);
DEFINE_FUNC_CHAR_IS(2, v);
DEFINE_FUNC_CHAR_IS(0, e);
DEFINE_FUNC_CHAR_IS(0, z);

static void do_test_list_find_first(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &char0_is_f));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  list_push(l, "first");
  list_push(l, "second");
  list_push(l, "third");
  list_push(l, "fourth");
  list_push(l, "fifth");
  list_push(l, "sixth");
  list_push(l, "seventh");

  assert_int_equal(7, l->size);
  assert_fly_status(FLY_OK);

  char *value;

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_find_first(l, &char0_is_f));
  assert_string_equal("first", value);
  assert_fly_status(FLY_OK);
  assert_non_null(value = list_find_first(l, &char1_is_e));
  assert_string_equal("second", value);
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_find_first(l, &char3_is_r));
  assert_string_equal("third", value);
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_find_first(l, &char2_is_x));
  assert_string_equal("sixth", value);
  assert_fly_status(FLY_OK);
  assert_non_null(value = list_find_first(l, &char2_is_v));
  assert_string_equal("seventh", value);
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  list_push(l, "eighth");

  assert_non_null(value = list_find_first(l, &char0_is_f));
  assert_string_equal("first", value);
  assert_fly_status(FLY_OK);

  list_shift(l);
  list_push(l, list_shift(l));
  list_push(l, list_shift(l));
  list_push(l, list_shift(l));

  assert_non_null(value = list_find_first(l, &char0_is_f));
  assert_string_equal("fifth", value);
  assert_fly_status(FLY_OK);

  list_unshift(l, "zeroth");

  assert_non_null(value = list_find_first(l, &char0_is_f));
  assert_string_equal("fifth", value);
  assert_fly_status(FLY_OK);

  assert_non_null(value = list_find_first(l, &char0_is_z));
  assert_string_equal("zeroth", value);
  assert_fly_status(FLY_OK);

  list_push(l, list_shift(l));

  assert_non_null(value = list_find_first(l, &char0_is_z));
  assert_string_equal("zeroth", value);
  assert_fly_status(FLY_OK);

  list_shift(l);

  assert_non_null(value = list_find_first(l, &char0_is_z));
  assert_string_equal("zeroth", value);
  assert_fly_status(FLY_OK);

  list_del(l);
}
#endif

TESTCALL(test_arlist_find_first, do_test_list_find_first(LISTKIND_ARRAY))
TESTCALL(test_deque_find_first, do_test_list_find_first(LISTKIND_DEQUE))
TESTCALL(test_dllist_find_first, do_test_list_find_first(LISTKIND_DLINK))
TESTCALL(test_sllist_find_first, do_test_list_find_first(LISTKIND_SLINK))

#ifndef METHODS_ONLY
DEFINE_FUNC_CHAR_IS(0, t);
DEFINE_FUNC_CHAR_IS(0, o);
DEFINE_FUNC_CHAR_IS(1, h);

int value_is_null(void *value) {
  return value == NULL;
}

static int value_was_null = 0;

int value_after_null(void *value) {
  if (value_was_null) {
    value_was_null = 0;
    return 1;
  }

  value_was_null = value == NULL;
  return 0;
}

static void do_test_list_find_first_null(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));
  assert_fly_status(FLY_OK);

  list_unshift(l, "three");
  list_unshift(l, "two");
  list_unshift(l, NULL);
  list_unshift(l, "one");

  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  char *value;

  value_was_null = 0;

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_find_first(l, &char0_is_t));
  assert_string_equal("two", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_find_first(l, &char0_is_o));
  assert_string_equal("one", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  assert_null(list_find_first(l, &value_is_null));
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_find_first(l, &value_after_null));
  assert_string_equal("two", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &char0_is_e));
  assert_fly_status(FLY_NOT_FOUND);

  list_shift(l);
  list_shift(l);
  list_unshift(l, "one");
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_find_first(l, &value_after_null));
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  list_del(l);
}
#endif

TESTCALL(test_arlist_find_first_null,
    do_test_list_find_first_null(LISTKIND_ARRAY))
TESTCALL(test_deque_find_first_null,
    do_test_list_find_first_null(LISTKIND_DEQUE))
TESTCALL(test_dllist_find_first_null,
    do_test_list_find_first_null(LISTKIND_DLINK))
TESTCALL(test_sllist_find_first_null,
    do_test_list_find_first_null(LISTKIND_SLINK))

#ifndef METHODS_ONLY
static void do_test_list_discard(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_f));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  list_push(l, "first");
  list_push(l, "second");
  list_push(l, "third");
  list_push(l, "fourth");
  list_push(l, "fifth");
  list_push(l, "sixth");
  list_push(l, "seventh");

  assert_int_equal(7, l->size);
  assert_fly_status(FLY_OK);

  char *value;

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(7, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char0_is_f));
  assert_string_equal("first", value);
  assert_int_equal(6, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(6, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char0_is_f));
  assert_string_equal("fourth", value);
  assert_int_equal(5, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(5, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char2_is_v));
  assert_string_equal("seventh", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char2_is_x));
  assert_string_equal("sixth", value);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char1_is_e));
  assert_string_equal("second", value);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char0_is_f));
  assert_string_equal("fifth", value);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char3_is_r));
  assert_string_equal("third", value);
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_null(list_discard(l, &char0_is_f));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  list_push(l, "one");
  list_push(l, "two");
  list_push(l, "three");
  list_push(l, "four");

  assert_non_null(value = list_discard(l, &char0_is_f));
  assert_string_equal("four", value);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  list_push(l, "not four");
  list_push(l, "five");

  assert_non_null(value = list_discard(l, &char0_is_f));
  assert_string_equal("five", value);
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  list_del(l);
}
#endif

TESTCALL(test_arlist_discard, do_test_list_discard(LISTKIND_ARRAY))
TESTCALL(test_deque_discard, do_test_list_discard(LISTKIND_DEQUE))
TESTCALL(test_dllist_discard, do_test_list_discard(LISTKIND_DLINK))
TESTCALL(test_sllist_discard, do_test_list_discard(LISTKIND_SLINK))

#ifndef METHODS_ONLY
static void do_test_list_discard_null(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));
  assert_fly_status(FLY_OK);

  list_unshift(l, "three");
  list_unshift(l, "two");
  list_unshift(l, NULL);
  list_unshift(l, "one");

  assert_int_equal(4, l->size);
  assert_fly_status(FLY_OK);

  char *value;

  value_was_null = 0;

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(4, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char0_is_t));
  assert_string_equal("two", value);
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &char0_is_o));
  assert_string_equal("one", value);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_discard(l, &value_after_null));
  assert_string_equal("three", value);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  list_unshift(l, "one 2.0");
  list_push(l, "three 2.0");
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(3, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_null(list_discard(l, &value_is_null));
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_shift(l));
  assert_string_equal("one 2.0", value);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &char0_is_e));
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = list_shift(l));
  assert_string_equal("three 2.0", value);
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_discard(l, &value_after_null));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_NOT_FOUND);

  list_del(l);
}
#endif

TESTCALL(test_arlist_discard_null,
    do_test_list_discard_null(LISTKIND_ARRAY))
TESTCALL(test_deque_discard_null,
    do_test_list_discard_null(LISTKIND_DEQUE))
TESTCALL(test_dllist_discard_null,
    do_test_list_discard_null(LISTKIND_DLINK))
TESTCALL(test_sllist_discard_null,
    do_test_list_discard_null(LISTKIND_SLINK))

#ifndef METHODS_ONLY
static uintptr_t prime = 0;
static size_t index_sum = 0;
static char order[9] = { 0 }, indices[9] = { 0 }, zilch[9] = { 0 };

int multiply_prime(void *data, size_t index) {
  prime *= (uintptr_t) data;
  index_sum += index;
  return 0;
}

int record_order(void *data, size_t index) {
  order[index] = (char) (uintptr_t) data + '0';
  indices[index] = (char) index + '0';
  return 0;
}

int record_order_r_stop(void *data, size_t index) {
  record_order(data, 2 - index + 1);
  return index == 2;
}

void do_test_list_foreach(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  prime = 1;
  index_sum = 0;

  list_foreach(l, &multiply_prime);

  assert_int_equal(1, prime);
  assert_int_equal(0, index_sum);
  assert_int_equal(0, l->size);

  list_push(l, (void *) 1);
  list_push(l, (void *) 2);
  list_push(l, (void *) 3);
  list_push(l, (void *) 5);
  list_push(l, (void *) 7);

  list_foreach(l, &multiply_prime);

  assert_int_equal(210, prime);
  assert_int_equal(10, index_sum);
  assert_int_equal(5, l->size);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  list_foreach(l, &record_order);

  assert_string_equal("12357", order);
  assert_string_equal("01234", indices);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  list_foreach(l, &record_order_r_stop);

  assert_int_equal(0, order[0]);
  assert_int_equal(0, indices[0]);

  order[0] = indices[0] = 'x';

  assert_string_equal("x321", order);
  assert_string_equal("x123", indices);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  list_push(l, list_shift(l));
  list_push(l, list_shift(l));

  list_foreach(l, &record_order);

  assert_string_equal("35712", order);
  assert_string_equal("01234", indices);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  list_push(l, list_shift(l));

  list_foreach(l, &record_order);

  assert_string_equal("57123", order);
  assert_string_equal("01234", indices);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  list_push(l, list_shift(l));

  list_foreach(l, &record_order);

  assert_string_equal("71235", order);
  assert_string_equal("01234", indices);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  list_push(l, list_shift(l));

  list_foreach(l, &record_order);

  assert_string_equal("12357", order);
  assert_string_equal("01234", indices);

  list_del(l);
}
#endif

TESTCALL(test_arlist_foreach, do_test_list_foreach(LISTKIND_ARRAY))
TESTCALL(test_deque_foreach, do_test_list_foreach(LISTKIND_DEQUE))
TESTCALL(test_dllist_foreach, do_test_list_foreach(LISTKIND_DLINK))
TESTCALL(test_sllist_foreach, do_test_list_foreach(LISTKIND_SLINK))

#ifndef METHODS_ONLY
int everything(void *unused_value) {
  return 1;
}

int nothing(void *unused_value) {
  return 0;
}

int value_is_odd(void *value) {
  return (uintptr_t) value & 0x1;
}

int value_is_even(void *value) {
  return !value_is_odd(value);
}

static size_t count = 0;

int record_order_by_count(void *data, size_t index) {
  order[count] = (char) (uintptr_t) data + '0';
  indices[count] = (char) (uintptr_t) index + '0';
  ++count;
  return 0;
}

void do_test_list_discard_all(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  prime = 1;
  index_sum = 2;

  assert_int_equal(
      0, list_discard_all(l, &everything, &multiply_prime));

  assert_int_equal(1, prime);
  assert_int_equal(2, index_sum);
  assert_int_equal(0, l->size);

  list_push(l, (void *) 1);
  list_push(l, (void *) 2);
  list_push(l, (void *) 3);
  list_push(l, (void *) 4);
  list_push(l, (void *) 5);

  assert_int_equal(5, l->size);

  assert_int_equal(
      0, list_discard_all(l, &nothing, &multiply_prime));

  assert_int_equal(1, prime);
  assert_int_equal(2, index_sum);
  assert_int_equal(5, l->size);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  count = 0;

  char expected_order[6] = "135";
  char expected_indices[6] = "024";

  assert_int_equal(
      3, list_discard_all(l, &value_is_odd, &record_order_by_count));

  assert_int_equal(2, l->size);
  assert_string_equal(expected_order, order);
  assert_string_equal(expected_indices, indices);
  assert_memory_equal(zilch, order + 3, sizeof (order) - 3);
  assert_memory_equal(zilch, indices + 3, sizeof (indices) - 3);

  order[0] = expected_order[0] = 'a';
  indices[0] = expected_indices[0] = 'b';

  assert_int_equal(
      0, list_discard_all(l, &value_is_odd, &record_order_by_count));

  assert_int_equal(2, l->size);
  assert_string_equal(expected_order, order);
  assert_string_equal(expected_indices, indices);
  assert_memory_equal(zilch, order + 3, sizeof (order) - 3);
  assert_memory_equal(zilch, indices + 3, sizeof (indices) - 3);

  count = 0;

  char expected_order_2[6] = "245";
  char expected_indices_2[6] = "014";

  assert_int_equal(
      2, list_discard_all(l, &value_is_even, &record_order_by_count));

  assert_int_equal(0, l->size);
  assert_string_equal(expected_order_2, order);
  assert_string_equal(expected_indices_2, indices);
  assert_memory_equal(zilch, order + 3, sizeof (order) - 3);
  assert_memory_equal(zilch, indices + 3, sizeof (indices) - 3);

  assert_int_equal(
      0, list_discard_all(l, &value_is_even, &record_order_by_count));

  assert_int_equal(0, l->size);
  assert_string_equal(expected_order_2, order);
  assert_string_equal(expected_indices_2, indices);
  assert_memory_equal(zilch, order + 3, sizeof (order) - 3);
  assert_memory_equal(zilch, indices + 3, sizeof (indices) - 3);

  list_push(l, (void *) 6);
  assert_int_equal(1, l->size);

  count = 0;
  expected_order_2[0] = '6';

  assert_int_equal(
      1, list_discard_all(l, &everything, &record_order_by_count));

  assert_int_equal(0, l->size);
  assert_string_equal(expected_order_2, order);
  assert_string_equal(expected_indices_2, indices);
  assert_memory_equal(zilch, order + 3, sizeof (order) - 3);
  assert_memory_equal(zilch, indices + 3, sizeof (indices) - 3);

  list_del(l);

  assert_non_null(l = list_new_kind(kind));

  list_push(l, (void *) 1);
  list_push(l, (void *) 2);
  list_push(l, (void *) 3);
  list_push(l, (void *) 4);
  list_push(l, (void *) 5);

  assert_int_equal(5, l->size);

  order[0] = indices[0] = 'a';
  order[1] = indices[1] = 'b';
  order[2] = indices[2] = 'c';
  order[3] = indices[3] = 'd';

  char expected_order_3[6] = "a3c1";
  char expected_indices_3[6] = "a1c3";

  assert_int_equal(
      2, list_discard_all(l, &value_is_odd, &record_order_r_stop));

  assert_int_equal(3, l->size);
  assert_string_equal(expected_order_3, order);
  assert_string_equal(expected_indices_3, indices);
  assert_memory_equal(zilch, order + 4, sizeof (order) - 4);
  assert_memory_equal(zilch, indices + 4, sizeof (indices) - 4);

  list_del(l);
}
#endif

TESTCALL(test_arlist_discard_all, do_test_list_discard_all(LISTKIND_ARRAY))
TESTCALL(test_deque_discard_all, do_test_list_discard_all(LISTKIND_DEQUE))
TESTCALL(test_dllist_discard_all, do_test_list_discard_all(LISTKIND_DLINK))
TESTCALL(test_sllist_discard_all, do_test_list_discard_all(LISTKIND_SLINK))

#ifndef METHODS_ONLY
static size_t stop_point = 0;

int record_and_stop(void *data, size_t index) {
  record_order(data, index);
  return data == (void *) stop_point;
}

int record_and_istop(void *data, size_t index) {
  record_order(data, index);
  return index == stop_point;
}

static void _do_test_list_discard_all_prefix_1(list *l, int push) {
  char expected_order[6] = "12345";
  char expected_indices[6] = "01234";

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  if (push) {
    list_push(l, (void *) 1);
    list_push(l, (void *) 2);
    list_push(l, (void *) 3);
    list_push(l, (void *) 4);
    list_push(l, (void *) 5);
  } else {
    list_unshift(l, (void *) 5);
    list_unshift(l, (void *) 4);
    list_unshift(l, (void *) 3);
    list_unshift(l, (void *) 2);
    list_unshift(l, (void *) 1);
  }

  assert_int_equal(5, l->size);

  assert_int_equal(
      stop_point, list_discard_all(l, &everything, &record_and_stop));

  assert_int_equal(5 - stop_point, l->size);

  expected_order[stop_point] = '\0';
  expected_indices[stop_point] = '\0';

  assert_string_equal(expected_order, order);
  assert_string_equal(expected_indices, indices);

  if (!l->size && l->kind == LISTKIND_DEQUE) {
    assert_int_equal(((deque *) l)->end, ((deque *) l)->start);
  }
}

void do_test_list_discard_all_prefix(listkind *kind) {
  list *l;
  stop_point = 1;

  do {
    assert_non_null(l = list_new_kind(kind));
    _do_test_list_discard_all_prefix_1(l, 1);
    list_del(l);
  } while (++stop_point <= 5);

  stop_point = 1;

  do {
    assert_non_null(l = list_new_kind(kind));
    _do_test_list_discard_all_prefix_1(l, 0);
    list_del(l);
  } while (++stop_point <= 5);
}
#endif

TESTCALL(test_arlist_discard_all_prefix,
    do_test_list_discard_all_prefix(LISTKIND_ARRAY))
TESTCALL(test_deque_discard_all_prefix,
    do_test_list_discard_all_prefix(LISTKIND_DEQUE))
TESTCALL(test_dllist_discard_all_prefix,
    do_test_list_discard_all_prefix(LISTKIND_DLINK))
TESTCALL(test_sllist_discard_all_prefix,
    do_test_list_discard_all_prefix(LISTKIND_SLINK))

#ifndef METHODS_ONLY
size_t match_bits = 0;

int match_by_bits(void *value) {
  return match_bits & (1ULL << ((size_t) value - 1));
}

size_t expected_from_bits(
    size_t input_size, char *expected_value, char *expected_order) {
  size_t i;
  size_t expected_removals = 0;

  if (input_size) {
    for (i = 0; i < input_size; i++) {
      if (match_bits & (1ULL << i) && i <= stop_point) {
        // Matching means it will be removed
        *expected_order++ = (char) i + '1';
        ++expected_removals;
      } else {
        *expected_order++ = 'x';
        *expected_value++ = (char) i + '1';
      }
    }
  }

  *expected_value = *expected_order = '\0';
  return expected_removals;
}

char actual_value[9] = { 0 };

int pointers2digits(void *data, size_t index) {
  actual_value[index] = (char) (uintptr_t) data + '0';
  return 0;
}

void do_test_deque_discard_all_exhaustive() {
  char expected_value[9] = { 0 };
  char expected_order[9] = { 0 };

  deque dq = { 0 };
  dq.kind = LISTKIND_DEQUE;
  dq.capacity = 8;

  void *dq_items[8] = { 0 };
  dq.items = dq_items;

  size_t i, test_start, test_size;

  for (test_start = 0; test_start < 8; test_start++) {
    for (test_size = 0; test_size <= 8; test_size++) {
      for (stop_point = 0; stop_point <= dq.size; stop_point++) {
        for (match_bits = 0; match_bits < 1ULL << stop_point; match_bits++) {
          size_t expected_removals =
            expected_from_bits(test_size, expected_value, expected_order);

          for (i = 0; i < test_size; i++) {
            dq_items[(test_start + i) % dq.capacity] = (void *) (i + 1);
          }

          dq.start = test_start;
          dq.end = (test_start + test_size) % dq.capacity;
          dq.size = test_size;

          memset(actual_value, 0, sizeof (actual_value));
          memset(order, 'x', test_size * sizeof (char));
          memset(order + test_size, 0,
              sizeof (order) - sizeof (char) * (test_size + 1));

          assert_int_equal(expected_removals,
            list_discard_all((list *) &dq, &match_by_bits, &record_and_istop));
          assert_int_equal(test_size - expected_removals, dq.size);

          list_foreach((list *) &dq, &pointers2digits);

          assert_string_equal(expected_value, actual_value);
          assert_string_equal(expected_order, order);

          assert_int_equal((dq.start + dq.size) % dq.capacity, dq.end);
        }
      }
    }
  }
}
#endif

TESTCALL(
    test_deque_discard_all_exhaustive,
    do_test_deque_discard_all_exhaustive())

#ifndef METHODS_ONLY
static char alnum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

#define ALNUM_CHARS (sizeof (alnum) - 1)

int assert_alnum_deranged(void *data, size_t i) {
  char unexpected[2] = { alnum[i], '\0' };
  char actual[2] = { (char) (uintptr_t) data, '\0' };

  assert_string_not_equal(unexpected, actual);
  return 0;
}

void do_test_list_shuffle(listkind *kind) {
  list *l;
  size_t i, end;
  struct sllistnode *temp_node = NULL;

  fly_status = FLY_OK;
  list_shuffle(NULL);
  assert_fly_status(FLY_E_NULL_PTR);

  l = list_new_kind(kind);
  assert_non_null(l);
  assert_fly_status(FLY_OK);

  if (kind == LISTKIND_DLINK || kind == LISTKIND_SLINK) {
    temp_node = ((sllist *) l)->head;
  }

  // This seed shuffles all alnum into different positions (derangement)
#ifdef _MSC_VER
  if (kind == LISTKIND_ARRAY || kind == LISTKIND_DEQUE) {
    rng64_set_seed(&l->rng, rng_seed64_make64(893, 1431, 9600, 10664));
  } else {
    rng64_set_seed(&l->rng, rng_seed64_make64(9600, 10664, 893, 1431));
  }
#else
  rng64_set_seed(&l->rng, rng_seed64_make64(893, 1431, 9600, 10664));
#endif

  for (i = 0; i < sizeof (alnum) - 1; ++i) {
    list_push(l, (void *) (uintptr_t) alnum[i]);
  }

  assert_int_equal(sizeof (alnum) - 1, l->size);

  fly_status = FLY_E_TOO_BIG;
  list_shuffle(l);

  assert_fly_status(FLY_OK);
  assert_int_equal(sizeof (alnum) - 1, l->size);

  if (kind == LISTKIND_DEQUE) {
    end = ((deque *) l)->end;

    if (!end) {
      end = l->size;
    }

    assert_int_equal(0, ((deque *) l)->start);
    assert_int_equal(l->size, end - ((deque *) l)->start);
  } else if (kind == LISTKIND_DLINK || kind == LISTKIND_SLINK) {
    assert_ptr_equal(temp_node, ((sllist *) l)->head);
  }

  list_foreach((list *) l, &assert_alnum_deranged);

  void *first = list_get(l, 0);

  for (i = 0; i < 4; ++i) {
    list_unshift(l, list_pop(l));
  }

  list_shuffle(l);

  assert_int_equal(sizeof (alnum) - 1, l->size);
  assert_ptr_not_equal(first, list_get(l, 0));

  first = list_get(l, 0);

  if (kind == LISTKIND_DEQUE) {
    end = ((deque *) l)->end;

    if (!end) {
      end = l->size;
    }

    assert_int_equal(0, ((deque *) l)->start);
    assert_int_equal(l->size, end - ((deque *) l)->start);
  } else if (kind == LISTKIND_DLINK || kind == LISTKIND_SLINK) {
    assert_ptr_equal(temp_node, ((sllist *) l)->head);
  }

  for (i = 0; i < (sizeof (alnum) >> 1) + (sizeof (alnum) >> 2); ++i) {
    void *p = list_pop(l);
    assert_non_null(p);
    assert_fly_status(FLY_OK);

    list_unshift(l, p);
    assert_fly_status(FLY_OK);
  }

  list_shuffle(l);

  assert_int_equal(sizeof (alnum) - 1, l->size);
  assert_ptr_not_equal(first, list_get(l, 0));

  first = list_get(l, 0);

  if (kind == LISTKIND_DEQUE) {
    end = ((deque *) l)->end;

    if (!end) {
      end = l->size;
    }

    assert_int_not_equal(0, ((deque *) l)->start);
    assert_int_equal(l->size, ((deque *) l)->capacity - ((deque *) l)->start);
  } else if (kind == LISTKIND_DLINK || kind == LISTKIND_SLINK) {
    assert_ptr_equal(temp_node, ((sllist *) l)->head);
  }

  list_del(l);
}
#endif

TESTCALL(test_arlist_shuffle, do_test_list_shuffle(LISTKIND_ARRAY))
TESTCALL(test_deque_shuffle, do_test_list_shuffle(LISTKIND_DEQUE))
TESTCALL(test_dllist_shuffle, do_test_list_shuffle(LISTKIND_DLINK))
TESTCALL(test_sllist_shuffle, do_test_list_shuffle(LISTKIND_SLINK))

#ifndef METHODS_ONLY
void assert_sizechar_seen(
    int *seen, size_t pos, size_t c,
    size_t iteration, char *expected, char *actual) {
  size_t i;
  char ch = (char) c;
  char markers[128];

  if (seen[pos] != 1) {
    testlog("'%c' (pos %zu) seen %d times on iteration %zu\n",
        ch, pos, seen[pos], iteration);
    testlog("expected %s\n", expected);
    testlog("actual   %s\n", actual);

    for (i = 0; i < ALNUM_CHARS; ++i) {
      markers[i] = ch == actual[i] ? '^' : ' ';
    }
    markers[i] = 0;

    testlog("         %s\n", markers);
  }

  assert_int_equal(1, seen[pos]);
}

void do_test_list_draw(listkind *kind) {
  size_t i;
  size_t iteration = 0;

  void **cur;
  void *expected[ALNUM_CHARS];
  void *actual[ALNUM_CHARS];

  char expected_str[ALNUM_CHARS + 1] = { 0 };
  char actual_str[ALNUM_CHARS + 1] = { 0 };

  void **(*draw)(list *, void **);

  list *l = list_new_kind(kind);
  assert_non_null(l);

  deque *dq = (deque *) l;  // convenient alias

  for (i = 0; i < ALNUM_CHARS; ++i) {
    list_push(l, (void *) (uintptr_t) alnum[i]);
  }

  if (kind == LISTKIND_ARRAY) {
    draw = (void *) arlist_draw;
  } else if (kind == LISTKIND_DEQUE) {
    draw = (void *) deque_draw;
  } else {
    list_del(l);
    _fail(__FILE__, __LINE__);
    draw = NULL;  // shouldn't get here but makes MSVC happy
  }

repeat_deque:
  rng64_set_seed(&l->rng, rng_seed64_make64(12495, 12195, 12395, 41396));

  list_shuffle(l);

  i = ALNUM_CHARS;

  do {
    expected[--i] = list_pop(l);
  } while (i > 0);

  assert_int_equal(0, l->size);

  for (i = 0; i < ALNUM_CHARS; ++i) {
    list_push(l, (void *) (uintptr_t) alnum[i]);
  }

  // go back in time
  rng64_set_seed(&l->rng, rng_seed64_make64(12495, 12195, 12395, 41396));

  if (iteration) {
    if (iteration < 5) {
      memcpy(dq->items + dq->end, dq->items, iteration * sizeof (void *));
      dq->start += iteration;
      dq->end += iteration;
    } else {
      dq->start = dq->capacity - (iteration - 4);
      dq->end = dq->size - (iteration - 4);

      for (i = 0; i < ALNUM_CHARS; i++) {
        dq->items[(dq->start + i) % dq->capacity] =
          (void *) (uintptr_t) alnum[i];
      }
    }
  }

  i = ALNUM_CHARS;

  for (cur = draw(l, NULL); cur; cur = draw(l, cur)) {
    actual[--i] = *cur;
    assert_int_equal(ALNUM_CHARS, l->size);
    assert_true(cur >= ((arlist *) l)->items);
  }

  size_t pos['Z' + 1] = { 0 };

  for (i = 0; i < ALNUM_CHARS; ++i) {
    pos[(size_t) alnum[i]] = i;
  }

  int seen[ALNUM_CHARS] = { 0 };

  for (i = 0; i < ALNUM_CHARS; ++i) {
    expected_str[i] = (char) (uintptr_t) expected[i];
    actual_str[i] = (char) (uintptr_t) actual[i];

    seen[pos[(size_t) actual[i]]] += 1;
  }

  for (i = 0; i < ALNUM_CHARS; ++i) {
    assert_sizechar_seen(
        seen, pos[(size_t) actual[i]], (size_t) actual[i],
        iteration, expected_str, actual_str);
  }

  expected_str[i] = actual_str[i] = 0;

  assert_string_equal(expected_str, actual_str);
  assert_memory_equal(expected, actual, ALNUM_CHARS);

  if (kind == LISTKIND_DEQUE) {
    if (++iteration >= 5) {
      if (iteration >= 10) {
        goto end;
      } else {
        dq->start = iteration - 5;
      }
    } else {
      dq->start = dq->capacity - iteration;
    }

    dq->end = dq->start;
    dq->size = 0;

    for (i = 0; i < ALNUM_CHARS; ++i) {
      deque_push(dq, (void *) (uintptr_t) alnum[i]);
    }

    memset(pos, 0, sizeof (pos));
    memset(seen, 0, sizeof (seen));

    goto repeat_deque;
  }

end:
  list_del(l);
}

void do_test_list_draw_empty(listkind *kind) {
  list *l = list_new_kind(kind);
  void **p;

  fly_status = FLY_OK;

  if (kind == LISTKIND_ARRAY) {
    p = arlist_draw((arlist *) l, NULL);
  } else {
    p = deque_draw((deque *) l, NULL);
  }

  assert_null(p);
  assert_fly_status(FLY_EMPTY);

  p = (void **) &p;
  fly_status = FLY_OK;

  if (kind == LISTKIND_ARRAY) {
    p = arlist_draw((arlist *) l, p);
  } else {
    p = deque_draw((deque *) l, p);
  }

  assert_null(p);
  assert_fly_status(FLY_EMPTY);

  list_del(l);
}
#endif

TESTCALL(test_arlist_draw, do_test_list_draw(LISTKIND_ARRAY))
TESTCALL(test_deque_draw, do_test_list_draw(LISTKIND_DEQUE))
TESTCALL(test_arlist_draw_empty, do_test_list_draw_empty(LISTKIND_ARRAY))
TESTCALL(test_deque_draw_empty, do_test_list_draw_empty(LISTKIND_DEQUE))

#ifndef METHODS_ONLY
void do_test_list_pick(listkind *kind) {
  size_t i;

  list *l = list_new_kind(kind);
  assert_non_null(l);

  uintptr_t (*pick)(void *l);

  if (kind == LISTKIND_ARRAY) {
    pick = (void *) &arlist_pick;
  } else if (kind == LISTKIND_DEQUE) {
    pick = (void *) &deque_pick;
  } else {
    list_del(l);
    _fail(__FILE__, __LINE__);
    pick = NULL;
  }

  for (i = 0; i < 16; i++) {
    list_push(l, (void *) (i + 1));
  }

  rng64_set_seed(&l->rng, rng_seed64_make64(875306, 823230, 776408, 708401));

#ifdef _MSC_VER
  const uintptr_t last = 13;
  uintptr_t sequence[] = {
       2,  3,  4,  4, 13,  3, 12,  7, 12, 13, 14, 10, 12,  6,
      14,  6,  2,  7,  8, 14, 12,  1, 14,  3, 12,  9, 15,  7,
       7,  4,  2, 16,  3,  3, 11, 15, 11, 12, 10, 10,  5,  0
  };
#else
  const uintptr_t last = 8;
  uintptr_t sequence[] = {
      11, 11,  7,  8,  9, 10,  2,  1, 14,  8,  7,  8,  3,  9,
      15, 15,  1,  9, 12,  7,  1,  1,  3, 14, 14, 14, 12, 14,
      15,  4,  2,  9, 14, 11, 11, 10, 16,  9, 16,  9,  1, 10,
       5, 14,  3,  5,  9,  7,  1, 12, 16, 13,  6,  7, 14,  0,
  };
#endif

  for (i = 0; i < sizeof (sequence) / sizeof (uintptr_t) - 1; ++i) {
    assert_int_equal(sequence[i], pick(l));
  }

  assert_int_equal(0, sequence[i]);  // ensure sequence ends correctly
  assert_int_equal(last, pick(l));

  list_del(l);
}
#endif

TESTCALL(test_arlist_pick, do_test_list_pick(LISTKIND_ARRAY))
TESTCALL(test_deque_pick, do_test_list_pick(LISTKIND_DEQUE))

#ifndef METHODS_ONLY
void do_test_list_e_null_ptr() {
  list *l = list_new();
  assert_non_null(l);
  assert_fly_status(FLY_OK);

  list_del(NULL);
  assert_fly_status(FLY_E_NULL_PTR);

  list_push(l, (void *) 0x1);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  list_push(NULL, (void *) 0x2);
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_E_NULL_PTR);

  assert_int_equal(0x1, (uintptr_t) list_get(l, 0));
  assert_fly_status(FLY_OK);

  assert_null(list_get(NULL, 0));
  assert_fly_status(FLY_E_NULL_PTR);

  list_unshift(l, (void *) 0x3);
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_pop(NULL));
  assert_int_equal(2, l->size);
  assert_fly_status(FLY_E_NULL_PTR);

  assert_int_equal(0x1, (uintptr_t) list_pop(l));
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_OK);

  assert_null(list_shift(NULL));
  assert_int_equal(1, l->size);
  assert_fly_status(FLY_E_NULL_PTR);

  assert_int_equal(0x3, (uintptr_t) list_shift(l));
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_OK);

  list_unshift(NULL, (void *) 0x4);
  assert_int_equal(0, l->size);
  assert_fly_status(FLY_E_NULL_PTR);

  list_del(l);
  assert_fly_status(FLY_OK);
}
#endif

TESTCALL(test_list_e_null_ptr, do_test_list_e_null_ptr())

#undef ARLIST_DEFAULT_CAPACITY

#ifndef _WINDLL
int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_list_new),
      cmocka_unit_test(test_arlist_push_pop),
      cmocka_unit_test(test_arlist_unshift_shift),
      cmocka_unit_test(test_arlist_push_shift),
      cmocka_unit_test(test_arlist_unshift_pop),
      cmocka_unit_test(test_arlist_pop_empty),
      cmocka_unit_test(test_arlist_shift_empty),
      cmocka_unit_test(test_arlist_del_nonempty),
      cmocka_unit_test(test_deque_push_pop),
      cmocka_unit_test(test_deque_unshift_shift),
      cmocka_unit_test(test_deque_push_shift),
      cmocka_unit_test(test_deque_unshift_pop),
      cmocka_unit_test(test_deque_pop_empty),
      cmocka_unit_test(test_deque_shift_empty),
      cmocka_unit_test(test_deque_del_nonempty),
      cmocka_unit_test(test_dllist_push_pop),
      cmocka_unit_test(test_dllist_unshift_shift),
      cmocka_unit_test(test_dllist_push_shift),
      cmocka_unit_test(test_dllist_unshift_pop),
      cmocka_unit_test(test_dllist_pop_empty),
      cmocka_unit_test(test_dllist_shift_empty),
      cmocka_unit_test(test_dllist_del_nonempty),
      cmocka_unit_test(test_sllist_push_pop),
      cmocka_unit_test(test_sllist_unshift_shift),
      cmocka_unit_test(test_sllist_push_shift),
      cmocka_unit_test(test_sllist_unshift_pop),
      cmocka_unit_test(test_sllist_pop_empty),
      cmocka_unit_test(test_sllist_shift_empty),
      cmocka_unit_test(test_sllist_del_nonempty),
      cmocka_unit_test(test_arlist_get),
      cmocka_unit_test(test_deque_get),
      cmocka_unit_test(test_dllist_get),
      cmocka_unit_test(test_sllist_get),
      cmocka_unit_test(test_arlist_capacity),
      cmocka_unit_test(test_deque_capacity),
      cmocka_unit_test(test_list_concat),
      cmocka_unit_test(test_list_concat_from_empty),
      cmocka_unit_test(test_list_concat_into_empty),
      cmocka_unit_test(test_list_concat_both_empty),
      cmocka_unit_test(test_arlist_append_array),
      cmocka_unit_test(test_deque_append_array),
      cmocka_unit_test(test_dllist_append_array),
      cmocka_unit_test(test_sllist_append_array),
      cmocka_unit_test(test_arlist_find_first),
      cmocka_unit_test(test_deque_find_first),
      cmocka_unit_test(test_dllist_find_first),
      cmocka_unit_test(test_sllist_find_first),
      cmocka_unit_test(test_arlist_find_first_null),
      cmocka_unit_test(test_deque_find_first_null),
      cmocka_unit_test(test_dllist_find_first_null),
      cmocka_unit_test(test_sllist_find_first_null),
      cmocka_unit_test(test_arlist_discard),
      cmocka_unit_test(test_deque_discard),
      cmocka_unit_test(test_dllist_discard),
      cmocka_unit_test(test_sllist_discard),
      cmocka_unit_test(test_arlist_discard_null),
      cmocka_unit_test(test_deque_discard_null),
      cmocka_unit_test(test_dllist_discard_null),
      cmocka_unit_test(test_sllist_discard_null),
      cmocka_unit_test(test_arlist_foreach),
      cmocka_unit_test(test_deque_foreach),
      cmocka_unit_test(test_dllist_foreach),
      cmocka_unit_test(test_sllist_foreach),
      cmocka_unit_test(test_arlist_discard_all),
      cmocka_unit_test(test_deque_discard_all),
      cmocka_unit_test(test_dllist_discard_all),
      cmocka_unit_test(test_sllist_discard_all),
      cmocka_unit_test(test_arlist_discard_all_prefix),
      cmocka_unit_test(test_deque_discard_all_prefix),
      cmocka_unit_test(test_dllist_discard_all_prefix),
      cmocka_unit_test(test_sllist_discard_all_prefix),
      cmocka_unit_test(test_deque_discard_all_exhaustive),
      cmocka_unit_test(test_arlist_shuffle),
      cmocka_unit_test(test_deque_shuffle),
      cmocka_unit_test(test_dllist_shuffle),
      cmocka_unit_test(test_sllist_shuffle),
      cmocka_unit_test(test_arlist_draw),
      cmocka_unit_test(test_deque_draw),
      cmocka_unit_test(test_arlist_draw_empty),
      cmocka_unit_test(test_deque_draw_empty),
      cmocka_unit_test(test_arlist_pick),
      cmocka_unit_test(test_deque_pick),
      cmocka_unit_test(test_list_e_null_ptr),
  };

  return cmocka_run_group_tests_name(
      "flytools list", tests, list_setup, list_teardown);
}
#endif
