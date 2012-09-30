#include <stdlib.h>
#include "common.h"

const char * const FLYERRMSG[8] = {
  /*EFLYNOMEM*/ "Can't allocate memory for a new object. (Are you out?)",
  /*EFLYBADFN*/ "Can't call this function for this kind of object.",
  /*EFLYBADFNONLY1*/ "The first parameter is not the expected kind.",
  /*EFLYBADFNONLY2*/ "The second parameter is not the expected kind.",
  /*EFLYBADFNBOTH*/ "Can't call this function for these kinds of objects.",
  /*EFLYBADCAST*/ "Could not cast to the expected type.",
  /*EFLYEMPTY*/ "Can't remove an item from an already empty structure.",
  /*EFLYBADARG*/ "Null or invalid argument specified in function call."
};

static void
(*error_handler)(int, const char * const, const char * const, const char * const, int) = NULL;

FLYAPI void
flytools_onerror(void (*h)(int, const char * const, const char * const, const char * const, int)) {
  error_handler = h;
}

FLYAPI void flytools_onerror_detach() {
  error_handler = NULL;
}

FLYAPI void
flytools_error(int err, const char * const msg, const char * const fn, const char * const file, int line)
{
  if(error_handler != NULL) {
    (*error_handler)(err, msg, fn, file, line);
  }
}
