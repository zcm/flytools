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

#ifdef WIN32
#define __func__ __FUNCTION__
#endif

#ifdef FLYAPIBUILD
#if (defined(_WIN32) && !defined(_WIN64)) || defined(__i386) || defined(_M_IX86)
#define IS32BIT
#endif

#ifdef IS32BIT
#define ONE 1
#else
#define ONE 1ULL
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
 
#ifdef FLYAPIBUILD
#define FLY_ERR(errcode) \
  flytools_error(errcode, FLYERRMSG[errcode], __func__, __FILE__, __LINE__)
#define FLY_ERR_CLEAR \
  flytools_error(EFLYOK, NULL, NULL, NULL, -1)
#endif

enum FLYERRCODE {
  EFLYOK = 0,
  EFLYNOMEM,
  EFLYBADFN,
  EFLYBADFNONLY1,
  EFLYBADFNONLY2,
  EFLYBADFNBOTH,
  EFLYBADCAST,
  EFLYEMPTY,
  EFLYBADARG,
  EFLYINTERNAL,
  FLYERRCODE_LENGTH /* not an errorcode -- do not use */
};

extern const char * const FLYERRMSG[FLYERRCODE_LENGTH];

FLYAPI void flytools_onerror(void (*h)(int, const char * const, const char * const, const char * const, int));

FLYAPI void flytools_onerror_detach();

FLYAPI void flytools_error(int err, const char * const msg, const char * const fn, const char * const file, int line);

FLYAPI int flytools_last_error();

#endif
