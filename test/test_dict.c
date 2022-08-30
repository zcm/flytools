#ifndef _WINDLL
#define TEST(name, def) void name(void **state) def

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>

#include "dict.h"
#include "internal/dict.h"

int dict_test_setup(void **state) {
  (void) state;

  return 0;
}

int dict_test_teardown(void **state) {
  (void) state;

  return 0;
}

#else
static void **state;

#ifndef TEST
#include <stdio.h>

#include "dict.h"
#include "internal/dict.h"

#include "adapters.h"

#define TEST(name, def) void name() def
#endif

#define restrict
#endif

#ifndef METHODS_ONLY
int verify_dict_size(dict * restrict d) {
  size_t bucket_sum = 0, i = 0;

  while (i < 1 << d->exponent) {
    if (d->buckets[i].flags & 0x1) {
      bucket_sum += ((list *) d->buckets[i].data)->size;
    } else if (d->buckets[i].data) {
      bucket_sum += 1;
    }

    i++;
  }

  return d->size == bucket_sum;
}

#define assert_fly_error(err) \
  assert_int_equal(flytools_last_error(), err)
#endif

TEST(test_dict_new, {
  (void) state;

  dict *d = dict_new();
  assert_non_null(d);
  assert_int_equal(d->size, 0);
  dict_del(d);
})

TEST(test_dict_new_of_size, {
  (void) state;

  dict *d;

  d = dict_new_of_size(16);
  assert_non_null(d);
  assert_int_equal(d->size, 0);
  assert_fly_error(EFLYOK);
  dict_del(d);

  d = dict_new_of_size(0);
  assert_null(d);
  assert_fly_error(EFLYBADARG);

  d = dict_new_of_size(2);
  assert_non_null(d);
  assert_int_equal(d->size, 0);
  assert_fly_error(EFLYOK);
  dict_del(d);

  d = dict_new_of_size(1);
  assert_null(d);
  assert_fly_error(EFLYBADARG);

  d = dict_new_of_size(32);
  assert_non_null(d);
  assert_int_equal(d->size, 0);
  assert_fly_error(EFLYOK);
  dict_del(d);

  d = dict_new_of_size(3);
  assert_null(d);
  assert_fly_error(EFLYBADARG);

  d = dict_new_of_size(10);
  assert_null(d);
  assert_fly_error(EFLYBADARG);
})

#ifndef METHODS_ONLY
struct pet {
  char *name, *animal;
};

struct pet cat_cj = { "cj", "cat" },
           cat_donna = {"donna", "cat" },
           dog_wahwa = {"wahwa", "dog" };
#endif

TEST(test_dict_set_then_get, {
  (void) state;

  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  dict_set(d, &cat_cj, "purr");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_set(d, &cat_donna, "chirp");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_set(d, &dog_wahwa, "yip");
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
})

TEST(test_dict_sets_then_gets, {
  (void) state;

  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  dict_sets(d, "cats", "meow");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_sets(d, "dogs", "bark");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_sets(d, "birds", "chirp");
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
})

#ifndef METHODS_ONLY
struct bev {
  char *brand, *type;
};

struct bev coke = { "pepsi", "cola" },
           energy = { "monster", "legal stimulant" },
           fruit = { "fanta", "juice substitute" };
#endif

TEST(test_dict_set_then_remove, {
  (void) state;

  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  dict_set(d, &coke, "$2");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_set(d, &energy, "$3");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_set(d, &fruit, "$1");
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
})

TEST(test_dict_sets_then_removes, {
  (void) state;

  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  dict_sets(d, "apple", "red");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_sets(d, "banana", "yellow");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_sets(d, "lime", "green");
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
})

TEST(test_dict_set_sets_get_gets_remove_removes_combo, {
  (void) state;

  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  dict_set(d, &test_dict_new, "it's new");
  assert_int_equal(d->size, 1);
  assert_true(verify_dict_size(d));

  dict_sets(d, "banana", "it's yellow");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_set(d, &printf, "it's printing");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  dict_sets(d, "dreamcast", "it's thinking");
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
})

