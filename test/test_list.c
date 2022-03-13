#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#include "list.h"


int list_setup(void **state) {
  (void) state;

  return 0;
}

int list_teardown(void **state) {
  (void) state;

  return 0;
}

void test_list_new(void **state) {
  (void) state;

  list *l = list_new();
  assert_non_null(l);
  assert_int_equal(list_size(l), 0);
  list_del(l);
}

void test_list_push_pop(void **state) {
  (void) state;

  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new();
  assert_non_null(l);

  list_push(l, data[0]);
  assert_int_equal(list_size(l), 1);
  list_push(l, data[1]);
  assert_int_equal(list_size(l), 2);
  list_push(l, data[2]);
  assert_int_equal(list_size(l), 3);
  list_push(l, data[3]);
  assert_int_equal(list_size(l), 4);
  list_push(l, data[4]);
  assert_int_equal(list_size(l), 5);

  char *value;

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "fifth");
  assert_int_equal(list_size(l), 4);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "fourth");
  assert_int_equal(list_size(l), 3);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "third");
  assert_int_equal(list_size(l), 2);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "second");
  assert_int_equal(list_size(l), 1);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "first");
  assert_int_equal(list_size(l), 0);

  list_del(l);
}

void test_list_unshift_shift(void **state) {
  (void) state;

  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new();
  assert_non_null(l);

  list_unshift(l, data[0]);
  assert_int_equal(list_size(l), 1);
  list_unshift(l, data[1]);
  assert_int_equal(list_size(l), 2);
  list_unshift(l, data[2]);
  assert_int_equal(list_size(l), 3);
  list_unshift(l, data[3]);
  assert_int_equal(list_size(l), 4);
  list_unshift(l, data[4]);
  assert_int_equal(list_size(l), 5);

  char *value;

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "fifth");
  assert_int_equal(list_size(l), 4);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "fourth");
  assert_int_equal(list_size(l), 3);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "third");
  assert_int_equal(list_size(l), 2);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "second");
  assert_int_equal(list_size(l), 1);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "first");
  assert_int_equal(list_size(l), 0);

  list_del(l);
}

void test_list_push_shift(void **state) {
  (void) state;

  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new();
  assert_non_null(l);

  list_push(l, data[0]);
  assert_int_equal(list_size(l), 1);
  list_push(l, data[1]);
  assert_int_equal(list_size(l), 2);
  list_push(l, data[2]);
  assert_int_equal(list_size(l), 3);
  list_push(l, data[3]);
  assert_int_equal(list_size(l), 4);
  list_push(l, data[4]);
  assert_int_equal(list_size(l), 5);

  char *value;

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "first");
  assert_int_equal(list_size(l), 4);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "second");
  assert_int_equal(list_size(l), 3);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "third");
  assert_int_equal(list_size(l), 2);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "fourth");
  assert_int_equal(list_size(l), 1);

  value = list_shift(l);
  assert_non_null(l);
  assert_string_equal(value, "fifth");
  assert_int_equal(list_size(l), 0);

  list_del(l);
}

void test_list_unshift_pop(void **state) {
  (void) state;

  static char *data[] = {
    "first", "second", "third", "fourth", "fifth"
  };

  list *l = list_new();
  assert_non_null(l);

  list_unshift(l, data[0]);
  assert_int_equal(list_size(l), 1);
  list_unshift(l, data[1]);
  assert_int_equal(list_size(l), 2);
  list_unshift(l, data[2]);
  assert_int_equal(list_size(l), 3);
  list_unshift(l, data[3]);
  assert_int_equal(list_size(l), 4);
  list_unshift(l, data[4]);
  assert_int_equal(list_size(l), 5);

  char *value;

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "first");
  assert_int_equal(list_size(l), 4);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "second");
  assert_int_equal(list_size(l), 3);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "third");
  assert_int_equal(list_size(l), 2);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "fourth");
  assert_int_equal(list_size(l), 1);

  value = list_pop(l);
  assert_non_null(l);
  assert_string_equal(value, "fifth");
  assert_int_equal(list_size(l), 0);

  list_del(l);
}

