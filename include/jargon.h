#ifndef ZCM_JARGON_H
#define ZCM_JARGON_H

#ifndef HAS_HAS_ATTRIBUTE
#ifdef __has_attribute
#define HAS_HAS_ATTRIBUTE 1
#else
#define HAS_HAS_ATTRIBUTE 0
#endif
#endif

#ifndef HAS_ATTRIBUTE_SYNTAX
#define HAS_ATTRIBUTE_SYNTAX \
  (__GNUC__ || __clang__ || __INTEL_LLVM_COMPILER)
#endif

#if !HAS_ATTRIBUTE_SYNTAX
#define __attribute__(x)
#endif

#endif
