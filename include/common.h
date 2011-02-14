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

#if defined(_MSC_VER) && !defined(FLYAPI)
// This will be defined by the build environment if compiling the API.
// External projects linking against the Flytools should never define the
// FLYAPIBUILD macro -- it is for internal use only.
#ifdef FLYAPIBUILD
#define FLYAPI __declspec(dllexport)
#define FLY_ERR(errcode) \
  flytools_error(errcode, FLYERRMSG[errcode], __FUNCTION__, __FILE__, __LINE__)
#else
#define FLYAPI __declspec(dllimport)
#endif

#endif

#define FLYTOOLS_TYPE_GENERIC 0x00000000
#define FLYTOOLS_TYPE_LIST    0x00000001
#define FLYTOOLS_TYPE_DICT    0x00000002

#define FLYTOOLS_TYPE_NODE    0x10000000
#define FLYTOOLS_TYPE_KIND    0x20000000

enum FLYERRCODE {
  EFLYNOMEM,
  EFLYBADFN,
  EFLYBADFNONLY1,
  EFLYBADFNONLY2,
  EFLYBADFNBOTH,
  EFLYBADCAST,
  EFLYEMPTY
};

extern const char * const FLYERRMSG[7];

FLYAPI void
flytools_onerror(void (*h)(int, const char * const, char *, char *, int));

FLYAPI void flytools_onerror_detach();

FLYAPI void
flytools_error(int err, const char * const msg, char *fn, char *file, int line);

#endif
