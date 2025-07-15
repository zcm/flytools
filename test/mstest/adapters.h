#ifndef FLYTOOLS_MSTEST_ADAPTERS_H
#define FLYTOOLS_MSTEST_ADAPTERS_H

#include <stdbool.h>
#include <stdint.h>

void assert_true(bool b);
void assert_null(void* obj);
void assert_non_null(void* obj);
void assert_int_equal(uint64_t a, uint64_t b);
void assert_int_not_equal(uint64_t a, uint64_t b);
void assert_in_range(uint64_t value, uint64_t minimum, uint64_t maximum);
void assert_string_equal(char *s, char *t);
void assert_ptr_equal(void *p1, void *p2);
void assert_memory_equal(void* p1, void* p2, size_t size);

void _fail(const char * const file, const int line);

#endif
