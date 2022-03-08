#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

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
  };

  return cmocka_run_group_tests_name(
      "flytools list", tests, list_setup, list_teardown);
}
