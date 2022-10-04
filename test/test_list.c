#include "tests.h"

#include "list.h"


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
  list_del(l);
})

#ifndef METHODS_ONLY
void do_test_list_push_pop(listkind *kind) {
  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new_kind(kind);
  assert_non_null(l);

  list_push(l, data[0]);
  assert_int_equal(1, l->size);
  list_push(l, data[1]);
  assert_int_equal(2, l->size);
  list_push(l, data[2]);
  assert_int_equal(3, l->size);
  list_push(l, data[3]);
  assert_int_equal(4, l->size);
  list_push(l, data[4]);
  assert_int_equal(5, l->size);

  char *value;

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("fifth", value);
  assert_int_equal(4, l->size);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("fourth", value);
  assert_int_equal(3, l->size);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("third", value);
  assert_int_equal(2, l->size);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("second", value);
  assert_int_equal(1, l->size);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("first", value);
  assert_int_equal(0, l->size);

  list_del(l);
}

void do_test_list_unshift_shift(listkind *kind) {
  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new_kind(kind);
  assert_non_null(l);

  list_unshift(l, data[0]);
  assert_int_equal(1, l->size);
  list_unshift(l, data[1]);
  assert_int_equal(2, l->size);
  list_unshift(l, data[2]);
  assert_int_equal(3, l->size);
  list_unshift(l, data[3]);
  assert_int_equal(4, l->size);
  list_unshift(l, data[4]);
  assert_int_equal(5, l->size);

  char *value;

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("fifth", value);
  assert_int_equal(4, l->size);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("fourth", value);
  assert_int_equal(3, l->size);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("third", value);
  assert_int_equal(2, l->size);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("second", value);
  assert_int_equal(1, l->size);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("first", value);
  assert_int_equal(0, l->size);

  list_del(l);
}

void do_test_list_push_shift(listkind *kind) {
  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new_kind(kind);
  assert_non_null(l);

  list_push(l, data[0]);
  assert_int_equal(1, l->size);
  list_push(l, data[1]);
  assert_int_equal(2, l->size);
  list_push(l, data[2]);
  assert_int_equal(3, l->size);
  list_push(l, data[3]);
  assert_int_equal(4, l->size);
  list_push(l, data[4]);
  assert_int_equal(5, l->size);

  char *value;

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("first", value);
  assert_int_equal(4, l->size);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("second", value);
  assert_int_equal(3, l->size);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("third", value);
  assert_int_equal(2, l->size);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("fourth", value);
  assert_int_equal(1, l->size);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal("fifth", value);
  assert_int_equal(0, l->size);

  list_del(l);
}

void do_test_list_unshift_pop(listkind *kind) {
  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new_kind(kind);
  assert_non_null(l);

  list_unshift(l, data[0]);
  assert_int_equal(1, l->size);
  list_unshift(l, data[1]);
  assert_int_equal(2, l->size);
  list_unshift(l, data[2]);
  assert_int_equal(3, l->size);
  list_unshift(l, data[3]);
  assert_int_equal(4, l->size);
  list_unshift(l, data[4]);
  assert_int_equal(5, l->size);

  char *value;

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("first", value);
  assert_int_equal(4, l->size);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("second", value);
  assert_int_equal(3, l->size);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("third", value);
  assert_int_equal(2, l->size);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("fourth", value);
  assert_int_equal(1, l->size);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal("fifth", value);
  assert_int_equal(0, l->size);

  list_del(l);
}
#endif

TESTCALL(test_arlist_push_pop, do_test_list_push_pop(LISTKIND_ARRAY))
TESTCALL(test_arlist_unshift_shift, do_test_list_unshift_shift(LISTKIND_ARRAY))
TESTCALL(test_arlist_push_shift, do_test_list_push_shift(LISTKIND_ARRAY))
TESTCALL(test_arlist_unshift_pop, do_test_list_unshift_pop(LISTKIND_ARRAY))

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

  assert_int_equal(0, l->size);
  assert_null(list_pop(l));
  assert_int_equal(0, l->size);

  list_push(l, l);
  assert_int_equal(1, l->size);
  assert_ptr_equal(l, list_pop(l));
  assert_int_equal(0, l->size);
  assert_null(list_pop(l));

  list_push(l, l);
  assert_int_equal(1, l->size);
  assert_ptr_equal(l, list_pop(l));
  assert_int_equal(0, l->size);
  assert_null(list_pop(l));

  list_del(l);
}

