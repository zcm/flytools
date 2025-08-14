#ifndef _WINDLL
#ifndef TEST
#define TEST(name, def) void name(void **state) def
#define TESTCALL(name, call) TEST(name, { (void) state; call; })

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#define testlog print_message
#define vtestlog vprint_message

#endif  // TEST
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

#include "mockmem.h"

#ifndef assert_fly_status
#define assert_fly_status(err) \
  assert_string_equal( \
      (char *) FLY_STATUS_STR[err], \
      (char *) FLY_STATUS_STR[fly_status])
#endif
