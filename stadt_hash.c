#ifdef __cplusplus
extern "C" {
#endif

#include "beagle_hash.h"
#include <stdio.h>

#define SCRAMBLE64(v,prime) STMT_START {  \
    v ^= (v>>13);                        \
    v ^= (v<<35);                       \
    v ^= (v>>30);                       \
    v *= prime;                         \
    v ^= (v>>19);                       \
    v ^= (v<<15);                       \
    v ^= (v>>46);                       \
} STMT_END


BEAGLE_STATIC_INLINE void stadt64_seed_state (
    const U8 *seed_ch,
    const U8 *state_ch
) {
    U64 *seed= (U64 *)seed_ch;
    U64 *state= (U64 *)state_ch;
    state[0]= seed[0] ^ 0xad1e48c158bd9789UL;
    state[1]= seed[1] ^ 0x89c9cd2ca7802fe9UL;
    state[2]= seed[0];
    state[3]= seed[1];
    if (!state[0]) state[0]= 1;
    if (!state[1]) state[1]= 1;
    if (!state[2]) state[2]= 1;
    if (!state[3]) state[3]= 1;
    SCRAMBLE64(state[0],0x801178846e899d17UL);
    SCRAMBLE64(state[1],0x803340f36895c2b5UL);
    SCRAMBLE64(state[2],0xbea9344eb7565eebUL);
    SCRAMBLE64(state[3],0x9999791977e30c13UL);
}

#define k0_U64 0xb89b0f8e1655514fUL
#define k1_U64 0x8c6f736011bd5127UL
#define k2_U32 0xa9d0d75b
#define k3_U32 0xb6fcd21d
#define k4_U32 0xc664f39d
#define k5_U32 0xde0784f1

BEAGLE_STATIC_INLINE U64 stadt64_hash_with_state(
    const U8 *state_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 *state= (U64 *)state_ch;
    U64 v0= state[0] ^ (k0_U64 * (key_len + 3));
    U64 v1= state[1] ^ (k1_U64 * (key_len + 2));
    U64 v2= state[0] - (key_len + 1);
    U64 v3= state[1] - (key_len + 0);
    U64 t0= v0;
    U64 t1= v1;
    U64 t2= v2;
    U64 t3= v3;
    U64 len= key_len;

    while ( len >= 32 ) {
        v0 += U8TO64_LE(key) * k2_U32; key += 8; v0= ROTR_64(v0,7)  ^ t3;
        v1 += U8TO64_LE(key) * k3_U32; key += 8; v1= ROTR_64(v1,11) ^ t2;
        v2 += U8TO64_LE(key) * k4_U32; key += 8; v2= ROTR_64(v2,13) ^ t0;
        v3 += U8TO64_LE(key) * k5_U32; key += 8; v3= ROTR_64(v3,19) ^ t1;
        t0 = v0; t1 = v1; t2 = v2; t3 = v3;
        len -= 32;
    }
    switch ( len >> 3 ) {
        case 3: v0 += U8TO64_LE(key) * k2_U32; key += 8; v0= ROTR_64(v0, 7) ^ t3; 
        case 2: v1 += U8TO64_LE(key) * k3_U32; key += 8; v1= ROTR_64(v1,11) ^ t2; 
        case 1: v2 += U8TO64_LE(key) * k4_U32; key += 8; v2= ROTR_64(v2,13) ^ t0;
        case 0:                                          v3= ROTR_64(v3,19) ^ t1;
        default: break;
    }
    t0 = v0; t1 = v1; t2 = v2; t3= v3;
    switch (len & 0x7) {
        case 7: v0 += (U64)key[6]   * k2_U32; v0= ROTR_64(v0, 7) ^ t3;
        case 6: v1 += (U64)key[5]   * k3_U32; v1= ROTR_64(v1,11) ^ t2; 
        case 5: v2 += (U64)key[4]   * k4_U32; v2= ROTR_64(v2,13) ^ t0;
        case 4: v3 += *((U32 *)key) * k5_U32; v3= ROTR_64(v3,19) ^ t1;
                break;
        case 3: v0 += (U64)key[2]   * k2_U32; v0= ROTR_64(v0, 7) ^ t3;
        case 2: v1 += *((U16 *)key) * k5_U32; v1= ROTR_64(v1,11) ^ t2;
                break;
        case 1: v2 += (U64)key[0]   * k4_U32; v2= ROTR_64(v2,13) ^ t0;
                break;
        case 0:
        default:v2 ^= 0xFF;
                break;
    }

    v1 -= v3;
    v2 -= v1;
    v0 = ROTR_64(v0,39);
    v3 -= v0;
    v1 = ROTR_64(v1,37) - v2;
    v2 -= v0;
    v3 = ROTL_64(v3,23) ^ v1;
    
    v0 = ROTL_64(v0,59) - v2;
    v1 = ROTL_64(v1, 5) + v3;
    v2 = ROTL_64(v2,53) ^ v1;
    v3 = ROTR_64(v3,15) + v0;
    v1 = ROTL_64(v1, 3) + v2;
    v3 ^= v1;
    
    v2 = ROTL_64(v2,55);
    v0 = ROTL_64(v0,53) ^ v2;
    v1 = ROTL_64(v1,11) ^ v3;
    v2 -= v1;
    v3 += v0;
    
    v0 = ROTL_64(v0,33) - v3;
    v1 = ROTL_64(v1,47) - v2;
    v2 = ROTL_64(v2, 9) ^ v0;
    v3 = ROTR_64(v3,57) ^ v1;
    
    v0 = ROTR_64(v0,31) + v2;
    v1 = ROTL_64(v1,23) + v3;
    v2 = ROTL_64(v2,41);
    v3 = ROTL_64(v3,32);

    return v0 ^ v1 ^ v2 ^ v3;
}

BEAGLE_STATIC_INLINE U64 stadt64_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 state[3];
    stadt64_seed_state(seed_ch,(U8*)state);
    return stadt64_hash_with_state((U8*)state,key,key_len);
}

void stadt64_seed_state_smhasher_test(int in_bits, const void *seed, const void *state) {
    stadt64_seed_state((U8*)seed,(U8*)state);
}

void stadt64_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *state, void *out) {
    *((U64 *)out)= stadt64_hash_with_state((U8*)state, (U8 *)key, len);
}

#ifdef __cplusplus
}
#endif

