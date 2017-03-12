#ifdef __cplusplus
extern "C" {
#endif

#include "stadtx_hash.h"
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


BEAGLE_STATIC_INLINE void stadtx_seed_state (
    const U8 *seed_ch,
    const U8 *state_ch
) {
    U64 *seed= (U64 *)seed_ch;
    U64 *state= (U64 *)state_ch;
    state[0]= seed[0] ^ 0xdd51e5d1c9a5a151UL;
    state[1]= seed[1] ^ 0x49bd91d595cd41bdUL;
    state[2]= seed[0] ^ 0xbda10db5bd91b985UL;
    state[3]= seed[1] ^ 0xcd95d1e509b995cdUL;
    if (!state[0]) state[0]=1;
    if (!state[1]) state[1]=2;
    if (!state[2]) state[2]=4;
    if (!state[3]) state[3]=8;
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

BEAGLE_STATIC_INLINE U64 stadtx_hash_with_state(
    const U8 *state_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 *state= (U64 *)state_ch;
    U64 v0= state[0] ^ ((key_len+1) * k0_U64);
    U64 v1= state[1] ^ ((key_len+2) * k1_U64);
    U64 v2= state[2];
    U64 v3= state[3];
    U64 len = key_len;

    while ( len >= 32 ) {
        U64 t0= v0;
        U64 t1= v1;
        U64 t2= v2;
        U64 t3= v3;
        v0 += ((U64)U8TO64_LE(key+ 0) * k2_U32); v0= ROTL_64(v0,31) ^ t3;
        v1 += ((U64)U8TO64_LE(key+ 8) * k3_U32); v1= ROTR_64(v1,43) ^ t2;
        v2 += ((U64)U8TO64_LE(key+16) * k4_U32); v2= ROTL_64(v2,17) ^ t0;
        v3 += ((U64)U8TO64_LE(key+24) * k5_U32); v3= ROTR_64(v3,61) ^ t1;
        key += 32;
        len -= 32;
    }
    switch ( len >> 3 ) {
        case 3: v0 += ((U64)U8TO64_LE(key) * k2_U32); key += 8; v0= ROTL_64(v0,31) ^ v3; 
        case 2: v1 += ((U64)U8TO64_LE(key) * k3_U32); key += 8; v1= ROTR_64(v1,43) ^ v2; 
        case 1: v2 += ((U64)U8TO64_LE(key) * k4_U32); key += 8; v2= ROTL_64(v2,17) ^ v0;
        case 0: break;
    }

    switch ( len & 0x7 ) {
        case 7: v0 += (U64)key[6] * k2_U32; 
        case 6: v1 += (U64)key[5] * k3_U32;
        case 5: v2 += (U64)key[4] * k4_U32;
        case 4: v3 += (U64)U8TO32_LE(key) * k5_U32; 
                break; 
        case 3: v0 += (U64)key[2] * k2_U32;          
        case 2: v1 += (U64)U8TO16_LE(key) * k3_U32;
                break;
        case 1: v2 += (U64)key[0] * k4_U32;          
        case 0: v3 += (U64)(len+1) * k5_U32;
                break;
    }
    if (0) {
    v0= ROTL_64(v0,31) ^ v3;
    v1= ROTR_64(v1,43) ^ v2;
    v2= ROTL_64(v2,17) ^ v0;
    v3= ROTR_64(v3,61) ^ v1;
    }
    v2 += v1;
    v1 -= v3;
    v1 = ROTL_64(v1, 1) ^ v2;
    v3 += v0;
    v0 += v1;
    v0 = ROTR_64(v0,47);
    v3 ^= v0;
    v2 = ROTL_64(v2, 7);
    v3 ^= v2;
    v1 += v3;
    v2 = ROTR_64(v2,47) + v1;
    v2 = ROTL_64(v2, 7);
    v3 ^= v2;
    v1 += v3;
    v2 = ROTR_64(v2,47) + v1;
    v3 = ROTL_64(v3,53);
    v1 -= v3;
    v3 += v2;
    v1 = ROTL_64(v1,25);
    v3 ^= v1;
    v3 = ROTL_64(v3,61);
    v1 ^= v3;
    v0 = ROTR_64(v0,55);
    v1 -= v0;
    v1 = ROTR_64(v1,48) ^ v2;
    v1 += v3;
    v0 -= v1;
    v3 = ROTR_64(v3,32) ^ v1;
    v1 = ROTR_64(v1,57);
    v2 -= v3;
    v2 = ROTR_64(v2,19);
    v3 += v2;
    v3 = ROTR_64(v3, 3);

    return v0 ^ v1 ^ v2 ^ v3;
}

BEAGLE_STATIC_INLINE U64 stadtx_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 state[4];
    stadtx_seed_state(seed_ch,(U8*)state);
    return stadtx_hash_with_state((U8*)state,key,key_len);
}

void stadtx_seed_state_smhasher_test(int in_bits, const void *seed, void *state) {
    stadtx_seed_state((U8*)seed,(U8*)state);
}

void stadtx_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *state, void *out) {
    *((U64 *)out)= stadtx_hash_with_state((U8*)state, (U8 *)key, len);
}

#ifdef __cplusplus
}
#endif