void do_test_list_shift_empty(listkind *kind) {
  list *l = list_new_kind(kind);
  assert_non_null(l);

  assert_int_equal(0, l->size);
  assert_null(list_shift(l));
  assert_int_equal(0, l->size);

  list_unshift(l, l);
  assert_int_equal(1, l->size);
  assert_ptr_equal(l, list_shift(l));
  assert_int_equal(0, l->size);
  assert_null(list_shift(l));

  list_unshift(l, l);
  assert_int_equal(1, l->size);
  assert_ptr_equal(l, list_shift(l));
  assert_int_equal(0, l->size);
  assert_null(list_shift(l));

  list_del(l);
}

void do_test_list_del_nonempty(listkind *kind) {
  list *l = list_new_kind(kind);
  assert_non_null(l);

  list_push(l, l);
  list_push(l, l);
  list_push(l, l);

  assert_int_equal(3, l->size);

  list_del(l);
}
#endif

TESTCALL(test_arlist_pop_empty, do_test_list_pop_empty(LISTKIND_ARRAY))
TESTCALL(test_arlist_shift_empty, do_test_list_shift_empty(LISTKIND_ARRAY))
TESTCALL(test_arlist_del_nonempty, do_test_list_del_nonempty(LISTKIND_ARRAY))

TESTCALL(test_dllist_pop_empty, do_test_list_pop_empty(LISTKIND_DLINK))
TESTCALL(test_dllist_shift_empty, do_test_list_shift_empty(LISTKIND_DLINK))
TESTCALL(test_dllist_del_nonempty, do_test_list_del_nonempty(LISTKIND_DLINK))

TESTCALL(test_sllist_pop_empty, do_test_list_pop_empty(LISTKIND_SLINK))
TESTCALL(test_sllist_shift_empty, do_test_list_shift_empty(LISTKIND_SLINK))
TESTCALL(test_sllist_del_nonempty, do_test_list_del_nonempty(LISTKIND_SLINK))

#ifndef METHODS_ONLY
void do_test_list_get(listkind *kind) {
  list *l;
  size_t i = 0;

  assert_non_null(l = list_new_kind(kind));
  assert_int_equal(0, l->size);

  assert_null(list_get(l, 0));
  assert_null(list_get(l, 1));
  assert_null(list_get(l, SIZE_MAX));
  assert_null(list_get(l, l->size));

  if (l->kind == LISTKIND_ARRAY) {
    assert_null(list_get(l, ((arlist *) l)->capacity));
  }

  list_push(l, (void *) 123);
  assert_int_equal(1, l->size);

  assert_int_equal(123, (uintptr_t) list_get(l, 0));
  assert_null(list_get(l, 1));
  assert_null(list_get(l, SIZE_MAX));
  assert_null(list_get(l, l->size));

  if (l->kind == LISTKIND_ARRAY) {
    assert_null(list_get(l, ((arlist *) l)->capacity));
  }

  list_unshift(l, (void *) 456);
  assert_int_equal(2, l->size);

  assert_int_equal(456, (uintptr_t) list_get(l, 0));
  assert_int_equal(123, (uintptr_t) list_get(l, 1));
  assert_null(list_get(l, 2));
  assert_null(list_get(l, SIZE_MAX));
  assert_null(list_get(l, l->size));

  if (l->kind == LISTKIND_ARRAY) {
    assert_null(list_get(l, ((arlist *) l)->capacity));

    while (l->size < ((arlist *) l)->capacity) {
      list_unshift(l, (void *) 10664);
    }

    assert_int_equal(10664, (uintptr_t) list_get(l, 0));
    assert_int_equal(456, (uintptr_t) list_get(l, l->size - 2));
    assert_int_equal(123, (uintptr_t) list_get(l, l->size - 1));
    assert_null(list_get(l, SIZE_MAX));
    assert_null(list_get(l, l->size));
    assert_null(list_get(l, l->size + 1));
  }

  while (l->size) {
    list_pop(l);
  }

  assert_null(list_get(l, 0));
  assert_null(list_get(l, 1));
  assert_null(list_get(l, SIZE_MAX));
  assert_null(list_get(l, l->size));

  if (l->kind == LISTKIND_ARRAY) {
    assert_null(list_get(l, ((arlist *) l)->capacity));
  }

  list_del(l);
}
#endif

