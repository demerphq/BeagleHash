#ifdef __cplusplus
extern "C" {
#endif

#include "beagle_hash.h"
#include "stdio.h"

#define k0 0xc3a5c85c97cb3127ULL
#define k1 0xb492b66fbe98f273ULL
#define k2 0x9ae16a3b2f90404fULL
#define k3 0xc949d7c7509e6557ULL
#define k6 0xA281DA4F1CFD7755ULL
#define k9 0x3CA061695A518A65ULL

#define MIXV0_7mul(v,v0,v1,k) STMT_START {     \
    WARN3("v0=%016llx v1=%016llx v=%016llx - MIXV0_7mul\n", \
            (unsigned long long)v0, (unsigned long long)v1, (unsigned long long)v); \
    v0 -= (v * k);              \
    v1 ^= v0;                   \
    v0 = ROTL_64(v0, 26) + v1;  \
    v1 = ROTL_64(v1, 42) ^ v0;  \
    v0 -= v1;                   \
} STMT_END

#define MIXV1_7mul(v,v0,v1,k) STMT_START {     \
    WARN3("v0=%016llx v1=%016llx v=%016llx - MIXV1_7mul\n", \
            (unsigned long long)v0, (unsigned long long)v1, (unsigned long long)v); \
    v1 += (v * k);              \
    v0 ^= v1;                   \
    v1 = ROTL_64(v1, 18) - v0;  \
    v0 = ROTL_64(v0, 39) ^ v1;  \
    v1 -= v0;                   \
} STMT_END

#define MIXV0_8mul(v,v0,v1,k) STMT_START {     \
    WARN3("v0=%016llx v1=%016llx v=%016llx - MIXV0_8mul\n", \
            (unsigned long long)v0, (unsigned long long)v1, (unsigned long long)v); \
    v1 += (v * k);                     \
    v0 += v1;                           \
    v1 = ROTL_64(v1, 32) ^ v0;          \
    v0 += v1;                           \
    v0 = ROTL_64(v0, 15) ^ v1;          \
    v1 -= v0;                           \
} STMT_END

#define MIXV1_8mul(v,v0,v1,k) STMT_START {     \
    WARN3("v0=%016llx v1=%016llx v=%016llx - MIXV1_8mul\n", \
            (unsigned long long)v0, (unsigned long long)v1, (unsigned long long)v); \
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


#define MIXV0(v,v0,v1) MIXV0_7mul(v,v0,v1,k0)
#define MIXV1(v,v0,v1) MIXV1_7mul(v,v0,v1,k1)
#define MIXV2(v,v0,v1) MIXV0_8mul(v,v0,v1,k2)
#define MIXV3(v,v0,v1) MIXV1_8mul(v,v0,v1,k3)

#define BEAGLE_MUNGE_SEED(seed_base,seed,bits,rounds) STMT_START {  \
    int irounds = rounds;                                           \
    seed[0] = seed_base[0];                                         \
    seed[1] = (seed_base[1] & ~((1ULL << (128 - bits)) - 1ULL))     \
            | ((~seed_base[0]) >> (64 - (128 - bits)));             \
    while ( irounds-- > 0 )                                         \
        BEAGLE_FINALIZE(seed[0],seed[1]);                           \
} STMT_END

#define BEAGLE_SEED_PREP_WITH_DECL(bits,seed_ch,seed)       \
    U64 seed[2];                                            \
    beagle_hash_seed_prep_ ## bits ## _128_a(seed_ch,(U8*)seed)

BEAGLE_STATIC_INLINE U64 beagle_hash_64_128_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 *seed= (U64 *)seed_ch;
    U64 v0= (U64)seed[0];
    U64 v1= (U64)seed[1];
    U64 v;
    STRLEN tail = key_len & 0xF;

    if ( key_len >= 16 ) {
        const U8 *end= key + key_len - tail;
        do {
            v = U8TO64_LE(key); key += 8;
            MIXV0(v,v0,v1);
            v = U8TO64_LE(key); key += 8;
            MIXV1(v,v0,v1);
        } while (key < end);
    }

    if ( tail >= 8 ) {
        v = U8TO64_LE(key); key += 8;
        MIXV2(v,v0,v1);
    }

    v= ((U64)~key_len) << 56;
    switch (tail & 0x7) {
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

    WARN3("v0=%016llx v1=%016llx   %16s - ENTER FINALIZE\n",
            (unsigned long long)v0, (unsigned long long)v1, "");

    BEAGLE_FINALIZE(v0,v1);

    v = v0 ^ v1;

    WARN3("v0=%016llx v1=%016llx h=%016llx - FINAL\n\n",
            (unsigned long long)v0, (unsigned long long)v1, (unsigned long long)v);

    return v;
}

BEAGLE_STATIC_INLINE U32 beagle_hash_32_128_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 hash= beagle_hash_64_128_a(seed_ch,key,key_len);
    U32 final=(U32)hash;

    NOTE3("h=%016llx l=%d f=%08x\n",
            (unsigned long long)hash, key_len, final);

    return final;
}

