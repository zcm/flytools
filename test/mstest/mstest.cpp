#include "CppUnitTest.h"

#define METHODS_ONLY
#define TEST(name, def) void name();

extern "C" {
#include "test_dict.c"
#include "test_list.c"
}

#undef TEST
#define TEST(name, def) TEST_METHOD(name) { \
	::name(); \
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace flytools {
	TEST_CLASS(dict) {
		public:
#include "test_dict.c"
	};
	TEST_CLASS(list) {
#include "test_list.c"
	};
}
