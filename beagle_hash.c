#ifdef __cplusplus
extern "C" {
#endif

#include "beagle_hash.h"
#include <stdio.h>

#define k0_U64 0xE48467F78A53154FULL
#define k1_U64 0x932CF3531C75ABE9ULL
#define k2_U64 0xA49FC191F5713E51ULL
#define k3_U64 0x8A0D8D12F6396613ULL
#define k4_U64 0xB7B08C3734EBCAE7ULL
#define k5_U64 0xF76B6D4BF411C86BULL
#define k6_U64 0xC6CF7A8BACBFE897ULL
#define k7_U64 0xD54F665799843511ULL

#define MIXV0_7mul(v,v0,v1,k) STMT_START {     \
    WARN3("v0=%016lx v1=%016lx v=%016lx - MIXV0_7mul\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v); \
    v0 -= (v * k);              \
    v1 ^= v0;                   \
    v0 = ROTL_64(v0, 26) + v1;  \
    v1 = ROTL_64(v1, 42) ^ v0;  \
    v0 -= v1;                   \
} STMT_END

#define MIXV1_7mul(v,v0,v1,k) STMT_START {     \
    WARN3("v0=%016lx v1=%016lx v=%016lx - MIXV1_7mul\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v); \
    v1 += (v * k);              \
    v0 ^= v1;                   \
    v1 = ROTL_64(v1, 18) - v0;  \
    v0 = ROTL_64(v0, 39) ^ v1;  \
    v1 -= v0;                   \
} STMT_END

#define MIXV0_8mul(v,v0,v1,k) STMT_START {     \
    WARN3("v0=%016lx v1=%016lx v=%016lx - MIXV0_8mul\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v); \
    v1 += (v * k);                     \
    v0 += v1;                           \
    v1 = ROTL_64(v1, 32) ^ v0;          \
    v0 += v1;                           \
    v0 = ROTL_64(v0, 15) ^ v1;          \
    v1 -= v0;                           \
} STMT_END

#define MIXV1_8mul(v,v0,v1,k) STMT_START {     \
    WARN3("v0=%016lx v1=%016lx v=%016lx - MIXV1_8mul\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v); \
    v1 += (v * k);                      \
    v0 -= v1;                           \
    v1 = ROTL_64(v1, 33) ^ v0;          \
    v0 += v1;                           \
    v0 = ROTL_64(v0, 16) ^ v1;          \
    v1 -= v0;                           \
} STMT_END

/*
#462:eldr: 193/64546  .5-4-m7t..4T.5-8-n1.b3r-4-2a.~ 22
9.9809e-01 1.2776e-03 1.2735e-03 269!
*/
#define BEAGLE_FINALIZE(v0,v1) STMT_START { \
    v0 -= v1;                               \
    v0 = ROTL_64(v0, 49);                   \
    v1 -= v0;                               \
    v1 = ROTL_64(v1, 56) ^ v0;              \
    v1 = ROTL_64(v1, 30);                   \
    v0 -= v1;                               \
    v0 = ROTL_64(v0, 50) + v1;              \
    v0 = ROTL_64(v0, 38);                   \
    v1 += v0;                               \
    v0 = ROTL_64(v0, 54) ^ v1;              \
    v1 = ROTL_64(v1, 37) + v0;              \
    v1 = ROTL_64(v1, 37);                   \
    v0 -= v1;                               \
    v0 = ROTL_64(v0, 54);                   \
    v1 ^= v0;                               \
    v1 = ROTL_64(v1, 33);                   \
    v0 += v1;                               \
    v0 = ROTL_64(v0, 49);                   \
} STMT_END


#define MIXV0(v,v0,v1) MIXV0_7mul(v,v0,v1,k0_U64)
#define MIXV1(v,v0,v1) MIXV1_7mul(v,v0,v1,k1_U64)
#define MIXV2(v,v0,v1) MIXV0_8mul(v,v0,v1,k2_U64)
#define MIXV3(v,v0,v1) MIXV1_8mul(v,v0,v1,k3_U64)

#define BEAGLE_MUNGE_SEED(seed,state,bits,rounds) STMT_START {      \
    int irounds = rounds;                                           \
    state[0] = seed[0];                                             \
    state[1] = (seed[1] << (128 - bits))                            \
            | ((~seed[0]) >> (64 -(128-bits)));                     \
    do {                                                            \
        BEAGLE_FINALIZE(state[0],state[1]);                         \
    } while (--irounds>0);                                          \
} STMT_END

#define BEAGLE_SEED_PREP_WITH_DECL(bits,seed_ch,seed)               \
    U64 seed[2];                                                    \
    beagle_seed_state_ ## bits ## _128_a(seed_ch,(U8*)seed);        \
    WARN3("v0=%016lx v1=%016lx b=%d\n",                           \
            ((unsigned long *)seed_ch)[0],                     \
            ((unsigned long *)seed_ch)[1],                     \
            bits)

BEAGLE_STATIC_INLINE U64 beagle_hash_with_state_64_128_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 *seed= (U64 *)seed_ch;
    U64 v0= (U64)seed[0] ^ (k5_U64 * (key_len + 1));
    U64 v1= (U64)seed[1] ^ (k6_U64 * (key_len + 2));
    U64 v;
    U64 len = key_len;

    while ( len >= 16 ) {
        v = U8TO64_LE(key); key += 8;
        MIXV0(v,v0,v1);
        v = U8TO64_LE(key); key += 8;
        MIXV1(v,v0,v1);
        len -= 16;
    }

    if ( len >= 8 ) {
        v = U8TO64_LE(key); key += 8;
        MIXV2(v,v0,v1);
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
    MIXV3(v,v0,v1);

    WARN3("v0=%016lx v1=%016lx   %16s - ENTER FINALIZE\n",
            (unsigned long)v0, (unsigned long)v1, "");

    BEAGLE_FINALIZE(v0,v1);

    v = v0 ^ v1;

    WARN3("v0=%016lx v1=%016lx h=%016lx - FINAL\n",
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

    return u.u32.v0 ^ u.u32.v1;
}


BEAGLE_STATIC_INLINE void beagle_seed_state_64_128_a(
    const U8 *seed_ch,
    const U8 *seed_state_ch
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

    BEAGLE_MUNGE_SEED(seed_base,state,96,1);
}

BEAGLE_STATIC_INLINE void beagle_seed_state_112_128_a(
    const U8 *seed_ch,
    const U8 *seed_state_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *state= (U64 *)seed_state_ch;

    BEAGLE_MUNGE_SEED(seed_base,state,112,2);
}

BEAGLE_STATIC_INLINE void beagle_seed_state_127_128_a(
    const U8 *seed_ch,
    const U8 *seed_state_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *state= (U64 *)seed_state_ch;

    BEAGLE_MUNGE_SEED(seed_base,state,127,2);
}

/* this is the same as 127 */
BEAGLE_STATIC_INLINE void beagle_seed_state_128_128_a(
    const U8 *seed_ch,
    const U8 *seed_state_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *state= (U64 *)seed_state_ch;

    BEAGLE_MUNGE_SEED(seed_base,state,127,2);
}


#define BEAGLE_HASH(ret_type, out_bits, in_bits, fmt, fmt_type)    \
BEAGLE_STATIC_INLINE ret_type beagle_hash_ ## out_bits ## _ ## in_bits ## _a(   \
    const U8 *seed_ch,                                                      \
    const U8 *key,                                                          \
    const STRLEN key_len                                                    \
) {                                                                         \
    U64 seed[2];                                                            \
    ret_type final;                                                         \
    beagle_seed_state_ ## in_bits ## _128_a(seed_ch,(U8*)seed);             \
    WARN3("v0=%016lx v1=%016lx b=%d\n",                                   \
            ((unsigned long *)seed_ch)[0],                             \
            ((unsigned long *)seed_ch)[1],                             \
            in_bits);                                                       \
    final = beagle_hash_with_state_ ## out_bits ## _128_a((U8*)seed,key,key_len);     \
    NOTE3("seed-bits=%d key-len=%d final="fmt"\n\n",                       \
            in_bits, key_len, (fmt_type)final);                             \
    return final;                                                           \
}                                                                           \
                                                                            \
void beagle_hash_ ## out_bits ## _ ## in_bits ## _a_smhasher_test(          \
    const void *key,                                                        \
    STRLEN len,                                                             \
    U32 seed_base,                                                          \
    void *out                                                               \
) {                                                                         \
    U32 seed[4]= { seed_base, seed_base, seed_base, seed_base };            \
    *((ret_type *)out)= beagle_hash_ ## out_bits ## _ ## in_bits ## _a((U8*)seed, (U8 *)key, len); \
}

BEAGLE_HASH(U64,64,64,"%016lx",unsigned long)
BEAGLE_HASH(U64,64,96,"%016lx",unsigned long)
BEAGLE_HASH(U64,64,112,"%016lx",unsigned long)
BEAGLE_HASH(U64,64,127,"%016lx",unsigned long)
BEAGLE_HASH(U64,64,128,"%016lx",unsigned long)
BEAGLE_HASH(U64,32,64,"%08x",unsigned int)
BEAGLE_HASH(U64,32,96,"%08x",unsigned int)
BEAGLE_HASH(U64,32,112,"%08x",unsigned int)
BEAGLE_HASH(U64,32,127,"%08x",unsigned int)
BEAGLE_HASH(U64,32,128,"%08x",unsigned int)

#include <stdlib.h>
void beagle_hash_with_state_32_128_a_smhasher(const void *key, STRLEN len, const void *state, void *out) {
    *((U32 *)out)= beagle_hash_with_state_32_128_a((U8*)state, (U8 *)key, len);
}

void beagle_hash_with_state_64_128_a_smhasher(const void *key, STRLEN len, const void *state, void *out) {
    *((U64 *)out)= beagle_hash_with_state_64_128_a((U8*)state, (U8 *)key, len);
}

void beagle_seed_state_128_a_smhasher( const int in_bits, const void *seed_base, const void *state ) {
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

#ifdef __cplusplus
}
#endif

