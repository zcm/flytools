#include "tests.h"

#include "dict.h"
#include "internal/dict.h"

#ifndef _WINDLL
int dict_test_setup(void **state) {
  (void) state;

  return 0;
}

int dict_test_teardown(void **state) {
  (void) state;

  return 0;
}
#endif

#ifndef METHODS_ONLY
int verify_dict_size(dict * restrict d) {
  size_t bucket_sum = 0, i = 0;

  while (i < (size_t) 1 << d->exponent) {
    if (d->buckets[i].flags & 0x1) {
      bucket_sum += ((list *) d->buckets[i].data)->size;
    } else if (d->buckets[i].data) {
      bucket_sum += 1;
    }

    i++;
  }

  return d->size == bucket_sum;
}

#ifndef assert_fly_status
#define assert_fly_status(err) \
  assert_string_equal( \
      (char *) FLY_STATUS_STR[err], \
      (char *) FLY_STATUS_STR[fly_status])
#endif

void do_test_dict_new() {
  dict *d = dict_new();
  assert_non_null(d);
  assert_int_equal(0, d->size);
  dict_del(d);
}

void do_test_dict_new_of_size() {
  dict *d;

  d = dict_new_of_size(16);
  assert_non_null(d);
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_OK);
  dict_del(d);

  d = dict_new_of_size(0);
  assert_null(d);
  assert_fly_status(FLY_E_INVALID_ARG);

  d = dict_new_of_size(2);
  assert_non_null(d);
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_OK);
  dict_del(d);

  d = dict_new_of_size(1);
  assert_null(d);
  assert_fly_status(FLY_E_INVALID_ARG);

  d = dict_new_of_size(32);
  assert_non_null(d);
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_OK);
  dict_del(d);

  d = dict_new_of_size(3);
  assert_null(d);
  assert_fly_status(FLY_E_INVALID_ARG);

  d = dict_new_of_size(10);
  assert_null(d);
  assert_fly_status(FLY_E_INVALID_ARG);
}
#endif

TESTCALL(test_dict_new, do_test_dict_new())
TESTCALL(test_dict_new_of_size, do_test_dict_new_of_size())

#ifndef METHODS_ONLY
struct pet {
  char *name, *animal;
};

struct pet cat_cj = { "cj", "cat" },
           cat_donna = {"donna", "cat" },
           dog_wahwa = {"wahwa", "dog" };

void do_test_dict_set_then_get() {
  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  dict_set(d, &cat_cj, "purr");
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_set(d, &cat_donna, "chirp");
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_set(d, &dog_wahwa, "yip");
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_get(d, &dog_wahwa));
  assert_string_equal("yip", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, &cat_donna));
  assert_string_equal("chirp", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, &cat_cj));
  assert_string_equal("purr", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void do_test_dict_sets_then_gets() {
  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  dict_sets(d, "cats", "meow");
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_sets(d, "dogs", "bark");
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_sets(d, "birds", "chirp");
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_gets(d, "dogs"));
  assert_string_equal("bark", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_gets(d, "cats"));
  assert_string_equal("meow", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_gets(d, "birds"));
  assert_string_equal("chirp", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_del(d);
}
#endif

TESTCALL(test_dict_set_then_get, do_test_dict_set_then_get())
TESTCALL(test_dict_sets_then_gets, do_test_dict_sets_then_gets())

#ifndef METHODS_ONLY
struct bev {
  char *brand, *type;
};

struct bev coke = { "pepsi", "cola" },
           energy = { "monster", "legal stimulant" },
           fruit = { "fanta", "juice substitute" };

void do_test_dict_set_then_remove() {
  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  dict_set(d, &coke, "$2");
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_set(d, &energy, "$3");
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_set(d, &fruit, "$1");
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_remove(d, &fruit));
  assert_string_equal("$1", value);
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, &coke));
  assert_string_equal("$2", value);
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, &energy));
  assert_string_equal("$3", value);
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_null(dict_remove(d, &fruit));
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_NOT_FOUND);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void do_test_dict_sets_then_removes() {
  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  dict_sets(d, "apple", "red");
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_sets(d, "banana", "yellow");
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_sets(d, "lime", "green");
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_removes(d, "apple"));
  assert_string_equal("red", value);
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_removes(d, "lime"));
  assert_string_equal("green", value);
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_removes(d, "banana"));
  assert_string_equal("yellow", value);
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_null(dict_removes(d, "apple"));
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_NOT_FOUND);
  assert_true(verify_dict_size(d));

  dict_del(d);
}
#endif

