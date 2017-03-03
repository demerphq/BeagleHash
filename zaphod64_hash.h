#ifndef ZAPHOD64_HASH_H
#define ZAPHOD64_HASH_H
#include "beagle_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

void zaphod64_seed_state_smhasher_test(int in_bits, const void *seed, const void *state);
void zaphod64_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);
void zaphod64_hash_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);

#ifdef __cplusplus
}
#endif


#endif
