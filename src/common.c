#include <stdlib.h>
#include "common.h"

const char * const FLYERRMSG[7] = {
  /*EFLYNOMEM*/ "Can't allocate memory for a new object. (Are you out?)",
  /*EFLYBADFN*/ "Can't call this function for this kind of object.",
  /*EFLYBADFNONLY1*/ "The first parameter is not the expected kind.",
  /*EFLYBADFNONLY2*/ "The second parameter is not the expected kind.",
  /*EFLYBADFNBOTH*/ "Can't call this function for these kinds of objects.",
  /*EFLYBADCAST*/ "Could not cast to the expected type.",
  /*EFLYEMPTY*/ "Can't remove an item from an already empty structure."
};

static void
(*error_handler)(int, const char * const, char *, char *, int) = NULL;

FLYAPI void
flytools_onerror(void (*h)(int, const char * const, char *, char *, int)) {
  error_handler = h;
}

FLYAPI void flytools_onerror_detach() {
  error_handler = NULL;
}

FLYAPI void
flytools_error(int err, const char * const msg, char *fn, char *file, int line)
{
  if(error_handler != NULL) {
    (*error_handler)(err, msg, fn, file, line);
  }
}
