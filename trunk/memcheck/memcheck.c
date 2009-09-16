#include <stdio.h>

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#define __EXIT_FAILURE_DEFINED__
#endif

int check_alloc(void *pointer) {
	if(pointer == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}

#ifdef __EXIT_FAILURE_DEFINED__
#undef EXIT_FAILURE
#undef __EXIT_FAILURE_DEFINED__
#endif