void test_list_pop_from_empty(void **state) {
  (void) state;

  list *l = list_new();
  assert_non_null(l);

  assert_int_equal(list_size(l), 0);
  assert_null(list_pop(l));
  assert_int_equal(list_size(l), 0);

  list_push(l, l);
  assert_int_equal(list_size(l), 1);
  assert_ptr_equal(list_pop(l), l);
  assert_int_equal(list_size(l), 0);
  assert_null(list_pop(l));

  list_push(l, l);
  assert_int_equal(list_size(l), 1);
  assert_ptr_equal(list_pop(l), l);
  assert_int_equal(list_size(l), 0);
  assert_null(list_pop(l));

  list_del(l);
}

void test_list_shift_from_empty(void **state) {
  (void) state;

  list *l = list_new();
  assert_non_null(l);

  assert_int_equal(list_size(l), 0);
  assert_null(list_shift(l));
  assert_int_equal(list_size(l), 0);

  list_unshift(l, l);
  assert_int_equal(list_size(l), 1);
  assert_ptr_equal(list_shift(l), l);
  assert_int_equal(list_size(l), 0);
  assert_null(list_shift(l));

  list_unshift(l, l);
  assert_int_equal(list_size(l), 1);
  assert_ptr_equal(list_shift(l), l);
  assert_int_equal(list_size(l), 0);
  assert_null(list_shift(l));

  list_del(l);
}

void test_list_del_nonempty(void **state) {
  (void) state;

  list *l = list_new();
  assert_non_null(l);

  list_push(l, l);
  list_push(l, l);
  list_push(l, l);

  assert_int_equal(list_size(l), 3);

  list_del(l);
}

void test_list_concat(void **state) {
  (void) state;

  static size_t data[] = {
    0x101, 0x102, 0x103, 0x201, 0x202, 0x203, 0x204
  };

  static int len_data = sizeof (data) / sizeof (size_t);

  list *l1, *l2;
  assert_non_null(l1 = list_new());
  assert_non_null(l2 = list_new());

  int i;

  for (i = 0; i < len_data / 2; i++) {
    list_unshift(l1, (void *) data[i]);
  }
  for (; i < len_data; i++) {
    list_unshift(l2, (void *) data[i]);
  }

  assert_int_equal(list_size(l1), 3);
  assert_int_equal(list_size(l2), 4);

  list_concat(l1, l2);

  while (i > 0) {
    assert_int_equal(list_size(l1), i);
    assert_ptr_equal(list_shift(l1), data[--i]);
  }

  assert_int_equal(list_size(l1), 0);

  list_del(l1);
}

void test_list_concat_from_empty(void **state) {
  (void) state;

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

  assert_int_equal(list_size(l1), 3);
  assert_int_equal(list_size(l2), 0);

  list_concat(l1, l2);

  while (i > 0) {
    assert_int_equal(list_size(l1), i);
    assert_ptr_equal(list_shift(l1), data[--i]);
  }

  assert_int_equal(list_size(l1), 0);

  list_del(l1);
}

void test_list_concat_into_empty(void **state) {
  (void) state;

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

  assert_int_equal(list_size(l1), 0);
  assert_int_equal(list_size(l2), 3);

  list_concat(l1, l2);

  while (i > 0) {
    assert_int_equal(list_size(l1), i);
    assert_ptr_equal(list_shift(l1), data[--i]);
  }

  assert_int_equal(list_size(l1), 0);

  list_del(l1);
}

void test_list_concat_both_empty(void **state) {
  (void) state;

  list *l1, *l2;
  assert_non_null(l1 = list_new());
  assert_non_null(l2 = list_new());

  assert_int_equal(list_size(l1), 0);
  assert_int_equal(list_size(l2), 0);

  list_concat(l1, l2);

  assert_int_equal(list_size(l1), 0);
  assert_null(list_pop(l1));
  assert_int_equal(list_size(l1), 0);

  list_del(l1);
}

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

static inline void __test_list_find_first(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  assert_null(list_find_first(l, &char0_is_f));
  assert_int_equal(list_size(l), 0);

  list_unshift(l, "first");
  list_unshift(l, "second");
  list_unshift(l, "third");
  list_unshift(l, "fourth");
  list_unshift(l, "fifth");
  list_unshift(l, "sixth");
  list_unshift(l, "seventh");

  assert_int_equal(list_size(l), 7);

  char *value;

  assert_non_null(value = list_find_first(l, &char0_is_f));
  assert_string_equal(value, "first");
  assert_non_null(value = list_find_first(l, &char1_is_e));
  assert_string_equal(value, "second");
  assert_non_null(value = list_find_first(l, &char3_is_r));
  assert_string_equal(value, "third");
  assert_non_null(value = list_find_first(l, &char2_is_x));
  assert_string_equal(value, "sixth");
  assert_non_null(value = list_find_first(l, &char2_is_v));
  assert_string_equal(value, "seventh");

  assert_null(list_find_first(l, &char0_is_e));

  list_del(l);
}

