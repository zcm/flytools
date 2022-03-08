#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "dict.h"

int dict_setup(void **state) {
  (void) state;

  return 0;
}

int dict_teardown(void **state) {
  (void) state;

  return 0;
}

int verify_dict_size(dict * restrict d) {
  unsigned int bucket_sum = 0, i = 0;

  while (i < d->maxsize) {
    bucket_sum += list_size(d->buckets[i]);
    i++;
  }

  return d->size == bucket_sum;
}

void test_dict_new(void **state) {
  (void) state;

  dict *d = dict_new(20);
  assert_non_null(d);
  assert_int_equal(d->size, 0);
  dict_del(d);
}

void test_dict_insert_then_get(void **state) {
  (void) state;

  dict *d = dict_new(20);
  assert_non_null(d);

  dict_insert(d, "cats", "meow");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_insert(d, "dogs", "bark");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_insert(d, "birds", "chirp");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_get(d, "dogs"));
  assert_string_equal(value, "bark");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, "cats"));
  assert_string_equal(value, "meow");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, "birds"));
  assert_string_equal(value, "chirp");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_insert_then_remove(void **state) {
  (void) state;

  dict *d = dict_new(20);
  assert_non_null(d);

  dict_insert(d, "apple", "red");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_insert(d, "banana", "yellow");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_insert(d, "lime", "green");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_remove(d, "apple"));
  assert_string_equal(value, "red");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, "lime"));
  assert_string_equal(value, "green");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, "banana"));
  assert_string_equal(value, "yellow");
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  assert_null(dict_remove(d, "apple"));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_get_from_empty(void **state) {
  (void) state;

  dict *d = dict_new(2);
  assert_non_null(d);

  assert_null(dict_get(d, "nothing"));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_remove_from_empty(void **state) {
  (void) state;

  dict *d = dict_new(2);
  assert_non_null(d);

  assert_null(dict_remove(d, "nothing"));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_dict_new),
      cmocka_unit_test(test_dict_insert_then_get),
      cmocka_unit_test(test_dict_insert_then_remove),
      cmocka_unit_test(test_dict_get_from_empty),
      cmocka_unit_test(test_dict_remove_from_empty),
  };

  return cmocka_run_group_tests_name(
      "flytools dict", tests, dict_setup, dict_teardown);
}
