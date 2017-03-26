#ifndef BEAGLEHASHES_TEST_HASH_H
#define BEAGLEHASHES_TEST_HASH_H

#ifndef STRLEN
#define STRLEN int
#endif
#ifndef SBOX32_MAX_LEN
#define SBOX32_MAX_LEN 128
#endif
#ifndef SBOX32_STATE_WORDS
#define SBOX32_STATE_WORDS (1 + (SBOX32_MAX_LEN * 256))
#define SBOX32_STATE_BYTES (SBOX32_STATE_WORDS * sizeof(uint32_t))
#define SBOX32_STATE_BITS (SBOX32_STATE_BYTES * 8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SBOX32_SMHASHER_SEEDBITS 128
#define SBOX32_SMHASHER_DESCR \
    "Yves Orton's 32 bit substitution box hash for up to 128 char strings, 128 bit seed"

void stadtx_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void stadtx_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

void beagle_hash_with_state_32_128_a_smhasher ( const void *blob, const int len, const void *seed, void *out );
void beagle_hash_with_state_64_128_a_smhasher ( const void *blob, const int len, const void *seed, void *out );
void beagle_seed_state_128_a_smhasher ( const int in_bits, const void *seed, void *state );

void zaphod32_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void zaphod32_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

void zaphod64_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void zaphod64_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out);

void phat_seed_state_smhasher_test(int in_bits, const void *seed, void *state);
void phat_hash_with_state_smhasher_test (const void *key, STRLEN len, const void *seed, void *out);

void sbox32_seed_state_smhasher(int in_bits, const void *seed, void *state);
void sbox32_hash_with_state_smhasher(const void *key, STRLEN len, const void *seed, void *out);

#ifdef __cplusplus
}
#endif


#endif