TESTCALL(test_arlist_get, do_test_list_get(LISTKIND_ARRAY))
TESTCALL(test_dllist_get, do_test_list_get(LISTKIND_DLINK))
TESTCALL(test_sllist_get, do_test_list_get(LISTKIND_SLINK))

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

static void do_test_list_find_first(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  assert_null(list_find_first(l, &char0_is_f));
  assert_int_equal(0, l->size);

  list_push(l, "first");
  list_push(l, "second");
  list_push(l, "third");
  list_push(l, "fourth");
  list_push(l, "fifth");
  list_push(l, "sixth");
  list_push(l, "seventh");

  assert_int_equal(7, l->size);

  char *value;

  assert_non_null(value = list_find_first(l, &char0_is_f));
  assert_string_equal("first", value);
  assert_non_null(value = list_find_first(l, &char1_is_e));
  assert_string_equal("second", value);
  assert_non_null(value = list_find_first(l, &char3_is_r));
  assert_string_equal("third", value);
  assert_non_null(value = list_find_first(l, &char2_is_x));
  assert_string_equal("sixth", value);
  assert_non_null(value = list_find_first(l, &char2_is_v));
  assert_string_equal("seventh", value);

  assert_null(list_find_first(l, &char0_is_e));

  list_del(l);
}
#endif

TESTCALL(test_arlist_find_first, do_test_list_find_first(LISTKIND_ARRAY))
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
#endif

#ifndef METHODS_ONLY
static void do_test_list_find_first_null(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  list_unshift(l, "three");
  list_unshift(l, "two");
  list_unshift(l, NULL);
  list_unshift(l, "one");

  assert_int_equal(4, l->size);

  char *value;

  value_was_null = 0;

  assert_non_null(value = list_find_first(l, &char0_is_t));
  assert_string_equal("two", value);
  assert_int_equal(4, l->size);
  assert_non_null(value = list_find_first(l, &char0_is_o));
  assert_string_equal("one", value);
  assert_int_equal(4, l->size);

  assert_null(list_find_first(l, &value_is_null));
  assert_int_equal(4, l->size);

  assert_non_null(value = list_find_first(l, &value_after_null));
  assert_string_equal("two", value);
  assert_int_equal(4, l->size);

  list_shift(l);
  list_shift(l);
  list_unshift(l, "one");
  assert_int_equal(3, l->size);

  assert_null(list_find_first(l, &value_after_null));
  assert_int_equal(3, l->size);

  list_del(l);
}
#endif

TESTCALL(test_arlist_find_first_null,
    do_test_list_find_first_null(LISTKIND_ARRAY))
TESTCALL(test_dllist_find_first_null,
    do_test_list_find_first_null(LISTKIND_DLINK))
TESTCALL(test_sllist_find_first_null,
    do_test_list_find_first_null(LISTKIND_SLINK))

