#ifndef BEAGLEHASHES_TEST_HASH_H
#define BEAGLEHASHES_TEST_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

void stadtx_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void stadtx_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

void beagle_hash_32_32_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_32_64_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_32_96_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_32_112_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_32_127_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_32_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_64_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_96_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_112_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_127_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);

void beagle_hash_with_state_32_128_a_smhasher ( const void *blob, const int len, const void *seed, void *out );
void beagle_hash_with_state_64_128_a_smhasher ( const void *blob, const int len, const void *seed, void *out );
void beagle_seed_state_128_a_smhasher ( const int in_bits, const void *seed, void *state );

#ifdef __cplusplus
}
#endif


#endif
