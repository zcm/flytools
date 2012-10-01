#include <stdlib.h>
#include <stdio.h>

#include "flytools.h"

static dict *d;
static int expected = -1;

void expect(const int errcode) {
  expected = errcode;
}

void errhandler(int code, const char * const msg, const char * const func, const char * const file, int line) {
  if (expected >= 0 && code == expected) {
    printf("hit the expected error! (%d -- %s():%s:%d)\n", code, func, file, line);
  } else {
    printf("\n    ---> CAUGHT AN ERROR! (%d)\n    ---> %s\n    ---> in function %s, file %s line %d\n", 
        code, msg, func, file, line);
  }
  expected = -1;
}

int verify_dict_size(dict * restrict di) {
  unsigned int bucket_sum = 0, i = 0;
  while (i < di->maxsize) {
    bucket_sum += list_get_size(di->buckets[i]);
    i++;
  }
  return di->size == bucket_sum;
}

void test_insert(const char * const restrict key) {
  unsigned int previous;
  previous = d->size;
  printf("inserting key: %s (size: %u", key, d->size);
  dict_insert(d, key, key);
  printf("->%u", d->size);
  if (previous + 1 != d->size) {
    printf(") <-- THIS IS BAD!");
  } else {
    printf(")");
  }
  if (!verify_dict_size(d)) {
    printf(" (Structure size mismatch!)");
  } else {
    printf(" (Size OK)");
  }
  printf("\n");
}

void test_remove(const char * const restrict key) {
  unsigned int previous = 999999;
  previous = d->size;
  printf("removing key: %s (size: %u", key, d->size);
  const char * const restrict val = dict_remove(d, key);
  printf("->%u", d->size);
  if (previous - 1 != d->size) {
    printf(") <-- THIS IS BAD!");
  } else {
    printf(")");
  }
  if (key != val) {
    printf(" (VALUE MISMATCH!)");
  } else {
    printf(" (Values OK)");
  }
  if (!verify_dict_size(d)) {
    printf(" (Structure size mismatch!)");
  } else {
    printf(" (Size OK)");
  }
  printf("\n");
}

int main(void) {
  flytools_onerror(errhandler);

  d = dict_create(15);

  test_insert("cats");
  test_insert("dogs");
  test_remove("cats");
  test_insert("rabbits");
  test_insert("dragons");
  test_remove("rabbits");
  test_remove("dogs");
  test_remove("dragons");

  printf("trying an insert on a null dict... ");
  expect(EFLYBADARG);
  dict_insert(NULL, "something", NULL);
  printf("trying to insert a null key into the dict... ");
  expect(EFLYBADARG);
  dict_insert(d, NULL, NULL);

  return 0;
}