BEAGLE_STATIC_INLINE void beagle_hash_seed_prep_32_128_a(
    const U8 *seed_ch,
    const U8 *seed_prep_ch
) {
    U32 *seed_base= (U32 *)seed_ch;
    U32 *seed_prep= (U32 *)seed_prep_ch;

    seed_prep[0] = seed_base[0] ^ 0x9555DC8BUL;
    seed_prep[1] = seed_base[0] ^ 0xF98B2A27UL;
    seed_prep[2] = seed_base[0] ^ 0xD0DC8056UL;
    seed_prep[3] = seed_base[0];
}

BEAGLE_STATIC_INLINE void beagle_hash_seed_prep_64_128_a(
    const U8 *seed_ch,
    const U8 *seed_prep_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *seed_prep= (U64 *)seed_prep_ch;

    seed_prep[0] = seed_base[0] ^ 0x736f6d6570736575ull;
    seed_prep[1] = seed_base[0];
}


BEAGLE_STATIC_INLINE void beagle_hash_seed_prep_96_128_a(
    const U8 *seed_ch,
    const U8 *seed_prep_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *seed_prep= (U64 *)seed_prep_ch;

    BEAGLE_MUNGE_SEED(seed_base,seed_prep,96,1);
}

BEAGLE_STATIC_INLINE void beagle_hash_seed_prep_112_128_a(
    const U8 *seed_ch,
    const U8 *seed_prep_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *seed_prep= (U64 *)seed_prep_ch;

    BEAGLE_MUNGE_SEED(seed_base,seed_prep,112,1);
}

BEAGLE_STATIC_INLINE void beagle_hash_seed_prep_127_128_a(
    const U8 *seed_ch,
    const U8 *seed_prep_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *seed_prep= (U64 *)seed_prep_ch;

    BEAGLE_MUNGE_SEED(seed_base,seed_prep,127,2);
}


BEAGLE_STATIC_INLINE U64 beagle_hash_64_64_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    BEAGLE_SEED_PREP_WITH_DECL(64,seed_ch,seed);
    return beagle_hash_64_128_a((U8*)seed,key,key_len);
}


BEAGLE_STATIC_INLINE U64 beagle_hash_64_96_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    BEAGLE_SEED_PREP_WITH_DECL(96,seed_ch,seed);
    return beagle_hash_64_128_a((U8*)seed,key,key_len);
}

BEAGLE_STATIC_INLINE U64 beagle_hash_64_112_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    BEAGLE_SEED_PREP_WITH_DECL(112,seed_ch,seed);
    return beagle_hash_64_128_a((U8*)seed,key,key_len);
}

BEAGLE_STATIC_INLINE U64 beagle_hash_64_127_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    BEAGLE_SEED_PREP_WITH_DECL(127,seed_ch,seed);
    return beagle_hash_64_128_a((U8*)seed,key,key_len);
}

BEAGLE_STATIC_INLINE U32 beagle_hash_32_64_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    BEAGLE_SEED_PREP_WITH_DECL(64,seed_ch,seed);
    return beagle_hash_32_128_a((U8*)seed,key,key_len);
}


BEAGLE_STATIC_INLINE U32 beagle_hash_32_96_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    BEAGLE_SEED_PREP_WITH_DECL(96,seed_ch,seed);
    return beagle_hash_32_128_a((U8*)seed,key,key_len);
}

BEAGLE_STATIC_INLINE U32 beagle_hash_32_112_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    BEAGLE_SEED_PREP_WITH_DECL(112,seed_ch,seed);
    return beagle_hash_32_128_a((U8*)seed,key,key_len);
}

BEAGLE_STATIC_INLINE U32 beagle_hash_32_127_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    BEAGLE_SEED_PREP_WITH_DECL(127,seed_ch,seed);
    return beagle_hash_32_128_a((U8*)seed,key,key_len);
}

void beagle_hash32_64_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[2]= { seed_base, seed_base };
    *((U32 *)out)= beagle_hash_32_64_a((U8*)seed, (U8 *)key, len);
}

void beagle_hash32_96_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[4]= { seed_base, seed_base, seed_base, seed_base };
    *((U32 *)out)= beagle_hash_32_96_a((U8*)seed, (U8 *)key, len);
}

void beagle_hash32_112_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[4]= { seed_base, seed_base, seed_base, seed_base };
    *((U32 *)out)= beagle_hash_32_112_a((U8*)seed, (U8 *)key, len);
}

void beagle_hash32_127_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[4]= { seed_base, seed_base, seed_base, seed_base };
    *((U32 *)out)= beagle_hash_32_127_a((U8*)seed, (U8 *)key, len);
}

void beagle_hash64_64_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[2]= { seed_base, seed_base };
    *((U64 *)out)= beagle_hash_64_64_a((U8*)seed, (U8 *)key, len);
}

void beagle_hash64_96_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[4]= { seed_base, seed_base, seed_base, seed_base };
    *((U64 *)out)= beagle_hash_64_112_a((U8*)seed, (U8 *)key, len);
}

void beagle_hash64_112_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[4]= { seed_base, seed_base, seed_base, seed_base };
    *((U64 *)out)= beagle_hash_64_112_a((U8*)seed, (U8 *)key, len);
}

void beagle_hash64_127_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[4]= { seed_base, seed_base, seed_base, seed_base };
    *((U64 *)out)= beagle_hash_64_127_a((U8*)seed, (U8 *)key, len);
}


#ifdef __cplusplus
}
#endif