TEST(test_dict_get_from_empty, {
  (void) state;

  dict *d = dict_new_of_size(2);
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  assert_null(dict_get(d, &test_dict_new));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
})

TEST(test_dict_remove_from_empty, {
  (void) state;

  dict *d = dict_new_of_size(2);
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  assert_null(dict_remove(d, &test_dict_new));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
})

TEST(test_dict_gets_from_empty, {
  (void) state;

  dict *d = dict_new_of_size(2);
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  assert_null(dict_get(d, "nothing"));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
})

TEST(test_dict_removes_from_empty, {
  (void) state;

  dict *d = dict_new_of_size(2);
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  assert_null(dict_remove(d, "nothing"));
  assert_int_equal(d->size, 0);
  assert_true(verify_dict_size(d));

  dict_del(d);
})

#ifndef METHODS_ONLY
void do_test_dict_null_as_key() {
  dict *d;
  assert_non_null(d = dict_new_of_size(32));

  char *one = "first";
  char *two = "second";

  dict_set(d, one, "uno");
  assert_int_equal(d->size, 1);
  dict_set(d, NULL, "cero");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));
  dict_set(d, two, "dos");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_get(d, one));
  assert_string_equal(value, "uno");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));
  assert_non_null(value = dict_get(d, NULL));
  assert_string_equal(value, "cero");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));
  assert_non_null(value = dict_get(d, two));
  assert_string_equal(value, "dos");
  assert_int_equal(d->size, 3);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, NULL));
  assert_string_equal(value, "cero");
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  assert_null(value = dict_get(d, NULL));
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));
  value = dict_remove(d, NULL);
  assert_null(value);
  assert_int_equal(d->size, 2);
  assert_true(verify_dict_size(d));

  dict_del(d);
}
#endif

TEST(test_dict_null_as_key, {
  (void) state;

  do_test_dict_null_as_key();
})

TEST(test_dict_set_overwrites, {
  (void) state;

  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_error(EFLYOK);

  dict_set(d, (void *) 1, "one");
  assert_int_equal(d->size, 1);
  assert_string_equal(dict_get(d, (void *) 1), "one");

  dict_set(d, (void *) 1, "uno");
  assert_int_equal(d->size, 1);
  assert_string_equal(dict_get(d, (void *) 1), "uno");

  dict_set(d, (void *) 10, "ten");
  assert_int_equal(d->size, 2);
  assert_string_equal(dict_get(d, (void *) 10), "ten");

  dict_set(d, (void *) 10, "cien");
  assert_int_equal(d->size, 2);
  assert_string_equal(dict_get(d, (void *) 10), "cien");

  assert_string_equal(dict_remove(d, (void *) 1), "uno");
  assert_int_equal(d->size, 1);
  assert_null(dict_get(d, (void *) 1));
  assert_int_equal(d->size, 1);

  assert_string_equal(dict_remove(d, (void *) 10), "cien");
  assert_int_equal(d->size, 0);
  assert_null(dict_get(d, (void *) 10));
  assert_int_equal(d->size, 0);

  dict_del(d);
})

#ifndef METHODS_ONLY
void *(*volatile _passthrough_malloc)(size_t) = &malloc;
void (*volatile _passthrough_free)(void *) = &free;

void *passthrough_malloc(size_t size) {
  return _passthrough_malloc(size);
}

void passthrough_free(void *ptr) {
  _passthrough_free(ptr);
}

void _test_dict_resize(dict *d) {
  assert_non_null(d);
  assert_int_equal(d->size, 0);
  assert_int_equal(d->exponent, 2);

  char *word_assoc[32] = {
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight",
    "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen", "twenty", "twenty-one",
    "twenty-two", "twenty-three", "twenty-four", "twenty-five", "twenty-six",
    "twenty-seven", "twenty-eight", "twenty-nine", "thirty", "thirty-one",
  };

  const size_t expected_exponents[32] = {
    //  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16
        2,  2,  2,  3,  3,  3,  4,  4,  4,  4,  4,  4,  5,  5,  5,  5,
    // 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
        5,  5,  5,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,
  };

  size_t i, j, last_x = 2;

  for (i = 0; i < 32; i++) {
    dict_set(d, (void *) i, word_assoc[i]);
    assert_int_equal(d->size, i + 1);
    assert_int_equal(d->exponent, expected_exponents[i]);
    if (last_x != d->exponent) {
      last_x = d->exponent;
      for (j = 0; j < i; j++) {
        assert_string_equal(dict_get(d, (void *) j), word_assoc[j]);
      }
    }
  }

  for (i = 0; i < 32; i++) {
    assert_string_equal(dict_get(d, (void *) i), word_assoc[i]);
  }

  dict_del(d);
}
#endif

