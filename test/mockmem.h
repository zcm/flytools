#ifndef FLYTEST_MOCKMEM_H
#define FLYTEST_MOCKMEM_H

#include <stdlib.h>

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "ImageHlp.lib")
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void mockmem_queue(void *ptr);
extern void *mockmem_peek();

#ifdef _MSC_VER
extern DWORD hook_malloc(HMODULE mod);
#endif

#ifdef __cplusplus
}
#endif

#endif
