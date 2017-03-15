#ifndef BEAGLEHASHES_TEST_HASH_H
#define BEAGLEHASHES_TEST_HASH_H

#ifndef STRLEN
#define STRLEN int
#endif

#ifdef __cplusplus
extern "C" {
#endif

void stadtx_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void stadtx_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

void beagle_hash_with_state_32_128_a_smhasher ( const void *blob, const int len, const void *seed, void *out );
void beagle_hash_with_state_64_128_a_smhasher ( const void *blob, const int len, const void *seed, void *out );
void beagle_seed_state_128_a_smhasher ( const int in_bits, const void *seed, void *state );

void zaphod32_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void zaphod32_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

void zaphod64_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void zaphod64_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

void phat_hash_with_state_smhasher_test (const void *key, STRLEN len, const void *seed, void *out);

void sbox_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void sbox_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

#ifdef __cplusplus
}
#endif


#endif