#ifndef METHODS_ONLY
static void do_test_list_remove_first(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  assert_null(list_remove_first(l, &char0_is_f));
  assert_int_equal(0, l->size);

  list_push(l, "first");
  list_push(l, "second");
  list_push(l, "third");
  list_push(l, "fourth");
  list_push(l, "fifth");
  list_push(l, "sixth");
  list_push(l, "seventh");

  assert_int_equal(7, l->size);

  char *value;

  assert_non_null(value = list_remove_first(l, &char0_is_f));
  assert_string_equal("first", value);
  assert_int_equal(6, l->size);
  assert_non_null(value = list_remove_first(l, &char0_is_f));
  assert_string_equal("fourth", value);
  assert_int_equal(5, l->size);
  assert_non_null(value = list_remove_first(l, &char2_is_v));
  assert_string_equal("seventh", value);
  assert_int_equal(4, l->size);
  assert_non_null(value = list_remove_first(l, &char2_is_x));
  assert_string_equal("sixth", value);
  assert_int_equal(3, l->size);
  assert_non_null(value = list_remove_first(l, &char1_is_e));
  assert_string_equal("second", value);
  assert_int_equal(2, l->size);
  assert_non_null(value = list_remove_first(l, &char0_is_f));
  assert_string_equal("fifth", value);
  assert_int_equal(1, l->size);
  assert_non_null(value = list_remove_first(l, &char3_is_r));
  assert_string_equal("third", value);
  assert_int_equal(0, l->size);

  assert_null(list_remove_first(l, &char0_is_e));
  assert_int_equal(0, l->size);
  assert_null(list_remove_first(l, &char0_is_f));
  assert_int_equal(0, l->size);

  list_del(l);
}
#endif

TESTCALL(test_arlist_remove_first, do_test_list_remove_first(LISTKIND_ARRAY))
TESTCALL(test_dllist_remove_first, do_test_list_remove_first(LISTKIND_DLINK))
TESTCALL(test_sllist_remove_first, do_test_list_remove_first(LISTKIND_SLINK))

#ifndef METHODS_ONLY
static void do_test_list_remove_first_null(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  list_unshift(l, "three");
  list_unshift(l, "two");
  list_unshift(l, NULL);
  list_unshift(l, "one");

  assert_int_equal(4, l->size);

  char *value;

  value_was_null = 0;

  assert_non_null(value = list_remove_first(l, &char0_is_t));
  assert_string_equal("two", value);
  assert_int_equal(3, l->size);
  assert_non_null(value = list_remove_first(l, &char0_is_o));
  assert_string_equal("one", value);
  assert_int_equal(2, l->size);

  assert_non_null(value = list_remove_first(l, &value_after_null));
  assert_string_equal("three", value);
  assert_int_equal(1, l->size);

  list_unshift(l, "one 2.0");
  list_push(l, "three 2.0");
  assert_int_equal(3, l->size);

  assert_null(list_remove_first(l, &value_is_null));
  assert_int_equal(2, l->size);

  assert_non_null(value = list_shift(l));
  assert_string_equal("one 2.0", value);
  assert_int_equal(1, l->size);
  assert_non_null(value = list_shift(l));
  assert_string_equal("three 2.0", value);
  assert_int_equal(0, l->size);

  assert_null(list_remove_first(l, &value_after_null));
  assert_int_equal(0, l->size);

  list_del(l);
}
#endif

TESTCALL(test_arlist_remove_first_null,
    do_test_list_remove_first_null(LISTKIND_ARRAY))
TESTCALL(test_dllist_remove_first_null,
    do_test_list_remove_first_null(LISTKIND_DLINK))
TESTCALL(test_sllist_remove_first_null,
    do_test_list_remove_first_null(LISTKIND_SLINK))

#ifndef METHODS_ONLY
static uintptr_t prime = 0;
static size_t index_sum = 0;
static char order[6] = { 0 }, indices[6] = { 0 }, zilch[6] = { 0 };

int multiply_prime(void *data, size_t index) {
  prime *= (uintptr_t) data;
  index_sum += index;
  return 0;
}

int record_order(void *data, size_t index) {
  order[index] = (char) data + '0';
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

  list_del(l);
}
#endif

TESTCALL(test_arlist_foreach, do_test_list_foreach(LISTKIND_ARRAY))
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
  order[count] = (char) data + '0';
  indices[count] = (char) index + '0';
  ++count;
  return 0;
}

