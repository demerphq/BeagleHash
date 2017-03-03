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

#define ZAPHOD64_MIX(v0,v1,v2,text) STMT_START {                            \
    WARN4("v0=%016lx v1=%016lx v2=%016lx - ZAPHOD64 %s MIX\n",              \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2, text );  \
    v0 = ROTL_64(v0, 8) + v1;   \
    v1 = ROTL_64(v1,23) ^ v2;   \
    v2 = ROTL_64(v2,53) ^ v0;   \
    v0 = ROTR_64(v0,48) - v2;   \
    v2 = ROTR_64(v2,47) + v1;   \
    v1 = ROTL_64(v1,29) - v0;   \
    v0 = ROTL_64(v0,29) ^ v2;   \
    v2 = ROTR_64(v2,49) + v0;   \
} STMT_END

#define ZAPHOD64_FINALIZE(v0,v1,v2) STMT_START { \
    v0 = ROTR_64(v0,61) - v2;   \
    v2 ^= v1;                   \
    v2 = ROTL_64(v2, 8) - v0;   \
    v1 -= v2;                   \
    v0 = ROTR_64(v0,23) - v2;   \
    v1 = ROTL_64(v1,11);        \
    v0 -= v1;                   \
    v1 = ROTL_64(v1,11);        \
    v0 -= v1;                   \
    v0 = ROTR_64(v0,32);        \
    v2 = ROTR_64(v2, 7) - v0;   \
    v1 = ROTR_64(v1,61) + v0;   \
    v2 = ROTL_64(v2, 9);        \
    v1 -= v2;                   \
    v1 = ROTR_64(v1,19);        \
    v2 ^= v1;                   \
    v1 -= v0;                   \
    v2 = ROTR_64(v2,40);        \
    v1 ^= v2;                   \
    v1 = ROTL_64(v1,47) - v2;   \
    v0 = ROTR_64(v0,49) ^ v1;   \
    v0 = ROTL_64(v0,57);        \
    v1 ^= v0;                   \
    v1 = ROTL_64(v1,25);        \
    v2 -= v1;                   \
    v2 = ROTR_64(v2,37);        \
    v1 = ~v1;                   \
} STMT_END

BEAGLE_STATIC_INLINE void sbox_seed_state (
    const U8 *seed_ch,
    const U8 *state_ch
) {
    U64 *seed= (U64 *)seed_ch;
    U64 *state= (U64 *)state_ch;
    U64 s0= seed[0];
    U64 s1= seed[1];

    int i,j,count;
    if ( s0 + s1 == 0 )
        s0 = 1;
    for( i = 0 ; i < 32 ; i++ ) {
        for ( j = 0 ; j < 50 ; j++ ) {
            XORSHIFT128plus(s0,s1);
        }
        for ( j = 0; j <= 255; j++ ) {
            XORSHIFT128plus(s0,s1);
            *state = s0 + s1;
            state++;
            count++;
        }
    }
    for ( i = 0; i < 3; i++ ) {
        XORSHIFT128plus(s0,s1);
        *state = s0 + s1;
        state++;
        count++;
    }
}

BEAGLE_STATIC_INLINE U64 sbox_hash_with_state(
    const U8 *state_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 *state= (U64 *)state_ch;
    U64 *end_state= state + (256 * 32 + 3);
    U64 v2= state[0] + key_len + 1;
    U64 v1= state[1];
    U64 v0= state[2];
    U64 len= key_len;
    U64 hash;
    const U8 *end_key= key + key_len;
    state+=3;

    v2 += key_len + 1;
    SCRAMBLE64(v2, 0xc0f9edd07d89152fUL);
    while ( key < end_key && state < end_state ) {
        v2 ^= state[*key];
        key++;
        state += 256;
    }
    v2 += key_len + 1;
    SCRAMBLE64(v2, 0xace809255f57529bUL);


    if (key < end_key) {
        len = end_key - key;
        v1 += key_len + 2;
        v2 += key_len + 3;
        SCRAMBLE64(v1, 0xc0f9edd07d89152fUL);
        SCRAMBLE64(v2, 0xbb83cd5dd6f450b9UL);
        WARN4("v0=%16lx v1=%16lx v2=%16lx ln=%16lx HASH START\n",
                (unsigned long)state[0], (unsigned long)state[1],
                (unsigned long)state[2], (unsigned long)key_len);
        while ( len >= 32 ) {
            v1 -= U8TO64_LE(key); key += 8;
            v0 += U8TO64_LE(key); key += 8;
            ZAPHOD64_MIX(v0,v1,v2,"MIX 2-WORDS A");
            v1 -= U8TO64_LE(key); key += 8;
            v0 += U8TO64_LE(key); key += 8;
            ZAPHOD64_MIX(v0,v1,v2,"MIX 2-WORDS B");
            len -= 32;
        }
        switch ( len >> 3 ) {
            case 3:
                    v1 -= U8TO64_LE(key); key += 8;
                    v0 += U8TO64_LE(key); key += 8;
                    ZAPHOD64_MIX(v0,v1,v2,"MIX 2-WORDS C");
            case 1:
                    v1 -= U8TO64_LE(key); key += 8;
                    break;
            case 2:
                    v1 -= U8TO64_LE(key); key += 8;
                    v0 += U8TO64_LE(key); key += 8;
                    ZAPHOD64_MIX(v0,v1,v2,"MIX 2-WORDS D");
                    break;
            case 0:
            default: break;
        }
        v0 += ((U64)(key_len+1) << 56);
        switch (len & 0x7) {
            case 7: v0 += (U64)key[6] << 48;
            case 6: v0 += (U64)key[5] << 40;
            case 5: v0 += (U64)key[4] << 32;
            case 4: v0 += *((U32 *)key);
                    break;
            case 3: v0 += (U64)key[2] << 16;
            case 2: v0 += *((U16 *)key);
                    break;
            case 1: v0 += (U64)key[0];
                    break;
            case 0:
            default:v2 ^= 0xFF;
                    break;
        }
        ZAPHOD64_FINALIZE(v0,v1,v2);
        hash = v0 ^ v1 ^ v2;
        WARN4("v0=%16lx v1=%16lx v2=%16lx hh=%16lx - FINAL\n\n",
                (unsigned long)v0, (unsigned long)v1, (unsigned long)v2,
                (unsigned long)hash);
    } else {
        hash = v2;
    }

    return hash;
}

BEAGLE_STATIC_INLINE U64 sbox_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 state[32 * 256 + 3];
    sbox_seed_state(seed_ch,(U8*)state);
    return sbox_hash_with_state((U8*)state,key,key_len);
}

void sbox_hash_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U64 seed[2]= { seed_base, 0, };
    *((U64 *)out)= sbox_hash((U8*)seed, (U8 *)key, len);
}

void sbox_seed_state_smhasher_test(int in_bits, const void *seed, const void *state) {
    sbox_seed_state((U8*)seed,(U8*)state);
}

void sbox_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *state, void *out) {
    *((U64 *)out)= sbox_hash_with_state((U8*)state, (U8 *)key, len);
}

#ifdef __cplusplus
}
#endif

