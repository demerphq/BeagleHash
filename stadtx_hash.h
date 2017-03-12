#ifndef STADTX_HASH_H
#define STADTX_HASH_H
#include "beagle_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

void stadtx_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void stadtx_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

#ifdef __cplusplus
}
#endif


#endif
