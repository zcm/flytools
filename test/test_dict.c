#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>

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

void test_dict_put_then_get(void **state) {
  (void) state;

  struct pet {
    char *name, *animal;
  };

  struct pet cat_cj = { "cj", "cat" },
             cat_donna = {"donna", "cat" },
             dog_wahwa = {"wahwa", "dog" };

  dict *d = dict_new(64);
  assert_non_null(d);

  dict_put(d, &cat_cj, "purr");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_put(d, &cat_donna, "chirp");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_put(d, &dog_wahwa, "yip");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_get(d, &dog_wahwa));
  assert_string_equal(value, "yip");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, &cat_donna));
  assert_string_equal(value, "chirp");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, &cat_cj));
  assert_string_equal(value, "purr");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_puts_then_gets(void **state) {
  (void) state;

  dict *d = dict_new(20);
  assert_non_null(d);

  dict_puts(d, "cats", "meow");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_puts(d, "dogs", "bark");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_puts(d, "birds", "chirp");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_gets(d, "dogs"));
  assert_string_equal(value, "bark");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_gets(d, "cats"));
  assert_string_equal(value, "meow");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_gets(d, "birds"));
  assert_string_equal(value, "chirp");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_put_then_remove(void **state) {
  (void) state;

  struct bev {
    char *brand, *type;
  };

  struct bev coke = { "pepsi", "cola" },
             energy = { "monster", "legal stimulant" },
             fruit = { "fanta", "juice substitute" };

  dict *d = dict_new(20);
  assert_non_null(d);

  dict_put(d, &coke, "$2");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_put(d, &energy, "$3");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_put(d, &fruit, "$1");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_remove(d, &fruit));
  assert_string_equal(value, "$1");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, &coke));
  assert_string_equal(value, "$2");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, &energy));
  assert_string_equal(value, "$3");
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  assert_null(dict_remove(d, &fruit));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_puts_then_removes(void **state) {
  (void) state;

  dict *d = dict_new(20);
  assert_non_null(d);

  dict_puts(d, "apple", "red");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_puts(d, "banana", "yellow");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_puts(d, "lime", "green");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_removes(d, "apple"));
  assert_string_equal(value, "red");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_removes(d, "lime"));
  assert_string_equal(value, "green");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_removes(d, "banana"));
  assert_string_equal(value, "yellow");
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  assert_null(dict_removes(d, "apple"));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}


void test_dict_put_puts_get_gets_remove_removes_combo(void **state) {
  (void) state;

  dict *d = dict_new(20);
  assert_non_null(d);

  dict_put(d, &test_dict_new, "it's new");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_puts(d, "banana", "it's yellow");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_put(d, &printf, "it's printing");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  dict_puts(d, "dreamcast", "it's thinking");
  assert_int_equal(d->size, 4);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_get(d, &printf));
  assert_string_equal(value, "it's printing");
  assert_int_equal(d->size, 4);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_gets(d, "dreamcast"));
  assert_string_equal(value, "it's thinking");
  assert_int_equal(d->size, 4);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, &test_dict_new));
  assert_string_equal(value, "it's new");
  assert_int_equal(d->size, 4);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_gets(d, "banana"));
  assert_string_equal(value, "it's yellow");
  assert_int_equal(d->size, 4);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, &printf));
  assert_string_equal(value, "it's printing");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_removes(d, "dreamcast"));
  assert_string_equal(value, "it's thinking");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, &test_dict_new));
  assert_string_equal(value, "it's new");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_removes(d, "banana"));
  assert_string_equal(value, "it's yellow");
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  assert_null(dict_remove(d, &test_dict_new));
  assert_null(dict_remove(d, &printf));
  assert_null(dict_removes(d, "dreamcast"));
  assert_null(dict_removes(d, "banana"));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_get_from_empty(void **state) {
  (void) state;

  dict *d = dict_new(2);
  assert_non_null(d);

  assert_null(dict_get(d, &test_dict_new));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_remove_from_empty(void **state) {
  (void) state;

  dict *d = dict_new(2);
  assert_non_null(d);

  assert_null(dict_remove(d, &test_dict_new));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_gets_from_empty(void **state) {
  (void) state;

  dict *d = dict_new(2);
  assert_non_null(d);

  assert_null(dict_get(d, "nothing"));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void test_dict_removes_from_empty(void **state) {
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
      cmocka_unit_test(test_dict_put_then_get),
      cmocka_unit_test(test_dict_puts_then_gets),
      cmocka_unit_test(test_dict_put_then_remove),
      cmocka_unit_test(test_dict_puts_then_removes),
      cmocka_unit_test(test_dict_put_puts_get_gets_remove_removes_combo),
      cmocka_unit_test(test_dict_get_from_empty),
      cmocka_unit_test(test_dict_remove_from_empty),
      cmocka_unit_test(test_dict_gets_from_empty),
      cmocka_unit_test(test_dict_removes_from_empty),
  };

  return cmocka_run_group_tests_name(
      "flytools dict", tests, dict_setup, dict_teardown);
}
