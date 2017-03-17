#ifndef BEAGLE_HASH_H
#define BEAGLE_HASH_H

#include <string.h>

#ifndef DEBUG_BEAGLE_HASH
#define DEBUG_BEAGLE_HASH 0
#endif

#if DEBUG_BEAGLE_HASH == 1
#include <stdio.h>
#define BEAGLE_WARN6(pat,v0,v1,v2,v3,v4,v5)    printf(pat, v0, v1, v2, v3, v4, v5)
#define BEAGLE_WARN5(pat,v0,v1,v2,v3,v4)       printf(pat, v0, v1, v2, v3, v4)
#define BEAGLE_WARN4(pat,v0,v1,v2,v3)          printf(pat, v0, v1, v2, v3)
#define BEAGLE_WARN3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#define BEAGLE_WARN2(pat,v0,v1)                printf(pat, v0, v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#elif DEBUG_BEAGLE_HASH == 2
#define BEAGLE_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define BEAGLE_WARN5(pat,v0,v1,v2,v3,v4)
#define BEAGLE_WARN4(pat,v0,v1,v2,v3)
#define BEAGLE_WARN3(pat,v0,v1,v2)
#define BEAGLE_WARN2(pat,v0,v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#else
#define BEAGLE_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define BEAGLE_WARN5(pat,v0,v1,v2,v3,v4)
#define BEAGLE_WARN4(pat,v0,v1,v2,v3)
#define BEAGLE_WARN3(pat,v0,v1,v2)
#define NOTE3(pat,v0,v1,v2)
#define BEAGLE_WARN2(pat,v0,v1)
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

#ifndef BEAGLE_STATIC_INLINE
#ifdef PERL_STATIC_INLINE
#define BEAGLE_STATIC_INLINE PERL_STATIC_INLINE
#else
#define BEAGLE_STATIC_INLINE static inline
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

#define BEAGLE_K0_U64 0xE48467F78A53154FULL
#define BEAGLE_K1_U64 0x932CF3531C75ABE9ULL
#define BEAGLE_K2_U64 0xA49FC191F5713E51ULL
#define BEAGLE_K3_U64 0x8A0D8D12F6396613ULL
#define BEAGLE_K4_U64 0xB7B08C3734EBCAE7ULL
#define BEAGLE_K5_U64 0xF76B6D4BF411C86BULL
#define BEAGLE_K6_U64 0xC6CF7A8BACBFE897ULL
#define BEAGLE_K7_U64 0xD54F665799843511ULL

