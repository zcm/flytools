/** @file common.h
 * This header file contains definitions and macros of things that are required
 * by all headers in the flytools.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009
 * Zachary Murray, all rights reserved. This source file may not be
 * redistributed, in whole or in part. By viewing and/or using this file, you
 * agree not to redistribute the source files without the express written
 * permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 * @date 11/10/2010 12:02:51 AM Pacific Standard Time (creation)
 */

#ifndef __FLYTOOLS_COMMON_H__
#define __FLYTOOLS_COMMON_H__

#if __STDC_VERSION__ < 202311L

#ifndef thread_local
#if defined(_MSC_VER)
#define thread_local __declspec(thread)
#elif defined(__TURBOC__)
#define thread_local
#else
#include <threads.h>
#endif
#endif /* thread_local */

#if __STDC_VERSION__ < 199901L

#ifndef restrict
#define restrict
#endif

#endif /* 199901L */
#endif /* 202311L */

#ifdef WIN32
#define __func__ __FUNCTION__
#endif

#ifdef __TURBOC__
#define __func__ ""
#endif

#ifdef FLYAPIBUILD
#if (defined(_WIN32) && !defined(_WIN64)) \
  || defined(__i386) || defined(_M_IX86) || defined(__TURBOC__)
#define IS32BIT
#endif

#endif

#if defined(_MSC_VER) && !defined(FLYAPI)
// This will be defined by the build environment if compiling the API.
// External projects linking against the Flytools should never define the
// FLYAPIBUILD macro -- it is for internal use only.
#ifdef FLYAPIBUILD
#define FLYAPI __declspec(dllexport)
#else
#define FLYAPI __declspec(dllimport)
#endif
#endif
// non-windows case
#ifndef FLYAPI
#define FLYAPI
#endif

#define FLY_STATUSES(DEFINITION)   \
  DEFINITION(FLY_OK)               \
  DEFINITION(FLY_EMPTY)            \
  DEFINITION(FLY_NOT_FOUND)        \
  DEFINITION(FLY_E_NULL_PTR)       \
  DEFINITION(FLY_E_INVALID_ARG)    \
  DEFINITION(FLY_E_OUT_OF_RANGE)   \
  DEFINITION(FLY_E_OUT_OF_MEMORY)  \
  DEFINITION(FLY_E_TOO_BIG)        \

#define AS_ENUM_DEFINITION(ENUM_NAME) ENUM_NAME,

enum FLY_STATUS {
  FLY_STATUSES(AS_ENUM_DEFINITION)
  FLY_STATUS_LEN
};

FLYAPI enum FLY_STATUS *fly_status_addr();
#define fly_status (*fly_status_addr())

#undef AS_ENUM_DEFINITION

extern FLYAPI const char * const FLY_STATUS_STR[FLY_STATUS_LEN];

#endif
