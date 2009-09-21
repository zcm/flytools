#ifndef _FUGUE_HASH_
#define _FUGUE_HASH_

typedef short crypto_uint8;
typedef unsigned long crypto_uint32;
typedef unsigned long long crypto_uint64;

int fugue_hash(unsigned char *out, const unsigned char *in,
                unsigned long long inlen);

#endif