#define BEAGLE_MIXV0_7mul(v,v0,v1,k) STMT_START {     \
    BEAGLE_WARN3("v0=%016lx v1=%016lx v=%016lx - BEAGLE_MIXV0_7mul\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v); \
    v0 -= (v * k);              \
    v1 ^= v0;                   \
    v0 = ROTL64(v0, 26) + v1;  \
    v1 = ROTL64(v1, 42) ^ v0;  \
    v0 -= v1;                   \
} STMT_END

#define BEAGLE_MIXV1_7mul(v,v0,v1,k) STMT_START {     \
    BEAGLE_WARN3("v0=%016lx v1=%016lx v=%016lx - BEAGLE_MIXV1_7mul\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v); \
    v1 += (v * k);              \
    v0 ^= v1;                   \
    v1 = ROTL64(v1, 18) - v0;  \
    v0 = ROTL64(v0, 39) ^ v1;  \
    v1 -= v0;                   \
} STMT_END

#define BEAGLE_MIXV0_8mul(v,v0,v1,k) STMT_START {     \
    BEAGLE_WARN3("v0=%016lx v1=%016lx v=%016lx - BEAGLE_MIXV0_8mul\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v); \
    v1 += (v * k);                     \
    v0 += v1;                           \
    v1 = ROTL64(v1, 32) ^ v0;          \
    v0 += v1;                           \
    v0 = ROTL64(v0, 15) ^ v1;          \
    v1 -= v0;                           \
} STMT_END

#define BEAGLE_MIXV1_8mul(v,v0,v1,k) STMT_START {     \
    BEAGLE_WARN3("v0=%016lx v1=%016lx v=%016lx - BEAGLE_MIXV1_8mul\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v); \
    v1 += (v * k);                      \
    v0 -= v1;                           \
    v1 = ROTL64(v1, 33) ^ v0;          \
    v0 += v1;                           \
    v0 = ROTL64(v0, 16) ^ v1;          \
    v1 -= v0;                           \
} STMT_END

/*
#462:eldr: 193/64546  .5-4-m7t..4T.5-8-n1.b3r-4-2a.~ 22
9.9809e-01 1.2776e-03 1.2735e-03 269!
*/
#define BEAGLE_FINALIZE(v0,v1) STMT_START { \
    v0 -= v1;                               \
    v0 = ROTL64(v0, 49);                   \
    v1 -= v0;                               \
    v1 = ROTL64(v1, 56) ^ v0;              \
    v1 = ROTL64(v1, 30);                   \
    v0 -= v1;                               \
    v0 = ROTL64(v0, 50) + v1;              \
    v0 = ROTL64(v0, 38);                   \
    v1 += v0;                               \
    v0 = ROTL64(v0, 54) ^ v1;              \
    v1 = ROTL64(v1, 37) + v0;              \
    v1 = ROTL64(v1, 37);                   \
    v0 -= v1;                               \
    v0 = ROTL64(v0, 54);                   \
    v1 ^= v0;                               \
    v1 = ROTL64(v1, 33);                   \
    v0 += v1;                               \
    v0 = ROTL64(v0, 49);                   \
} STMT_END


#define BEAGLE_MIXV0(v,v0,v1) BEAGLE_MIXV0_7mul(v,v0,v1,BEAGLE_K0_U64)
#define BEAGLE_MIXV1(v,v0,v1) BEAGLE_MIXV1_7mul(v,v0,v1,BEAGLE_K1_U64)
#define BEAGLE_MIXV2(v,v0,v1) BEAGLE_MIXV0_8mul(v,v0,v1,BEAGLE_K2_U64)
#define BEAGLE_MIXV3(v,v0,v1) BEAGLE_MIXV1_8mul(v,v0,v1,BEAGLE_K3_U64)

#define BEAGLE_SCRAMBLE64(v,prime) STMT_START {  \
    v ^= (v>>13);                        \
    v ^= (v<<35);                       \
    v ^= (v>>30);                       \
    v *= prime;                         \
    v ^= (v>>19);                       \
    v ^= (v<<15);                       \
    v ^= (v>>46);                       \
} STMT_END

#define BEAGLE_MUNGE_SEED(seed,state,bits) STMT_START {             \
    /* make sure the seed[0]=0 case does not look like 0 */         \
    state[0] = seed[0] ^ 0x933a4a3f54ba4bafUL;                      \
    /* and then make sure that if we hit the 0 we turn into 1 */    \
    if (!state[0]) state[0]=1;                                      \
    state[1]= 0;                                                    \
    /* safely copy the tail bits of the seed into the state */      \
    /* the +7 deals with the 127 case properly */                   \
    memcpy(state+1,seed+1,((bits-64)+7)/8);                         \
    /* now make the low bits be the inverse of the high bits that*/ \
    /* we can use from state[1] */                                  \
    state[1] = (state[1] << (128 - bits))                           \
            | (((~state[1]) >> (32 - (128 - bits))) & ((1UL << (128-bits))-1));                 \
    /* and then scramble the states */                              \
    BEAGLE_SCRAMBLE64(state[0],0xb37337df3d56d90bUL);                      \
    BEAGLE_SCRAMBLE64(state[1],0x819ffd05fcf65945UL);                      \
    /* and mix them too for good measure */                         \
    BEAGLE_FINALIZE(state[0],state[1]);                             \
} STMT_END

#define BEAGLE_SEED_PREP_WITH_DECL(bits,seed_ch,seed)               \
    U64 seed[2];                                                    \
    beagle_seed_state_ ## bits ## _128_a(seed_ch,(U8*)seed);        \
    BEAGLE_WARN3("v0=%016lx v1=%016lx b=%d\n",                           \
            ((unsigned long *)seed_ch)[0],                     \
            ((unsigned long *)seed_ch)[1],                     \
            bits)

BEAGLE_STATIC_INLINE U64 beagle_hash_with_state_64_128_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 *seed= (U64 *)seed_ch;
    U64 v0= (U64)seed[0] ^ (BEAGLE_K5_U64 * (key_len + 1));
    U64 v1= (U64)seed[1] ^ (BEAGLE_K6_U64 * (key_len + 2));
    U64 v;
    U64 len = key_len;

    while ( len >= 16 ) {
        v = U8TO64_LE(key); key += 8;
        BEAGLE_MIXV0(v,v0,v1);
        v = U8TO64_LE(key); key += 8;
        BEAGLE_MIXV1(v,v0,v1);
        len -= 16;
    }

    if ( len >= 8 ) {
        v = U8TO64_LE(key); key += 8;
        BEAGLE_MIXV2(v,v0,v1);
    }

    v= ((U64)key_len+1) << 56;
    switch (len & 0x7) {
        case 7: v += (U64)key[6] << 48;
        case 6: v += (U64)key[5] << 40;
        case 5: v += (U64)key[4] << 32;
        case 4: v += (U64)key[3] << 24;
        case 3: v += (U64)key[2] << 16;
        case 2: v += (U64)key[1] <<  8;
        case 1: v += (U64)key[0];
        default: break;
    }
    BEAGLE_MIXV3(v,v0,v1);

    BEAGLE_WARN3("v0=%016lx v1=%016lx   %16s - ENTER FINALIZE\n",
            (unsigned long)v0, (unsigned long)v1, "");

    BEAGLE_FINALIZE(v0,v1);

    v = v0 ^ v1;

    BEAGLE_WARN3("v0=%016lx v1=%016lx h=%016lx - FINAL\n",
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v);

    return v;
}

