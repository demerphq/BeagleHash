#ifndef DEBUG_ZAPHOD64_HASH
#define DEBUG_ZAPHOD64_HASH 0
#endif

#if DEBUG_ZAPHOD64_HASH == 1
#include <stdio.h>
#define ZAPHOD64_WARN6(pat,v0,v1,v2,v3,v4,v5)    printf(pat, v0, v1, v2, v3, v4, v5)
#define ZAPHOD64_WARN5(pat,v0,v1,v2,v3,v4)       printf(pat, v0, v1, v2, v3, v4)
#define ZAPHOD64_WARN4(pat,v0,v1,v2,v3)          printf(pat, v0, v1, v2, v3)
#define ZAPHOD64_WARN3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#define ZAPHOD64_WARN2(pat,v0,v1)                printf(pat, v0, v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#elif DEBUG_ZAPHOD64_HASH == 2
#define ZAPHOD64_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define ZAPHOD64_WARN5(pat,v0,v1,v2,v3,v4)
#define ZAPHOD64_WARN4(pat,v0,v1,v2,v3)
#define ZAPHOD64_WARN3(pat,v0,v1,v2)
#define ZAPHOD64_WARN2(pat,v0,v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#else
#define ZAPHOD64_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define ZAPHOD64_WARN5(pat,v0,v1,v2,v3,v4)
#define ZAPHOD64_WARN4(pat,v0,v1,v2,v3)
#define ZAPHOD64_WARN3(pat,v0,v1,v2)
#define NOTE3(pat,v0,v1,v2)
#define ZAPHOD64_WARN2(pat,v0,v1)
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

#ifndef ZAPHOD64_STATIC_INLINE
#ifdef PERL_STATIC_INLINE
#define ZAPHOD64_STATIC_INLINE PERL_STATIC_INLINE
#else
#define ZAPHOD64_STATIC_INLINE static inline
#endif
#endif

#ifndef STMT_START
#define STMT_START do
#define STMT_END while(0)
#endif

#ifndef ZAPHOD64_UNALIGNED_AND_LITTLE_ENDIAN
#define ZAPHOD64_UNALIGNED_AND_LITTLE_ENDIAN 1
#endif

#if ZAPHOD64_ALLOW_UNALIGNED_AND_LITTLE_ENDIAN
  #ifndef U8TO64_LE
    #define U8TO64_LE(ptr)  (*((const U64 *)(ptr)))
  #endif
  #ifndef U8TO32_LE
    #define U8TO32_LE(ptr)  (*((const U32 *)(ptr)))
  #endif
  #ifndef U8TO16_LE
    #define U8TO16_LE(ptr)  (*((const U16 *)(ptr)))
  #endif
#else
  #ifndef U8TO64_LE
    #define U8TO64_LE(ptr)  (\
        (U64)(ptr)[7] << 56 | \
        (U64)(ptr)[6] << 48 | \
        (U64)(ptr)[5] << 40 | \
        (U64)(ptr)[4] << 32 | \
        (U64)(ptr)[3] << 24 | \
        (U64)(ptr)[2] << 16 | \
        (U64)(ptr)[1] << 8  | \
        (U64)(ptr)[0]         \
    )
  #endif
  #ifndef U8TO32_LE
    #define U8TO32_LE(ptr)  (\
        (U32)(ptr)[3] << 24 | \
        (U32)(ptr)[2] << 16 | \
        (U32)(ptr)[1] << 8  | \
        (U32)(ptr)[0]         \
    )
  #endif
  #ifndef U8TO16_LE
    #define U8TO16_LE(ptr)  (\
        (U16)(ptr)[1] << 8  | \
        (U16)(ptr)[0]         \
    )
  #endif
#endif

#define ZAPHOD64_SCRAMBLE64(v,prime) STMT_START {  \
    v ^= (v>>13);                        \
    v ^= (v<<35);                       \
    v ^= (v>>30);                       \
    v *= prime;                         \
    v ^= (v>>19);                       \
    v ^= (v<<15);                       \
    v ^= (v>>46);                       \
} STMT_END

#define ZAPHOD64_MIX(v0,v1,v2,text) STMT_START {                            \
    ZAPHOD64_WARN4("v0=%016lx v1=%016lx v2=%016lx - ZAPHOD64 %s MIX STEP 1\n",       \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2, text );  \
    v0 = ROTL64(v0,57) + v1;   \
    v1 = ROTL64(v1,43) ^ v2;   \
    v2 = ROTL64(v2,24) ^ v0;   \
    v0 = ROTR64(v0, 5) + v2;   \
    v2 = ROTR64(v2, 8) - v1;   \
    v1 = ROTR64(v1,63) ^ v0;   \
    v0 = ROTL64(v0,17) ^ v2;   \
    v2 = ROTL64(v2,55) - v0;   \
} STMT_END

