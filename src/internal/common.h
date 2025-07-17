#ifndef ZCM_INTERNAL_COMMON_H_
#define ZCM_INTERNAL_COMMON_H_

#include <stdint.h>

#define PTRINDEX_MAX (PTRDIFF_MAX / sizeof (void *))

#ifndef ASSUME
#if defined(_MSC_VER)
#define ASSUME(expr) __assume(expr)
#else
#ifdef __has_attribute
#if __has_attribute(assume) && (!__clang__ || __clang_major__ >= 19)
#define ASSUME(expr) __attribute__((assume (expr)))
#else
#if __GNUC__ >= 13
#define ASSUME(expr) __attribute__((assume (expr)))
#elif __STDC_VERSION__ >= 202311L
#define ASSUME(expr) [[assume (expr)]]
#else
#define ASSUME(expr)
#endif
#endif
#endif /* __has_attribute */
#endif /* _MSC_VER */
#endif /* ASSUME */

#endif
