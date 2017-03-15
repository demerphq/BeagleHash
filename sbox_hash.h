#ifndef DEBUG_SBOX_HASH
#define DEBUG_SBOX_HASH 0
#endif

#if DEBUG_SBOX_HASH == 1
#include <stdio.h>
#define SBOX_WARN6(pat,v0,v1,v2,v3,v4,v5)    printf(pat, v0, v1, v2, v3, v4, v5)
#define SBOX_WARN5(pat,v0,v1,v2,v3,v4)       printf(pat, v0, v1, v2, v3, v4)
#define SBOX_WARN4(pat,v0,v1,v2,v3)          printf(pat, v0, v1, v2, v3)
#define SBOX_WARN3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#define SBOX_WARN2(pat,v0,v1)                printf(pat, v0, v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#elif DEBUG_SBOX_HASH == 2
#define SBOX_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define SBOX_WARN5(pat,v0,v1,v2,v3,v4)
#define SBOX_WARN4(pat,v0,v1,v2,v3)
#define SBOX_WARN3(pat,v0,v1,v2)
#define SBOX_WARN2(pat,v0,v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#else
#define SBOX_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define SBOX_WARN5(pat,v0,v1,v2,v3,v4)
#define SBOX_WARN4(pat,v0,v1,v2,v3)
#define SBOX_WARN3(pat,v0,v1,v2)
#define NOTE3(pat,v0,v1,v2)
#define SBOX_WARN2(pat,v0,v1)
#endif

#ifndef ROTL64
#define _ROTL_SIZED(x,r,s) ( ((x) << (r)) | ((x) >> ((s) - (r))) )
#define _ROTR_SIZED(x,r,s) ( ((x) << ((s) - (r))) | ((x) >> (r)) )
#define ROTL64(x,r) _ROTL_SIZED(x,r,64)
#define ROTR64(x,r) _ROTR_SIZED(x,r,64)
#endif

#ifndef PERL_SEEN_HV_FUNC_H
#if !defined(U64)
    #include <stdint.h>
    #define U64 uint64_t
#endif

#if !defined(U32)
  #define U32 uint32_t
#endif

#if !defined(U8)
    #define U8 unsigned char
#endif

#if !defined(U16)
    #define U16 uint16_t
#endif

#ifndef STRLEN
#define STRLEN int
#endif
#endif

#ifndef SBOX_STATIC_INLINE
#ifdef PERL_STATIC_INLINE
#define SBOX_STATIC_INLINE PERL_STATIC_INLINE
#else
#define SBOX_STATIC_INLINE static inline
#endif
#endif

#ifndef STMT_START
#define STMT_START do
#define STMT_END while(0)
#endif

#ifndef U8TO64_LE
#define U8TO64_LE(ptr)  (*((const U64 *)(ptr)))
#endif
#ifndef U8TO32_LE
#define U8TO32_LE(ptr)  (*((const U32 *)(ptr)))
#endif
#ifndef U8TO16_LE
#define U8TO16_LE(ptr)  (*((const U16 *)(ptr)))
#endif
#define SBOX_SCRAMBLE64(v,prime) STMT_START {  \
    v ^= (v>>13);                        \
    v ^= (v<<35);                       \
    v ^= (v>>30);                       \
    v *= prime;                         \
    v ^= (v>>19);                       \
    v ^= (v<<15);                       \
    v ^= (v>>46);                       \
} STMT_END

#define SBOX_XORSHIFT128plus(s0,s1) STMT_START {         \
    U64 x = s0;                                     \
    U64 const y = s1;                               \
    s0 = y;                                         \
    x ^= x << 23; /* a */                           \
    s1 = x ^ y ^ (x >> 17) ^ (y >> 26); /* b, c */  \
} STMT_END

#define SBOX_MIX(v0,v1,v2,text) STMT_START {                            \
    SBOX_WARN4("v0=%016lx v1=%016lx v2=%016lx - SBOX %s MIX\n",              \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2, text );  \
    v0 = ROTL64(v0, 8) + v1;   \
    v1 = ROTL64(v1,23) ^ v2;   \
    v2 = ROTL64(v2,53) ^ v0;   \
    v0 = ROTR64(v0,48) - v2;   \
    v2 = ROTR64(v2,47) + v1;   \
    v1 = ROTL64(v1,29) - v0;   \
    v0 = ROTL64(v0,29) ^ v2;   \
    v2 = ROTR64(v2,49) + v0;   \
} STMT_END