#define ZAPHOD64_FINALIZE(v0,v1,v2) STMT_START { \
    ZAPHOD64_WARN3("v0=%016lx v1=%016lx v2=%016lx - ZAPHOD64 ENTER FINALIZE\n",     \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2 );                \
    v0 = ROTR64(v0,61) - v2;    \
    v2 ^= v1;                   \
    v2 = ROTL64(v2, 8) - v0;    \
    v1 -= v2;                   \
    v0 = ROTR64(v0,23) - v2;    \
    v1 = ROTL64(v1,11);         \
    v0 -= v1;                   \
    v1 = ROTL64(v1,11);         \
    v0 -= v1;                   \
    v0 = ROTR64(v0,32);         \
    v2 = ROTR64(v2, 7) - v0;    \
    v1 = ROTR64(v1,61) + v0;    \
    v2 = ROTL64(v2, 9);         \
    v1 -= v2;                   \
    v1 = ROTR64(v1,19);         \
    ZAPHOD64_WARN3("v0=%016lx v1=%016lx v2=%016lx - ZAPHOD64 MID FINALIZE\n",   \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2 );            \
    v2 ^= v1;                   \
    v1 -= v0;                   \
    v2 = ROTR64(v2,40);         \
    v1 ^= v2;                   \
    v1 = ROTL64(v1,47) - v2;    \
    v0 = ROTR64(v0,49) ^ v1;    \
    v0 = ROTL64(v0,57);         \
    v1 ^= v0;                   \
    v1 = ROTL64(v1,25);         \
    v2 -= v1;                   \
    v2 = ROTR64(v2,37);         \
    v1 = ~v1;                   \
} STMT_END


ZAPHOD64_STATIC_INLINE void zaphod64_seed_state (
    const U8 *seed_ch,
    const U8 *state_ch
) {
    U64 *seed= (U64 *)seed_ch;
    U64 *state= (U64 *)state_ch;
    /* hex expansion of pi, skipping first two digits. pi= 3.2[43f6...]*/
    /* pi value in hex from here:
     * http://turner.faculty.swau.edu/mathematics/materialslibrary/pi/pibases.html*/
    state[0]= seed[0] ^ 0x43f6a8885a308d31UL;
    state[1]= seed[1] ^ 0x3198a2e03707344aUL;
    state[2]= seed[2] ^ 0x4093822299f31d00UL;
    if (!state[0]) state[0] = 1;
    if (!state[1]) state[1] = 2;
    if (!state[2]) state[2] = 4;
    /* these are pseduo-randomly selected primes between 2**63 and 2**64
     * (I generated a big list and then randomly chose some from the list) */
    ZAPHOD64_SCRAMBLE64(state[0],0x801178846e899d17UL);
    ZAPHOD64_SCRAMBLE64(state[1],0x803340f36895c2b5UL);
    ZAPHOD64_SCRAMBLE64(state[2],0x80445170f5f2e0b1UL);
    ZAPHOD64_SCRAMBLE64(state[0],0x9c1b8e1e9628323fUL);
    ZAPHOD64_SCRAMBLE64(state[1],0xa52a78f6dea653c1UL);
    ZAPHOD64_SCRAMBLE64(state[2],0xd0959cc6bf8d866dUL);

    ZAPHOD64_WARN3("v0=%016lx v1=%016lx v2=%016lx - ZAPHOD64 SEED-STATE FINAL\n",
            (unsigned long)state[0], (unsigned long)state[1], (unsigned long)state[2]);
}

ZAPHOD64_STATIC_INLINE U64 zaphod64_hash_with_state(
    const U8 *state_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 *state= (U64 *)state_ch;
    U64 v0= state[0] ^ (0xb89b0f8e1655514fUL * (key_len + 3));
    U64 v1= state[1] ^ (0x8c6f736011bd5127UL * (key_len + 2));
    U64 v2= state[2] ^ (0xc0f9edd07d89152fUL * (key_len + 1));
    U64 len= key_len;
    U64 hash;

    ZAPHOD64_WARN4("v0=%016lx v1=%016lx v2=%016lx ln=%016lx - ZAPHOD64 HASH START\n",
            (unsigned long)state[0], (unsigned long)state[1],
            (unsigned long)state[2], (unsigned long)key_len);

    while ( len >= 16 ) {
        ZAPHOD64_WARN2("m0=%016lx m1=%016lx - ZAPHOD64 READ 2-WORDS A\n",
            U8TO64_LE(key),U8TO64_LE(key+8));
        v1 -= U8TO64_LE(key); key += 8;
        v0 += U8TO64_LE(key); key += 8;
        ZAPHOD64_MIX(v0,v1,v2,"MIX 2-WORDS A");
        len -= 16;
    }

    if ( len >= 8 ) {
        v1 -= U8TO64_LE(key); key += 8;
    }

    v0 += ((U64)(key_len+1) << 56);
    switch (len & 0x7) {
        case 7: v0 += (U64)key[6] << 48;
        case 6: v0 += (U64)U8TO16_LE(key+4) << 32;
                v0 += (U64)U8TO32_LE(key);
                break;
        case 5: v0 += (U64)key[4] << 32;
        case 4: v0 += (U64)U8TO32_LE(key);
                break;
        case 3: v0 += (U64)key[2] << 16;
        case 2: v0 += (U64)U8TO16_LE(key);
                break;
        case 1: v0 += (U64)key[0];
                break;
        case 0:
        default:v2 ^= 0xFF;
                break;
    }

    ZAPHOD64_FINALIZE(v0,v1,v2);
    hash = v0 ^ v1 ^ v2;

    ZAPHOD64_WARN4("v0=%016lx v1=%016lx v2=%016lx hh=%016lx - ZAPHOD64 FINAL\n\n",
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2,
            (unsigned long)hash);
    return hash;
}

ZAPHOD64_STATIC_INLINE U64 zaphod64_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 state[3];
    zaphod64_seed_state(seed_ch,(U8*)state);
    return zaphod64_hash_with_state((U8*)state,key,key_len);
}


