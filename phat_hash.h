#ifndef PHAT_HASH_H
#define PHAT_HASH_H
#include "beagle_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

void phat4_hash_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void phat4_hash_with_state_smhasher_test (const void *key, STRLEN len, const void *seed, void *out);

#ifdef __cplusplus
}
#endif

#endif
