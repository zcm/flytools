/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014-2017 Melissa O'Neill <oneill@pcg-random.org>,
 *                     and the PCG Project contributors.
 *
 * SPDX-License-Identifier: (Apache-2.0 OR MIT)
 *
 * Licensed under the Apache License, Version 2.0 (provided in
 * LICENSE-APACHE.txt and at http://www.apache.org/licenses/LICENSE-2.0)
 * or under the MIT license (provided in LICENSE-MIT.txt and at
 * http://opensource.org/licenses/MIT), at your option. This file may not
 * be copied, modified, or distributed except according to those terms.
 *
 * Distributed on an "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied.  See your chosen license for details.
 *
 * For additional information about the PCG random number generation scheme,
 * visit http://www.pcg-random.org/.
 */

/* This code provides a mechanism for getting external randomness for
 * seeding purposes.  Usually, it's just a wrapper around reading
 * /dev/random.
 *
 * Alas, because not every system provides /dev/random, we need a fallback.
 * We also need to try to test whether or not to use the fallback.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "pcg_variants.h"
#include "entropy.h"

#ifndef IS_UNIX
#if !defined(_WIN32) && (defined(__unix__)  || defined(__unix) \
                         || (defined(__APPLE__) && defined(__MACH__)))
    #define IS_UNIX 1
#else
    #define IS_UNIX 0
#endif
#endif

/* If HAVE_DEV_RANDOM is set, we use that value, otherwise we guess */
#ifndef HAVE_DEV_RANDOM
#define HAVE_DEV_RANDOM IS_UNIX
#endif

#if HAVE_DEV_RANDOM
#include <fcntl.h>
#include <unistd.h>
#endif

/**
 * Get some external entropy for seeding purposes.
 *
 * On operating systems that support `/dev/urandom`, it will be used as the
 * source for entropy unless reading from it fails for some reason, which ought
 * to never happen. But if it does, or no random device is available, a fallback
 * method is used.
 *
 * The fallback method uses a private RNG to return different seeds between
 * calls. It makes no attempt at cryptographic security. It combines use of the
 * addresses of stack variables and library code with the current time as
 * initializers, which is better than using the current time alone as most
 * modern operating systems use address space layout randomization.
 */
FLYAPI void entropy_getbytes(void *dest, size_t size) {
#if HAVE_DEV_RANDOM
  int fd = open("/dev/urandom", O_RDONLY);

  if (fd >= 0) {
    ssize_t sz = read(fd, dest, size);

    if ((close(fd) == 0) && (sz == (ssize_t) size)) {
      return;
    }
  }
#endif

  static int intitialized = 0;
  static pcg32_random_t entropy_rng;

  if (!intitialized) {
    int dummyvar;

    pcg32_srandom_r(
        &entropy_rng, time(NULL) ^ (intptr_t) &time, (intptr_t) &dummyvar);

    intitialized = 1;
  }

  char *dest_cp = (char *) dest;
  for (size_t i = 0; i < size; ++i) {
    dest_cp[i] = (char) pcg32_random_r(&entropy_rng);
  }
}