void do_test_list_remove_all(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  prime = 1;
  index_sum = 2;

  assert_int_equal(
      0, list_remove_all(l, &everything, &multiply_prime));

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
      0, list_remove_all(l, &nothing, &multiply_prime));

  assert_int_equal(1, prime);
  assert_int_equal(2, index_sum);
  assert_int_equal(5, l->size);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  count = 0;

  char expected_order[6] = "135";
  char expected_indices[6] = "024";

  assert_int_equal(
      3, list_remove_all(l, &value_is_odd, &record_order_by_count));

  assert_int_equal(2, l->size);
  assert_string_equal(expected_order, order);
  assert_string_equal(expected_indices, indices);
  assert_memory_equal(zilch, order + 3, sizeof (order) - 3);
  assert_memory_equal(zilch, indices + 3, sizeof (indices) - 3);

  order[0] = expected_order[0] = 'a';
  indices[0] = expected_indices[0] = 'b';

  assert_int_equal(
      0, list_remove_all(l, &value_is_odd, &record_order_by_count));

  assert_int_equal(2, l->size);
  assert_string_equal(expected_order, order);
  assert_string_equal(expected_indices, indices);
  assert_memory_equal(zilch, order + 3, sizeof (order) - 3);
  assert_memory_equal(zilch, indices + 3, sizeof (indices) - 3);

  count = 0;

  char expected_order_2[6] = "245";
  char expected_indices_2[6] = "014";

  assert_int_equal(
      2, list_remove_all(l, &value_is_even, &record_order_by_count));

  assert_int_equal(0, l->size);
  assert_string_equal(expected_order_2, order);
  assert_string_equal(expected_indices_2, indices);
  assert_memory_equal(zilch, order + 3, sizeof (order) - 3);
  assert_memory_equal(zilch, indices + 3, sizeof (indices) - 3);

  assert_int_equal(
      0, list_remove_all(l, &value_is_even, &record_order_by_count));

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
      1, list_remove_all(l, &everything, &record_order_by_count));

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
      2, list_remove_all(l, &value_is_odd, &record_order_r_stop));

  assert_int_equal(3, l->size);
  assert_string_equal(expected_order_3, order);
  assert_string_equal(expected_indices_3, indices);
  assert_memory_equal(zilch, order + 4, sizeof (order) - 4);
  assert_memory_equal(zilch, indices + 4, sizeof (indices) - 4);

  list_del(l);
}
#endif

TESTCALL(test_arlist_remove_all, do_test_list_remove_all(LISTKIND_ARRAY))
TESTCALL(test_dllist_remove_all, do_test_list_remove_all(LISTKIND_DLINK))
TESTCALL(test_sllist_remove_all, do_test_list_remove_all(LISTKIND_SLINK))

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
      cmocka_unit_test(test_dllist_get),
      cmocka_unit_test(test_sllist_get),
      cmocka_unit_test(test_list_concat),
      cmocka_unit_test(test_list_concat_from_empty),
      cmocka_unit_test(test_list_concat_into_empty),
      cmocka_unit_test(test_list_concat_both_empty),
      cmocka_unit_test(test_arlist_find_first),
      cmocka_unit_test(test_dllist_find_first),
      cmocka_unit_test(test_sllist_find_first),
      cmocka_unit_test(test_arlist_find_first_null),
      cmocka_unit_test(test_dllist_find_first_null),
      cmocka_unit_test(test_sllist_find_first_null),
      cmocka_unit_test(test_arlist_remove_first),
      cmocka_unit_test(test_dllist_remove_first),
      cmocka_unit_test(test_sllist_remove_first),
      cmocka_unit_test(test_arlist_remove_first_null),
      cmocka_unit_test(test_dllist_remove_first_null),
      cmocka_unit_test(test_sllist_remove_first_null),
      cmocka_unit_test(test_arlist_foreach),
      cmocka_unit_test(test_dllist_foreach),
      cmocka_unit_test(test_sllist_foreach),
      cmocka_unit_test(test_arlist_remove_all),
      cmocka_unit_test(test_dllist_remove_all),
      cmocka_unit_test(test_sllist_remove_all),
  };

  return cmocka_run_group_tests_name(
      "flytools list", tests, list_setup, list_teardown);
}
#endif