TEST(test_dict_resize, {
  (void) state;

  _test_dict_resize(dict_new_of_size(4));
})

TEST(test_dict_resize_custom_allocator, {
  (void) state;

  _test_dict_resize(dict_new_with(4, &passthrough_malloc, &passthrough_free));
})

#ifndef METHODS_ONLY
static char *letters = "abcdefghij";
static char *answers = "abcdefghij";
static char *answers2 = "abjdifh";
static char *answers3 = "abjdhf";
static char *answers4 = "fbjdh";
static char *answers5 = "fbjdhz";
static char *answers6 = "fbjdhzacegi";
static char *answers7 = "fbjdhzacegiabcdefghij";

static char *answer_key = NULL;

static int verify_order(char *lptr, size_t i) {
  char letter[2] = { *lptr },
       answer[2] = { answer_key[i] };

  assert_string_equal(letter, answer);
  return 0;
}

void do_test_dict_foreach() {
  dict *d = dict_new();

  dict_foreach(d, (void *) &_fail);

  for (uintptr_t i = 0; i < 10; ++i) {
    dict_set(d, (void *) i, letters + i);
  }

  answer_key = answers;
  dict_foreach(d, (void *) &verify_order);

  dict_remove(d, (void *) 2);
  dict_remove(d, (void *) 4);
  dict_remove(d, (void *) 6);

  answer_key = answers2;
  dict_foreach(d, (void *) &verify_order);

  dict_remove(d, (void *) 8);

  answer_key = answers3;
  dict_foreach(d, (void *) &verify_order);

  dict_remove(d, (void *) 0);

  answer_key = answers4;
  dict_foreach(d, (void *) &verify_order);

  dict_set(d, (void *) 100, "z");

  answer_key = answers5;
  dict_foreach(d, (void *) &verify_order);

  for (uintptr_t i = 0; i < 10; ++i) {
    dict_set(d, (void *) i, letters + i);
  }

  answer_key = answers6;
  dict_foreach(d, (void *) &verify_order);

  for (uintptr_t i = 10; i < 19; ++i) {
    dict_set(d, (void *) i, letters + i - 10);
  }

  answer_key = answers7;
  dict_foreach(d, (void *) &verify_order);

  dict_del(d);
}
#endif

TEST(test_dict_foreach, {
  (void) state;

  do_test_dict_foreach();
})

#ifndef _WINDLL

#define dict_unit_test(f) \
  cmocka_unit_test_setup_teardown(f, dict_test_setup, dict_test_teardown)

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_dict_new),
      cmocka_unit_test(test_dict_new_of_size),
      dict_unit_test(test_dict_set_then_get),
      dict_unit_test(test_dict_sets_then_gets),
      dict_unit_test(test_dict_set_then_remove),
      dict_unit_test(test_dict_sets_then_removes),
      dict_unit_test(test_dict_set_sets_get_gets_remove_removes_combo),
      dict_unit_test(test_dict_get_from_empty),
      dict_unit_test(test_dict_remove_from_empty),
      dict_unit_test(test_dict_gets_from_empty),
      dict_unit_test(test_dict_removes_from_empty),
      dict_unit_test(test_dict_null_as_key),
      dict_unit_test(test_dict_set_overwrites),
      dict_unit_test(test_dict_resize),
      dict_unit_test(test_dict_resize_custom_allocator),
      dict_unit_test(test_dict_foreach),
  };

  return cmocka_run_group_tests_name(
      "flytools dict", tests, NULL, NULL);
}

#endif