void test_list_find_first_dl(void **state) {
  __test_list_find_first(LISTKIND_DLINK);
}

void test_list_find_first_sl(void **state) {
  __test_list_find_first(LISTKIND_SLINK);
}

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

static inline void __test_list_find_first_null_entry(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  list_push(l, "three");
  list_push(l, "two");
  list_push(l, NULL);
  list_push(l, "one");

  assert_int_equal(list_size(l), 4);

  char *value;

  value_was_null = 0;

  assert_non_null(value = list_find_first(l, &char0_is_t));
  assert_string_equal(value, "two");
  assert_int_equal(list_size(l), 4);
  assert_non_null(value = list_find_first(l, &char0_is_o));
  assert_string_equal(value, "one");
  assert_int_equal(list_size(l), 4);

  assert_null(list_find_first(l, &value_is_null));
  assert_int_equal(list_size(l), 4);

  assert_non_null(value = list_find_first(l, &value_after_null));
  assert_string_equal(value, "two");
  assert_int_equal(list_size(l), 4);

  list_pop(l);
  list_pop(l);
  list_push(l, "one");
  assert_int_equal(list_size(l), 3);

  assert_null(list_find_first(l, &value_after_null));
  assert_int_equal(list_size(l), 3);

  list_del(l);
}

void test_list_find_first_null_entry_dl(void **state) {
  (void) state;
  __test_list_find_first_null_entry(LISTKIND_DLINK);
}

void test_list_find_first_null_entry_sl(void **state) {
  (void) state;
  __test_list_find_first_null_entry(LISTKIND_SLINK);
}

static inline void __test_list_remove_first(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  assert_null(list_remove_first(l, &char0_is_f));
  assert_int_equal(list_size(l), 0);

  list_unshift(l, "first");
  list_unshift(l, "second");
  list_unshift(l, "third");
  list_unshift(l, "fourth");
  list_unshift(l, "fifth");
  list_unshift(l, "sixth");
  list_unshift(l, "seventh");

  assert_int_equal(list_size(l), 7);

  char *value;

  assert_non_null(value = list_remove_first(l, &char0_is_f));
  assert_string_equal(value, "first");
  assert_int_equal(list_size(l), 6);
  assert_non_null(value = list_remove_first(l, &char0_is_f));
  assert_string_equal(value, "fourth");
  assert_int_equal(list_size(l), 5);
  assert_non_null(value = list_remove_first(l, &char2_is_v));
  assert_string_equal(value, "seventh");
  assert_int_equal(list_size(l), 4);
  assert_non_null(value = list_remove_first(l, &char2_is_x));
  assert_string_equal(value, "sixth");
  assert_int_equal(list_size(l), 3);
  assert_non_null(value = list_remove_first(l, &char1_is_e));
  assert_string_equal(value, "second");
  assert_int_equal(list_size(l), 2);
  assert_non_null(value = list_remove_first(l, &char0_is_f));
  assert_string_equal(value, "fifth");
  assert_int_equal(list_size(l), 1);
  assert_non_null(value = list_remove_first(l, &char3_is_r));
  assert_string_equal(value, "third");
  assert_int_equal(list_size(l), 0);

  assert_null(list_remove_first(l, &char0_is_e));
  assert_int_equal(list_size(l), 0);
  assert_null(list_remove_first(l, &char0_is_f));
  assert_int_equal(list_size(l), 0);

  list_del(l);
}

void test_list_remove_first_dl(void **state) {
  (void) state;
  __test_list_remove_first(LISTKIND_DLINK);
}

void test_list_remove_first_sl(void **state) {
  (void) state;
  __test_list_remove_first(LISTKIND_SLINK);
}