#define SBOX_FINALIZE(v0,v1,v2) STMT_START { \
    v0 = ROTR64(v0,61) - v2;   \
    v2 ^= v1;                   \
    v2 = ROTL64(v2, 8) - v0;   \
    v1 -= v2;                   \
    v0 = ROTR64(v0,23) - v2;   \
    v1 = ROTL64(v1,11);        \
    v0 -= v1;                   \
    v1 = ROTL64(v1,11);        \
    v0 -= v1;                   \
    v0 = ROTR64(v0,32);        \
    v2 = ROTR64(v2, 7) - v0;   \
    v1 = ROTR64(v1,61) + v0;   \
    v2 = ROTL64(v2, 9);        \
    v1 -= v2;                   \
    v1 = ROTR64(v1,19);        \
    v2 ^= v1;                   \
    v1 -= v0;                   \
    v2 = ROTR64(v2,40);        \
    v1 ^= v2;                   \
    v1 = ROTL64(v1,47) - v2;   \
    v0 = ROTR64(v0,49) ^ v1;   \
    v0 = ROTL64(v0,57);        \
    v1 ^= v0;                   \
    v1 = ROTL64(v1,25);        \
    v2 -= v1;                   \
    v2 = ROTR64(v2,37);        \
    v1 = ~v1;                   \
} STMT_END

SBOX_STATIC_INLINE void sbox_seed_state (
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
            SBOX_XORSHIFT128plus(s0,s1);
        }
        for ( j = 0; j <= 255; j++ ) {
            SBOX_XORSHIFT128plus(s0,s1);
            *state = s0 + s1;
            state++;
            count++;
        }
    }
    for ( i = 0; i < 3; i++ ) {
        SBOX_XORSHIFT128plus(s0,s1);
        *state = s0 + s1;
        state++;
        count++;
    }
}

SBOX_STATIC_INLINE U64 sbox_hash_with_state(
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
    SBOX_SCRAMBLE64(v2, 0xc0f9edd07d89152fUL);
    while ( key < end_key && state < end_state ) {
        v2 ^= state[*key];
        key++;
        state += 256;
    }
    v2 += key_len + 1;
    SBOX_SCRAMBLE64(v2, 0xace809255f57529bUL);


    if (key < end_key) {
        len = end_key - key;
        v1 += key_len + 2;
        v2 += key_len + 3;
        SBOX_SCRAMBLE64(v1, 0xc0f9edd07d89152fUL);
        SBOX_SCRAMBLE64(v2, 0xbb83cd5dd6f450b9UL);
        SBOX_WARN4("v0=%16lx v1=%16lx v2=%16lx ln=%16lx HASH START\n",
                (unsigned long)state[0], (unsigned long)state[1],
                (unsigned long)state[2], (unsigned long)key_len);
        while ( len >= 32 ) {
            v1 -= U8TO64_LE(key); key += 8;
            v0 += U8TO64_LE(key); key += 8;
            SBOX_MIX(v0,v1,v2,"MIX 2-WORDS A");
            v1 -= U8TO64_LE(key); key += 8;
            v0 += U8TO64_LE(key); key += 8;
            SBOX_MIX(v0,v1,v2,"MIX 2-WORDS B");
            len -= 32;
        }
        switch ( len >> 3 ) {
            case 3:
                    v1 -= U8TO64_LE(key); key += 8;
                    v0 += U8TO64_LE(key); key += 8;
                    SBOX_MIX(v0,v1,v2,"MIX 2-WORDS C");
            case 1:
                    v1 -= U8TO64_LE(key); key += 8;
                    break;
            case 2:
                    v1 -= U8TO64_LE(key); key += 8;
                    v0 += U8TO64_LE(key); key += 8;
                    SBOX_MIX(v0,v1,v2,"MIX 2-WORDS D");
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
        SBOX_FINALIZE(v0,v1,v2);
        hash = v0 ^ v1 ^ v2;
        SBOX_WARN4("v0=%16lx v1=%16lx v2=%16lx hh=%16lx - FINAL\n\n",
                (unsigned long)v0, (unsigned long)v1, (unsigned long)v2,
                (unsigned long)hash);
    } else {
        hash = v2;
    }

    return hash;
}

SBOX_STATIC_INLINE U64 sbox_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 state[32 * 256 + 3];
    sbox_seed_state(seed_ch,(U8*)state);
    return sbox_hash_with_state((U8*)state,key,key_len);
}


