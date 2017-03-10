#ifndef SBOX_HASH_H
#define SBOX_HASH_H
#include "beagle_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

void sbox_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void sbox_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);
void sbox_hash_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);

#ifdef __cplusplus
}
#endif


#endif
