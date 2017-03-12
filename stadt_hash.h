#ifndef STADT_HASH_H
#define STADT_HASH_H
#include "beagle_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

void stadt_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void stadt_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

#ifdef __cplusplus
}
#endif


#endif