static inline void __test_list_remove_first_null_entry(listkind *kind) {
  list *l;
  assert_non_null(l = list_new_kind(kind));

  list_push(l, "three");
  list_push(l, "two");
  list_push(l, NULL);
  list_push(l, "one");

  assert_int_equal(list_size(l), 4);

  char *value;

  value_was_null = 0;

  assert_non_null(value = list_remove_first(l, &char0_is_t));
  assert_string_equal(value, "two");
  assert_int_equal(list_size(l), 3);
  assert_non_null(value = list_remove_first(l, &char0_is_o));
  assert_string_equal(value, "one");
  assert_int_equal(list_size(l), 2);

  assert_non_null(value = list_remove_first(l, &value_after_null));
  assert_string_equal(value, "three");
  assert_int_equal(list_size(l), 1);

  list_push(l, "one 2.0");
  list_unshift(l, "three 2.0");
  assert_int_equal(list_size(l), 3);

  assert_null(list_remove_first(l, &value_is_null));
  assert_int_equal(list_size(l), 2);

  assert_non_null(value = list_pop(l));
  assert_string_equal(value, "one 2.0");
  assert_int_equal(list_size(l), 1);
  assert_non_null(value = list_pop(l));
  assert_string_equal(value, "three 2.0");
  assert_int_equal(list_size(l), 0);

  assert_null(list_remove_first(l, &value_after_null));
  assert_int_equal(list_size(l), 0);

  list_del(l);
}

void test_list_remove_first_null_entry_dl(void **state) {
  (void) state;
  __test_list_remove_first_null_entry(LISTKIND_DLINK);
}

void test_list_remove_first_null_entry_sl(void **state) {
  (void) state;
  __test_list_remove_first_null_entry(LISTKIND_SLINK);
}

static uintptr_t prime = 1;
static size_t index_sum = 0;
static char order[6] = { 0 }, indices[6] = { 0 };

int multiply_prime(void *data, size_t index) {
  prime *= (uintptr_t) data;
  index_sum += index;
  return 0;
}

int record_order(void *data, size_t index) {
  order[index] = (uintptr_t) data + '0';
  indices[index] = index + '0';
  return 0;
}

int record_order_r_stop(void *data, size_t index) {
  record_order(data, 2 - index + 1);
  return index == 2;
}

void test_list_foreach(void **state) {
  (void) state;

  list *l;
  assert_non_null(l = list_new());

  list_foreach(l, &multiply_prime);

  assert_int_equal(prime, 1);
  assert_int_equal(index_sum, 0);
  assert_int_equal(list_size(l), 0);

  list_unshift(l, (void *) 1);
  list_unshift(l, (void *) 2);
  list_unshift(l, (void *) 3);
  list_unshift(l, (void *) 5);
  list_unshift(l, (void *) 7);

  list_foreach(l, &multiply_prime);

  assert_int_equal(prime, 210);
  assert_int_equal(index_sum, 10);
  assert_int_equal(list_size(l), 5);

  list_foreach(l, &record_order);

  assert_string_equal("12357", order);
  assert_string_equal("01234", indices);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  list_foreach(l, &record_order_r_stop);

  assert_int_equal(order[0], 0);
  assert_int_equal(indices[0], 0);

  order[0] = indices[0] = 'x';

  assert_string_equal("x321", order);
  assert_string_equal("x123", indices);

  memset(order, 0, sizeof (order));
  memset(indices, 0, sizeof (indices));

  list_del(l);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_list_new),
      cmocka_unit_test(test_list_push_pop),
      cmocka_unit_test(test_list_unshift_shift),
      cmocka_unit_test(test_list_push_shift),
      cmocka_unit_test(test_list_unshift_pop),
      cmocka_unit_test(test_list_pop_from_empty),
      cmocka_unit_test(test_list_shift_from_empty),
      cmocka_unit_test(test_list_del_nonempty),
      cmocka_unit_test(test_list_concat),
      cmocka_unit_test(test_list_concat_from_empty),
      cmocka_unit_test(test_list_concat_into_empty),
      cmocka_unit_test(test_list_concat_both_empty),
      cmocka_unit_test(test_list_find_first_dl),
      cmocka_unit_test(test_list_find_first_sl),
      cmocka_unit_test(test_list_find_first_null_entry_dl),
      cmocka_unit_test(test_list_find_first_null_entry_sl),
      cmocka_unit_test(test_list_remove_first_dl),
      cmocka_unit_test(test_list_remove_first_sl),
      cmocka_unit_test(test_list_remove_first_null_entry_dl),
      cmocka_unit_test(test_list_remove_first_null_entry_sl),
      cmocka_unit_test(test_list_foreach),
  };

  return cmocka_run_group_tests_name(
      "flytools list", tests, list_setup, list_teardown);
}
