#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "BeagleHashes_test.h"
#include "beagle_hash.h"
#include "stadtx_hash.h"
#include "phat_hash.h"
#include "sbox32_hash.h"
#include "zaphod32_hash.h"
#include "zaphod64_hash.h"

void beagle_hash_with_state_32_128_a_smhasher(const void *key, STRLEN len, const void *state, void *out) {
    *((U32 *)out)= beagle_hash_with_state_32_128_a((U8*)state, (U8 *)key, len);
}

void beagle_hash_with_state_64_128_a_smhasher(const void *key, STRLEN len, const void *state, void *out) {
    *((U64 *)out)= beagle_hash_with_state_64_128_a((U8*)state, (U8 *)key, len);
}

void beagle_seed_state_128_a_smhasher( const int in_bits, const void *seed_base, void *state ) {
    switch( in_bits ) {
        case  64:
            beagle_seed_state_64_128_a((U8*)seed_base,(U8*)state);
            break;
        case  96:
            beagle_seed_state_96_128_a((U8*)seed_base,(U8*)state);
            break;
        case 112:
            beagle_seed_state_112_128_a((U8*)seed_base,(U8*)state);
            break;
        case 127:
            beagle_seed_state_127_128_a((U8*)seed_base,(U8*)state);
            break;
        case 128:
            beagle_seed_state_128_128_a((U8*)seed_base,(U8*)state);
            break;
        default:
            printf("Don't know how to prepare %d bits in beagle_seed_state_smhasher()",
                    in_bits);
            exit(0);
    }
}

void stadtx_seed_state_smhasher_test(int in_bits, const void *seed, void *state) {
    stadtx_seed_state((U8*)seed,(U8*)state);
}

void stadtx_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *state, void *out) {
    *((U64 *)out)= stadtx_hash_with_state((U8*)state, (U8 *)key, len);
}

void zaphod32_seed_state_smhasher_test(int in_bits, const void *seed, void *state) {
    zaphod32_seed_state((U8*)seed,(U8*)state);
}

void zaphod32_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out) {
    *((U32 *)out)= zaphod32_hash_with_state((U8*)seed, (U8 *)key, len);
}

void zaphod64_seed_state_smhasher_test(int in_bits, const void *seed, void *state) {
    zaphod64_seed_state((U8*)seed,(U8*)state);
}

void zaphod64_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *state, void *out) {
    *((U64 *)out)= zaphod64_hash_with_state((U8*)state, (U8 *)key, len);
}

void sbox32_seed_state_smhasher(int in_bits, const void *seed, void *state) {
    if ( in_bits == 96 ) {
        sbox32_seed_state96((U8*)seed,(U8*)state);
    } else if ( in_bits == 128 ) {
        sbox32_seed_state128((U8*)seed,(U8*)state);
    } else {
        printf("Cant handle %d bit seeds in sbox32_seed_state_smhasher", in_bits);
        exit(1);
    }
}

void sbox32_hash_with_state_smhasher(const void *key, STRLEN len, const void *state, void *out) {
    *((U32 *)out)= sbox32_hash_with_state((U8*)state, (U8 *)key, len);
}

void phat_hash_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[3]= { seed_base, 0, 0 };
    *((U32 *)out)= phat_hash((U8*)seed, (U8 *)key, len);
}

void phat_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out) {
    *((U32 *)out)= phat_hash((U8*)seed, (U8 *)key, len);
}

#ifdef __cplusplus
}
#endif