BEAGLE_STATIC_INLINE U32 beagle_hash_with_state_32_128_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    union {
        struct {
            U32 v0;
            U32 v1;
        } u32;
        struct {
            U64 v0;
        } u64;
    } u;

    u.u64.v0= beagle_hash_with_state_64_128_a(seed_ch,key,key_len);

    return u.u32.v1;
}


BEAGLE_STATIC_INLINE void beagle_seed_state_64_128_a(
    const U8 *seed_ch,
    U8 *seed_state_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *state= (U64 *)seed_state_ch;

    state[0] = seed_base[0] ^ 0x736f6d6570736575ull;
    state[1] = seed_base[0];
}


BEAGLE_STATIC_INLINE void beagle_seed_state_96_128_a(
    const U8 *seed_ch,
    const U8 *seed_state_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *state= (U64 *)seed_state_ch;

    BEAGLE_MUNGE_SEED(seed_base,state,96);
}

BEAGLE_STATIC_INLINE void beagle_seed_state_112_128_a(
    const U8 *seed_ch,
    const U8 *seed_state_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *state= (U64 *)seed_state_ch;

    BEAGLE_MUNGE_SEED(seed_base,state,112);
}

BEAGLE_STATIC_INLINE void beagle_seed_state_127_128_a(
    const U8 *seed_ch,
    const U8 *seed_state_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *state= (U64 *)seed_state_ch;

    BEAGLE_MUNGE_SEED(seed_base,state,127);
}

/* this is the same as 127 */
BEAGLE_STATIC_INLINE void beagle_seed_state_128_128_a(
    const U8 *seed_ch,
    const U8 *seed_state_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *state= (U64 *)seed_state_ch;

    BEAGLE_MUNGE_SEED(seed_base,state,127);
}


#define BEAGLE_HASH_DECL(ret_type, out_bits, in_bits, fmt, fmt_type)    \
BEAGLE_STATIC_INLINE ret_type beagle_hash_ ## out_bits ## _ ## in_bits ## _a(   \
    const U8 *seed_ch,                                                      \
    const U8 *key,                                                          \
    const STRLEN key_len                                                    \
) {                                                                         \
    U64 seed[2];                                                            \
    ret_type final;                                                         \
    beagle_seed_state_ ## in_bits ## _128_a(seed_ch,(U8*)seed);             \
    BEAGLE_WARN3("v0=%016lx v1=%016lx b=%d\n",                                   \
            ((unsigned long *)seed_ch)[0],                             \
            ((unsigned long *)seed_ch)[1],                             \
            in_bits);                                                       \
    final = beagle_hash_with_state_ ## out_bits ## _128_a((U8*)seed,key,key_len);     \
    NOTE3("seed-bits=%d key-len=%d final="fmt"\n\n",                       \
            in_bits, key_len, (fmt_type)final);                             \
    return final;                                                           \
}                                                                           

BEAGLE_HASH_DECL(U64,64,64,"%016lx",unsigned long)
BEAGLE_HASH_DECL(U64,64,96,"%016lx",unsigned long)
BEAGLE_HASH_DECL(U64,64,112,"%016lx",unsigned long)
BEAGLE_HASH_DECL(U64,64,127,"%016lx",unsigned long)
BEAGLE_HASH_DECL(U64,64,128,"%016lx",unsigned long)
BEAGLE_HASH_DECL(U64,32,64,"%08x",unsigned int)
BEAGLE_HASH_DECL(U64,32,96,"%08x",unsigned int)
BEAGLE_HASH_DECL(U64,32,112,"%08x",unsigned int)
BEAGLE_HASH_DECL(U64,32,127,"%08x",unsigned int)
BEAGLE_HASH_DECL(U64,32,128,"%08x",unsigned int)

#endif