TESTCALL(test_dict_set_then_remove, do_test_dict_set_then_remove())
TESTCALL(test_dict_sets_then_removes, do_test_dict_sets_then_removes())

#ifndef METHODS_ONLY
void do_test_dict_set_sets_get_gets_remove_removes_combo() {
  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  dict_set(d, &test_dict_new, "it's new");
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_sets(d, "banana", "it's yellow");
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_set(d, &printf, "it's printing");
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  dict_sets(d, "dreamcast", "it's thinking");
  assert_int_equal(4, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_get(d, &printf));
  assert_string_equal("it's printing", value);
  assert_int_equal(4, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_gets(d, "dreamcast"));
  assert_string_equal("it's thinking", value);
  assert_int_equal(4, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, &test_dict_new));
  assert_string_equal("it's new", value);
  assert_int_equal(4, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_gets(d, "banana"));
  assert_string_equal("it's yellow", value);
  assert_int_equal(4, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, &printf));
  assert_string_equal("it's printing", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_removes(d, "dreamcast"));
  assert_string_equal("it's thinking", value);
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, &test_dict_new));
  assert_string_equal("it's new", value);
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_removes(d, "banana"));
  assert_string_equal("it's yellow", value);
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_null(dict_remove(d, &test_dict_new));
  assert_fly_status(FLY_NOT_FOUND);
  assert_null(dict_remove(d, &printf));
  assert_fly_status(FLY_NOT_FOUND);
  assert_null(dict_removes(d, "dreamcast"));
  assert_fly_status(FLY_NOT_FOUND);
  assert_null(dict_removes(d, "banana"));
  assert_fly_status(FLY_NOT_FOUND);

  assert_int_equal(0, d->size);
  assert_true(verify_dict_size(d));

  dict_del(d);
}
#endif

TESTCALL(
    test_dict_set_sets_get_gets_remove_removes_combo,
    do_test_dict_set_sets_get_gets_remove_removes_combo())

#ifndef METHODS_ONLY
void do_test_dict_get_from_empty() {
  dict *d = dict_new_of_size(2);
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  assert_null(dict_get(d, &test_dict_new));
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_NOT_FOUND);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void do_test_dict_remove_from_empty() {
  dict *d = dict_new_of_size(2);
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  assert_null(dict_remove(d, &test_dict_new));
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_NOT_FOUND);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void do_test_dict_gets_from_empty() {
  dict *d = dict_new_of_size(2);
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  assert_null(dict_get(d, "nothing"));
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_NOT_FOUND);
  assert_true(verify_dict_size(d));

  dict_del(d);
}

void do_test_dict_removes_from_empty() {
  dict *d = dict_new_of_size(2);
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  assert_null(dict_remove(d, "nothing"));
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_NOT_FOUND);
  assert_true(verify_dict_size(d));

  dict_del(d);
}
#endif

TESTCALL(test_dict_get_from_empty, do_test_dict_get_from_empty())
TESTCALL(test_dict_remove_from_empty, do_test_dict_remove_from_empty())
TESTCALL(test_dict_gets_from_empty, do_test_dict_gets_from_empty())
TESTCALL(test_dict_removes_from_empty, do_test_dict_removes_from_empty())

#ifndef METHODS_ONLY
void do_test_dict_null_as_key() {
  dict *d;
  assert_non_null(d = dict_new_of_size(32));
  assert_fly_status(FLY_OK);

  char *one = "first";
  char *two = "second";

  dict_set(d, one, "uno");
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  dict_set(d, NULL, "cero");
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));
  dict_set(d, two, "dos");
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  char *value;

  assert_non_null(value = dict_get(d, one));
  assert_string_equal("uno", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, NULL));
  assert_string_equal("cero", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_get(d, two));
  assert_string_equal("dos", value);
  assert_int_equal(3, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_non_null(value = dict_remove(d, NULL));
  assert_string_equal("cero", value);
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_true(verify_dict_size(d));

  assert_null(value = dict_get(d, NULL));
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_NOT_FOUND);
  assert_true(verify_dict_size(d));

  value = dict_remove(d, NULL);
  assert_null(value);
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_NOT_FOUND);
  assert_true(verify_dict_size(d));

  dict_del(d);
}
#endif

