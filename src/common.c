#include <stdlib.h>
#include "common.h"

const char * const FLYERRMSG[FLYERRCODE_LENGTH] = {
  /*EFLYOK*/ "Everything is fine, nothing is broken.",
  /*EFLYNOMEM*/ "Can't allocate memory for a new object. (Are you out?)",
  /*EFLYBADFN*/ "Can't call this function for this kind of object.",
  /*EFLYBADFNONLY1*/ "The first parameter is not the expected kind.",
  /*EFLYBADFNONLY2*/ "The second parameter is not the expected kind.",
  /*EFLYBADFNBOTH*/ "Can't call this function for these kinds of objects.",
  /*EFLYBADCAST*/ "Could not cast to the expected type.",
  /*EFLYEMPTY*/ "Can't remove an item from an already empty structure.",
  /*EFLYBADARG*/ "Null or invalid argument specified in function call.",
  /*EFLYINTERNAL*/ "An internal error occurred in the Flytools."
};

static void (*error_handler)(int, const char * const, const char * const, const char * const, int) = NULL;
static int last_error = EFLYOK;

FLYAPI void flytools_onerror(void (*h)(int, const char * const, const char * const, const char * const, int)) {
  error_handler = h;
}

FLYAPI void flytools_onerror_detach() {
  error_handler = NULL;
}

FLYAPI void flytools_error(int err, const char * const msg, const char * const fn, const char * const file, int line) {
  if (line >= 0 && error_handler != NULL) {
    (*error_handler)(err, msg, fn, file, line);
  }
  last_error = err;
}

FLYAPI int flytools_last_error() {
  return last_error;
}
