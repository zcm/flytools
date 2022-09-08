#ifndef _WINDLL
#define TEST(name, def) void name(void **state) def
#define TESTCALL(name, call) TEST(name, { (void) state; call; });

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#else

#define TESTCALL(name, call) TEST(name, { call; });

#ifndef TEST
#include <stdio.h>
#include <string.h>

#include "adapters.h"

static void **state;

#define TEST(name, def) void name() def
#endif

#ifdef _MSC_VER
#define restrict __restrict
#else
#define restrict
#endif
#endif

#ifndef assert_fly_error
#define assert_fly_error(err) \
  assert_int_equal(flytools_last_error(), err)
#endif
