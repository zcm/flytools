#include "CppUnitTest.h"

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

void assert_string_equal(char *s, char *t) {
	if (s == t) {
		return;
	}

	std::string str_s(s);
	std::string str_t(t);

	Assert::AreEqual(str_s, str_t);
}

void _fail() {
	Assert::Fail();
}

}