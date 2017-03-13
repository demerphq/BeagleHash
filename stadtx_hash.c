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
    state[4]= state[0] ^ seed[3];
    if (!state[0]) state[0]=1;
    if (!state[1]) state[1]=2;
    if (!state[2]) state[2]=4;
    if (!state[3]) state[3]=8;
    if (!state[4]) state[4]=16;
    SCRAMBLE64(state[0],0x801178846e899d17UL);
    SCRAMBLE64(state[1],0x803340f36895c2b5UL);
    SCRAMBLE64(state[2],0xbea9344eb7565eebUL);
    SCRAMBLE64(state[3],0x9999791977e30c13UL);
}

#define k0_U64 0xb89b0f8e1655514fUL
#define k1_U64 0x8c6f736011bd5127UL

#define k2_U64 0x8f29bd94edce7b39UL
#define k3_U64 0x9c1b8e1e9628323fUL
#define k4_U64 0xaab8b6b05abfc6cdUL
#define k5_U64 0x93a7d6c8c62e4835UL

#define k2_U32 0x802910e3
#define k3_U32 0x819b13af
#define k4_U32 0x91cb27e5
#define k5_U32 0xc1a269c1

BEAGLE_STATIC_INLINE U64 stadtx_hash_with_state(
    const U8 *state_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 *state= (U64 *)state_ch;
    U64 len = key_len;
    U64 v0= state[0] ^ (key_len+1 * k0_U64);
    U64 v1= state[1] ^ (key_len+2 * k1_U64);
    if (len < 32) {
        while (len >= 8) {
            v0 += U8TO64_LE(key) * k3_U64;
            v0= ROTR_64(v0, 17) ^ v1;
            v1= ROTR_64(v1, 53) + v0;
            key += 8;
            len -= 8;
        }
        switch ( len & 0x7 ) {
            case 7: v0 += (U64)key[6] << 32;
            case 6: v1 += (U64)key[5] << 48;
            case 5: v0 += (U64)key[4] << 16;
            case 4: v1 += (U64)U8TO32_LE(key);
                    break;
            case 3: v0 += (U64)key[2] << 48;
            case 2: v1 += (U64)U8TO16_LE(key);
                    break;
            case 1: v0 += (U64)key[0];
            case 0: v1 = ROTL_64(v1, 32) ^ 0xFF;
                    break;
        }
        v1 ^= v0;
        v0 = ROTR_64(v0,33) + v1;
        v1 = ROTL_64(v1,17) ^ v0;
        v0 = ROTL_64(v0,43) + v1;
        v1 = ROTL_64(v1,31) - v0;
        v0 = ROTL_64(v0,13) ^ v1;
        v1 -= v0;
        v0 = ROTL_64(v0,41) + v1;
        v1 = ROTL_64(v1,37) ^ v0;
        v0 = ROTR_64(v0,39) + v1;
        v1 = ROTR_64(v1,15) + v0;
        v0 = ROTL_64(v0,15) ^ v1;
        v1 = ROTR_64(v1, 5);
        return v0 ^ v1;
    } else {
        U64 v2= state[2] ^ (key_len+3 * k2_U64);
        U64 v3= state[3] ^ (key_len+4 * k3_U64);

        do {
            v0 += (U64)U8TO64_LE(key+ 0) * k2_U32; v0= ROTL_64(v0,57) ^ v3;
            v1 += (U64)U8TO64_LE(key+ 8) * k3_U32; v1= ROTL_64(v1,63) ^ v2;
            v2 += (U64)U8TO64_LE(key+16) * k4_U32; v2= ROTR_64(v2,47) + v0;
            v3 += (U64)U8TO64_LE(key+24) * k5_U32; v3= ROTR_64(v3,11) - v1;
            key += 32;
            len -= 32;
        } while ( len >= 32 );

        switch ( len >> 3 ) {
            case 3: v0 += ((U64)U8TO64_LE(key) * k2_U32); key += 8; v0= ROTL_64(v0,57) ^ v3;
            case 2: v1 += ((U64)U8TO64_LE(key) * k3_U32); key += 8; v1= ROTL_64(v1,63) ^ v2;
            case 1: v2 += ((U64)U8TO64_LE(key) * k4_U32); key += 8; v2= ROTR_64(v2,47) + v0;
            case 0: v3 = ROTR_64(v3,11) - v1;
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

        v1 -= v2;
        v0 = ROTR_64(v0,19);
        v1 -= v0;
        v1 = ROTR_64(v1,53);
        v3 ^= v1;
        v0 -= v3;
        v3 = ROTL_64(v3,43);
        v0 += v3;
        v0 = ROTR_64(v0, 3);
        v3 -= v0;
        v2 = ROTR_64(v2,43) - v3;
        v2 = ROTL_64(v2,55) ^ v0;
        v1 -= v2;
        v3 = ROTR_64(v3, 7) - v2;
        v2 = ROTR_64(v2,31);
        v3 += v2;
        v2 -= v1;
        v3 = ROTR_64(v3,39);
        v2 ^= v3;
        v3 = ROTR_64(v3,17) ^ v2;
        v1 += v3;
        v1 = ROTR_64(v1, 9);
        v2 ^= v1;
        v2 = ROTL_64(v2,24);
        v3 ^= v2;
        v3 = ROTR_64(v3,59);
        v0 = ROTR_64(v0, 1) - v1;

        return v0 ^ v1 ^ v2 ^ v3;
    }
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