TESTCALL(test_dict_null_as_key, do_test_dict_null_as_key())

#ifndef METHODS_ONLY
void do_test_dict_remove_after_collision() {
  void *one = (void *) 0x1;
  void *two = (void *) 0x5;

  // Hashes must collide for test to be valid
  assert_int_equal(
      hash_xorshift64s((uint64_t) one) & 0b11,
      hash_xorshift64s((uint64_t) two) & 0b11);
  assert_int_equal(
      hash_xorshift64s_ptr((uintptr_t) one) & 0b11,
      hash_xorshift64s_ptr((uintptr_t) two) & 0b11);

  char *first = "first";
  char *second = "second";

  dict *d = dict_new_of_size(4);

  dict_set(d, one, first);
  assert_int_equal(d->size, 1);
  assert_int_equal(d->exponent, 2);
  assert_fly_status(FLY_OK);

  dict_set(d, two, second);
  assert_int_equal(d->size, 2);
  assert_int_equal(d->exponent, 2);
  assert_fly_status(FLY_OK);

  char *value;

  assert_non_null(value = (char *) dict_get(d, one));
  assert_string_equal(first, value);
  assert_int_equal(d->size, 2);
  assert_int_equal(d->exponent, 2);
  assert_fly_status(FLY_OK);

  assert_non_null(value = (char *) dict_get(d, two));
  assert_string_equal(second, value);
  assert_int_equal(d->size, 2);
  assert_int_equal(d->exponent, 2);
  assert_fly_status(FLY_OK);

  assert_non_null(value = (char *) dict_remove(d, one));
  assert_string_equal(first, value);
  assert_int_equal(d->size, 1);
  assert_int_equal(d->exponent, 2);
  assert_fly_status(FLY_OK);

  assert_null(value = (char *) dict_remove(d, one));
  assert_int_equal(d->size, 1);
  assert_int_equal(d->exponent, 2);
  assert_fly_status(FLY_NOT_FOUND);

  assert_non_null(value = (char *) dict_get(d, two));
  assert_string_equal(second, value);
  assert_int_equal(d->size, 1);
  assert_int_equal(d->exponent, 2);
  assert_fly_status(FLY_OK);

  dict_del(d);
}

void do_test_dict_set_overwrites() {
  dict *d = dict_new();
  assert_non_null(d);
  assert_fly_status(FLY_OK);

  dict_set(d, (void *) 1, "one");
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_string_equal("one", dict_get(d, (void *) 1));
  assert_fly_status(FLY_OK);

  dict_set(d, (void *) 1, "uno");
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_string_equal("uno", dict_get(d, (void *) 1));
  assert_fly_status(FLY_OK);

  dict_set(d, (void *) 10, "ten");
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_string_equal("ten", dict_get(d, (void *) 10));
  assert_fly_status(FLY_OK);

  dict_set(d, (void *) 10, "cien");
  assert_int_equal(2, d->size);
  assert_fly_status(FLY_OK);
  assert_string_equal("cien", dict_get(d, (void *) 10));
  assert_fly_status(FLY_OK);

  assert_string_equal("uno", dict_remove(d, (void *) 1));
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_OK);
  assert_null(dict_get(d, (void *) 1));
  assert_int_equal(1, d->size);
  assert_fly_status(FLY_NOT_FOUND);

  assert_string_equal("cien", dict_remove(d, (void *) 10));
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_OK);
  assert_null(dict_get(d, (void *) 10));
  assert_int_equal(0, d->size);
  assert_fly_status(FLY_NOT_FOUND);

  dict_del(d);
}
#endif

TESTCALL(test_dict_remove_after_collision, do_test_dict_remove_after_collision())
TESTCALL(test_dict_set_overwrites, do_test_dict_set_overwrites())

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
  assert_int_equal(0, d->size);
  assert_int_equal(2, d->exponent);

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
    assert_int_equal(i + 1, d->size);
    assert_int_equal(expected_exponents[i], d->exponent);
    if (last_x != d->exponent) {
      last_x = d->exponent;
      for (j = 0; j < i; j++) {
        assert_string_equal(word_assoc[j], dict_get(d, (void *) j));
      }
    }
  }

  for (i = 0; i < 32; i++) {
    assert_string_equal(word_assoc[i], dict_get(d, (void *) i));
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

  assert_string_equal(answer, letter);
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
