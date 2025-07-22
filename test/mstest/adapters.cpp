#include <stdio.h>

#include "CppUnitTest.h"
#include "adapters.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" {

void assert_true(bool b) {
	Assert::IsTrue(b);
}

void assert_null(void *obj) {
	Assert::IsNull(obj);
}

void assert_non_null(void *obj) {
	Assert::IsNotNull(obj);
}

void assert_int_equal(uint64_t a, uint64_t b) {
	Assert::AreEqual(a, b);
}

void assert_int_not_equal(uint64_t a, uint64_t b) {
	Assert::AreNotEqual(a, b);
}

void assert_in_range(uint64_t value, uint64_t minimum, uint64_t maximum) {
	Assert::IsTrue(minimum <= value && value <= maximum);
}

void assert_string_equal(char *s, char *t) {
	if (s == t) {
		return;
	}

	std::string str_s(s);
	std::string str_t(t);

	Assert::AreEqual(str_s, str_t);
}

void assert_string_not_equal(char *s, char *t) {
	if (s == t) {
		return;
	}

	std::string str_s(s);
	std::string str_t(t);

	Assert::AreNotEqual(str_s, str_t);
}

void assert_ptr_equal(void *p1, void *p2) {
	Assert::AreEqual(p1, p2);
}

void assert_ptr_not_equal(void *p1, void *p2) {
	Assert::AreNotEqual(p1, p2);
}

void assert_memory_equal(void *p1, void *p2, size_t size) {
	if (p1 == p2) {
		return;
	}

	char *c1 = (char *) p1,
		 *c2 = (char *) p2;

	for (size_t i = 0; i < size; i++) {
		if (c1[i] != c2[i]) {
			Assert::Fail();
		}
	}
}

void _fail(const char * const file, const int line) {
	Assert::Fail();
}

void testlog(const char *msg, ...) {
  va_list args;
  va_start(args, msg);

  vtestlog(msg, args);

  va_end(args);
}

void vtestlog(const char *msg, va_list args) {
  char s[1024];

  vsnprintf(s, sizeof (s), msg, args);
  Logger::WriteMessage((const char *) s);
}

}
