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

#define k0_U32 0xE597A751UL
#define k1_U32 0xA9166027UL
#define k2_U32 0xBA14E5F7UL
#define k3_U32 0x8BF5E321UL
#define k4_U32 0xFEC63225UL
#define k5_U32 0xC3D00531UL
#define k6_U32 0xD9FBAD75UL
#define k7_U32 0x9BFB6FEBUL

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


#define MIXV0(v,v0,v1) MIXV0_7mul(v,v0,v1,k0_U64)
#define MIXV1(v,v0,v1) MIXV1_7mul(v,v0,v1,k1_U64)
#define MIXV2(v,v0,v1) MIXV0_8mul(v,v0,v1,k2_U64)
#define MIXV3(v,v0,v1) MIXV1_8mul(v,v0,v1,k3_U64)

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
    beagle_hash_seed_prep_ ## bits ## _128_a(seed_ch,(U8*)seed); \
    WARN3("v0=%016llx v1=%016llx b=%d\n",                         \
            ((unsigned long long *)seed_ch)[0],                 \
            ((unsigned long long *)seed_ch)[1],                 \
            bits)

BEAGLE_STATIC_INLINE U64 _beagle_hash_64_128_a(
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

    WARN3("v0=%016llx v1=%016llx h=%016llx - FINAL\n",
            (unsigned long long)v0, (unsigned long long)v1, (unsigned long long)v);

    return v;
}

BEAGLE_STATIC_INLINE U32 _beagle_hash_32_128_a(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U64 hash= _beagle_hash_64_128_a(seed_ch,key,key_len);
    U32 final= (hash >> 32) ^ hash;
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

/* this is the same as 127 */
BEAGLE_STATIC_INLINE void beagle_hash_seed_prep_128_128_a(
    const U8 *seed_ch,
    const U8 *seed_prep_ch
) {
    U64 *seed_base= (U64 *)seed_ch;
    U64 *seed_prep= (U64 *)seed_prep_ch;

    BEAGLE_MUNGE_SEED(seed_base,seed_prep,127,2);
}


#define BEAGLE_HASH(ret_type, out_bits, in_bits, fmt, fmt_type)    \
BEAGLE_STATIC_INLINE ret_type beagle_hash_ ## out_bits ## _ ## in_bits ## _a(   \
    const U8 *seed_ch,                                                      \
    const U8 *key,                                                          \
    const STRLEN key_len                                                    \
) {                                                                         \
    U64 seed[2];                                                            \
    ret_type final;                                                         \
    beagle_hash_seed_prep_ ## in_bits ## _128_a(seed_ch,(U8*)seed);         \
    WARN3("v0=%016llx v1=%016llx b=%d\n",                                   \
            ((unsigned long long *)seed_ch)[0],                             \
            ((unsigned long long *)seed_ch)[1],                             \
            in_bits);                                                       \
    final = _beagle_hash_ ## out_bits ## _128_a((U8*)seed,key,key_len);     \
    NOTE3("seed-bits=%d key-len=%d final=%"fmt"\n\n",                       \
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

BEAGLE_HASH(U64,64,32,"%016llx",unsigned long long)
BEAGLE_HASH(U64,64,64,"%016llx",unsigned long long)
BEAGLE_HASH(U64,64,96,"%016llx",unsigned long long)
BEAGLE_HASH(U64,64,112,"%016llx",unsigned long long)
BEAGLE_HASH(U64,64,127,"%016llx",unsigned long long)
BEAGLE_HASH(U64,64,128,"%016llx",unsigned long long)
BEAGLE_HASH(U64,32,32,"%08llx",unsigned long)
BEAGLE_HASH(U64,32,64,"%08llx",unsigned long)
BEAGLE_HASH(U64,32,96,"%08llx",unsigned long)
BEAGLE_HASH(U64,32,112,"%08llx",unsigned long)
BEAGLE_HASH(U64,32,127,"%08llx",unsigned long)
BEAGLE_HASH(U64,32,128,"%08llx",unsigned long)

void beagle_hash_32_128_a_smhasher( const void *blob, const int len, const void *seed, void *out )
{
    *((uint32_t *)out)= _beagle_hash_32_128_a((U8*)seed, (U8*)blob, len);
}

void beagle_hash_64_128_a_smhasher( const void *blob, const int len, const void *seed, void *out )
{
    *((uint32_t *)out)= _beagle_hash_64_128_a((U8*)seed, (U8*)blob, len);
}

void beagle_hash_seed_prep_smhasher( const int in_bits, const void *seed_base, const void *seed_prep ) {
    switch( in_bits ) {
        case  32:
            beagle_hash_seed_prep_32_128_a((U8*)seed_base,(U8*)seed_prep);
            break;
        case  64:
            beagle_hash_seed_prep_64_128_a((U8*)seed_base,(U8*)seed_prep);
            break;
        case  96:
            beagle_hash_seed_prep_96_128_a((U8*)seed_base,(U8*)seed_prep);
            break;
        case 127:
            beagle_hash_seed_prep_127_128_a((U8*)seed_base,(U8*)seed_prep);
            break;
        case 128:
            beagle_hash_seed_prep_128_128_a((U8*)seed_base,(U8*)seed_prep);
            break;
        default:
            printf("Don't know how to prepare %d bits in beagle_hash_seed_prep_smhasher()",
                    in_bits);
    }
}
#include <nmmintrin.h>
#define ZAPHOD_MIX(v0,v1,v2) STMT_START {                              \
    WARN3("v0=%08lx v1=%08lx v2=%08lx - ZAPHOD_MIX\n",                  \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2 );    \
    v1 = ROTL_32(v1,11) ^ v2;   \
    v0 = ROTR_32(v0,23) - v2;   \
    v2 = ROTL_32(v2,25) + v1;   \
    v0 = ROTR_32(v0,24) + v1;   \
    v1 = ROTL_32(v1,25) - v0;   \
    v2 = ROTL_32(v2,13) ^ v0;   \
} STMT_END

#define ZAPHOD_MIX_1(v0,v1,v2) STMT_START {                              \
    WARN3("v0=%08lx v1=%08lx v2=%08lx - ZAPHOD_MIX\n",                  \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2 );    \
    v1 = ROTR_32(v1,16) ^ v2;   \
    v0 = ROTL_32(v0,15) - v2;   \
    v2 = ROTR_32(v2, 1) + v1;   \
    v0 = ROTL_32(v0,24) + v1;   \
    v1 = ROTL_32(v1, 8) - v0;   \
    v2 = ROTL_32(v2,13) ^ v0;   \
} STMT_END

#define ZAPHOD_MIXy(v0,v1,v2) STMT_START {                              \
    WARN3("v0=%08lx v1=%08lx v2=%08lx - ZAPHOD_MIX\n",                  \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2 );    \
    v0 = ROTL_32(v0, 19) - v2;  \
    v1 = ROTL_32(v1, 10) ^ v2;  \
    v2 ^= v1;\
    v2 = ROTL_32(v2,  3) ^ v1;  \
    v0 = ROTL_32(v0,  6) ^ v1;  \
    v2 ^= v1;\
    v2 = ROTL_32(v2, 18) - v0;  \
    v1 = ROTL_32(v1, 18) - v0;  \
} STMT_END

#define ZAPHOD_MIX_f(v0,v1,v2) STMT_START {                              \
    WARN3("v0=%08lx v1=%08lx v2=%08lx - ZAPHOD_MIX\n",                  \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2 );    \
    v0 = ROTL_32(v0,17) - v2;   \
    v1 = ROTR_32(v1,23) ^ v2;   \
    v2 = ROTR_32(v2,11) ^ v1;   \
    v0 = ROTR_32(v0,29) ^ v1;   \
    v1 = ROTR_32(v1, 2) - v0;   \
    v2 = ROTR_32(v2, 7) - v0;   \
} STMT_END

#define ZAPHOD_MIXx(v0,v1,v2) STMT_START {                              \
    WARN3("v0=%08lx v1=%08lx v2=%08lx - ZAPHOD_MIX\n",                  \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2 );    \
    v0 = ROTL_32(v0, 19) - v2;  \
    v1 = ROTL_32(v1, 10) ^ v2;  \
    v2 = ROTL_32(v2,  3) ^ v1;  \
    v0 = ROTL_32(v0,  6) ^ v1;  \
    v1 = ROTL_32(v1, 18) - v0;  \
    v2 = ROTL_32(v2, 17) - v0;  \
} STMT_END

#define ZAPHOD_MIXp(v0,v1,v2) STMT_START {                              \
    WARN3("v0=%08lx v1=%08lx v2=%08lx - ZAPHOD_MIX\n",                  \
            (unsigned long)v0,(unsigned long)v1,(unsigned long)v2 );    \
    v2 += v1;                   \
    v0 = ROTL_32(v0,15) ^ v2;   \
    v1 = ROTR_32(v1,23) - v0;   \
    v0 = ROTR_32(v0,25) + v1;   \
    v2 ^= v0;                   \
    v1 = ROTR_32(v1,15) ^ v0;   \
    v2 = ROTL_32(v2,16);        \
    v0 -= v1;                   \
} STMT_END

#define ZAPHOD_FINALIZE(v0,v1,v2) STMT_START {          \
    WARN3("v0=%08lx v1=%08lx v2=%08lx - ZAPHOD FINALIZE\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2);  \
    v2 += v0;                       \
    v1 -= v2;                       \
    v1 = ROTL_32(v1,  6);           \
    v2 ^= v1;                       \
    v2 = ROTL_32(v2, 28);           \
    v1 ^= v2;                       \
    v0 += v1;                       \
    v1 = ROTL_32(v1, 24);           \
    v2 += v1;                       \
    v2 = ROTL_32(v2, 18) + v1;      \
    v0 ^= v2;                       \
    v0 = ROTL_32(v0, 20);           \
    v2 += v0;                       \
    v1 ^= v2;                       \
    v0 += v1;                       \
    v0 = ROTL_32(v0,  5);           \
    v2 += v0;                       \
    v2 = ROTL_32(v2, 22);           \
    v0 -= v1;                       \
    v1 -= v2;                       \
    v1 = ROTL_32(v1, 17);           \
} STMT_END


/* Passes Test: Testing took 837.813757 seconds
 * 1.491 bytes/cycle - 4264.57 MiB/sec @ 3 ghz
 * Small key speed test -    1-byte keys -    27.23 cycles/hash
 * Small key speed test -    4-byte keys -    24.64 cycles/hash
 * Small key speed test -   31-byte keys -    38.72 cycles/hash
 */
BEAGLE_STATIC_INLINE U32 zaphod_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U32 *seed= (U32 *)seed_ch;
    U32 v0= seed[0] ^ (0xBF2B3817UL * (key_len + 0));
    U32 v1= seed[1] ^ (0xA58CEF81UL * (key_len + 1));
    U32 v2= seed[2] ^ (0x8FA16711UL * (key_len - 1));
    U32 len= key_len;
    U32 hash;
    switch (key_len) {
        case 7: v0 += key[6] << 16;
        case 6: v2 -= key[5] << 8;
        case 5: v1 -= key[4] << 8;
        case 4: v0 -= key[3] << 8;
        case 3: v2 += key[2];
        case 2: v1 += key[1];
        case 1: v0 += key[0];
        case 0: break;
        case 15: /* fallthrough */
        case 14: /* fallthrough */
        case 13: /* fallthrough */
        case 12: goto read3;
        case 11: /* fallthrough */
        case 10: /* fallthrough */
        case 9: /* fallthrough */
        case 8: goto read2;
        default:{
            do {
                v1 -= U8TO32_LE(key); key += 4;
                v2 += U8TO32_LE(key); key += 4;
                ZAPHOD_MIX(v0,v1,v2);
                v1 -= U8TO32_LE(key); key += 4;
                v2 += U8TO32_LE(key); key += 4;
                ZAPHOD_MIX(v0,v1,v2);
                len -= 16;
            } while ( len >= 16 );
            switch ( len >> 2 ) {
                case 3:
                read3: v1 -= U8TO32_LE(key); key += 4;
                case 2:
                read2: v2 += U8TO32_LE(key); key += 4;
                ZAPHOD_MIX(v0,v1,v2);
                case 1:
                read1: v1 -= U8TO32_LE(key); key += 4;
                case 0:
                default: break;
            }
            v2 += ((key_len+1) << 24);
            switch (len & 0x3) {
                case 3: v2 -= (U32)key[2] << 16;
                case 2: v2 += (U32)key[1] <<  8;
                case 1: v2 -= (U32)key[0];
                        break;
                case 0:
                default:v2 ^= 0xFF;
                        break;
            }
            v0 ^= 0xFF;
            ZAPHOD_MIX(v0,v1,v2);
        }
    }
    ZAPHOD_FINALIZE(v0,v1,v2);
    hash = v0 ^ v1 ^ v2;

    WARN4("v0=%08lx v1=%08lx v2=%08lx hh=%08lx - FINAL\n\n",
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2,
            (unsigned long)hash);
    return hash;
}

void zaphod_hash_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[3]= { 0, 0, seed_base };

    *((U32 *)out)= zaphod_hash((U8*)seed, (U8 *)key, len);
}

#define PHAT_MIX_20a(v0,v1,v2,v3) STMT_START {      \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx %11s - PHAT_MIX\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2, (unsigned long)v3, "");  \
    v1 += v0;                   \
    v0 = ROTL_32(v0, 5) ^ v1;   \
    v2 = ROTL_32(v2,27) + v3;   \
    v1 = ROTL_32(v1,30) ^ v2;   \
    v0 -= v2;                   \
    v1 = ROTL_32(v1, 8) + v0;   \
    v2 -= v1;                   \
    v3 = ROTL_32(v3,22) - v2;   \
    v1 = ROTL_32(v1,12);        \
    v0 ^= v1;                   \
    v1 -= v2;                   \
    v0 += v3;                   \
    v1 = ROTL_32(v1,14);        \
    v2 ^= v1;                   \
    v3 ^= v2;   /* passes: 1.177 bytes/cycle - 3366.00 MiB/sec @ 3 ghz */ \
    /* fails: v3 *= k2_U32; - 1.189 bytes/cycle - 3402.56 MiB/sec @ 3 ghz */ \
} STMT_END

#define PHAT_MIX_20b(v0,v1,v2,v3) STMT_START {      \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx %11s - PHAT_MIX\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2, (unsigned long)v3, "");  \
    v1 += v0;                   \
    v0 = ROTL_32(v0, 5) ^ v1;   \
    v2 = ROTL_32(v2,27) + v3;   \
    v1 = ROTL_32(v1,30) - v2;   \
    v3 = ROTL_32(v3,15) - v0;   \
    v2 -= v1;                   \
    v0 = ROTL_32(v0, 5) ^ v1;   \
    v2 = ROTL_32(v2,17) + v3;   \
    v1 = ROTL_32(v1,30) ^ v2;   \
    v3 = ROTL_32(v3,15) - v0;   \
    v0 -= v2;                   \
    v1 -= v3;                   \
} STMT_END

#define PHAT_MIX_20c(v0,v1,v2,v3) STMT_START {      \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx %11s - PHAT_MIX\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2, (unsigned long)v3, "");  \
    v0 = ROTL_32(v0,27) - v1;   \
    v2 = ROTL_32(v2,13) - v3;   \
    v1 = ROTL_32(v1, 5) ^ v2;   \
    v3 = ROTL_32(v3,23) - v0;   \
    v1 *= k4_U32;               \
    v0 *= k0_U32;               \
    v0 ^= v1;                   \
    v1 = ROTL_32(v1, 8) + v0;   \
    v3 = ROTL_32(v3,15) + v2;   \
    v0 = ROTL_32(v0,10) - v3;   \
    v2 = ROTL_32(v2,17) - v1;   \
    v0 ^= v1;                   \
    v3 ^= v1;                   \
} STMT_END

#define PHAT_MIX_20d(v0,v1,v2,v3) STMT_START {      \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx %11s - PHAT_MIX\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2, (unsigned long)v3, "");  \
    v1 -= v0;                   \
    v0 = ROTL_32(v0,27) ^ v1;   \
    v2 = ROTL_32(v2, 3) - v3;   \
    v1 = ROTL_32(v1,14) ^ v2;   \
    v3 = ROTL_32(v3,25) - v0;   \
    v1 *= k4_U32;               \
    v0 += v1;                   \
    v1 = ROTL_32(v1,19) ^ v0;   \
    v3 = ROTL_32(v3,12) ^ v2;   \
    v0 = ROTL_32(v0, 7) ^ v3;   \
    v2 = ROTL_32(v2, 7) - v1;   \
    v3 += v2;                   \
} STMT_END

#define PHAT_MIX_21a(v0,v1,v2,v3) STMT_START { \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx %11s - PHAT_MIX\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2, (unsigned long)v3, "");  \
    v0 -= v2;                   \
    v0 = ROTL_32(v0, 7) ^ v1;   \
    v2 = ROTL_32(v2,21) - v3;   \
    v1 = ROTL_32(v1, 7) + v2;   \
    v3 = ROTL_32(v3,12) + v0;   \
    v3 *= k3_U32;               \
    v2 -= v3;                   \
    v1 = ROTL_32(v1,18) ^ v0;   \
    v3 = ROTL_32(v3,18) - v2;   \
    v0 = ROTL_32(v0, 4) ^ v3;   \
    v2 = ROTL_32(v2, 8) + v1;   \
    v1 ^= v0;                   \
    v2 *= k4_U32;               \
} STMT_END

#define PHAT_MIX_21b(v0,v1,v2,v3) STMT_START { \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx %11s - PHAT_MIX\n", \
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2, (unsigned long)v3, "");  \
    v2 -= v0;                   \
    v0 = ROTL_32(v0, 7) + v1;   \
    v2 = ROTL_32(v2,25) ^ v3;   \
    v1 = ROTL_32(v1,10) ^ v2;   \
    v3 = ROTL_32(v3, 6) - v0;   \
    v1 *= k6_U32;               \
    v0 ^= v1;                   \
    v1 = ROTL_32(v1, 8) - v0;   \
    v3 = ROTL_32(v3,30) ^ v2;   \
    v0 = ROTL_32(v0, 5) - v3;   \
    v2 = ROTL_32(v2,19) + v1;   \
    v0 *= k4_U32;               \
    v3 -= v2;                   \
} STMT_END

#define PHAT_MIX PHAT_MIX_21b
#define PHAT_MIXV(v0,v1,v2,v3,vx,vy,vz) STMT_START { \
    WARN3("vx=%08lx vy=%08lx vz=%08lx - PHAT INPUT\n",vx,vy,vz);\
    v0 ^= vx;   \
    v1 += vy;   \
    v2 -= vz;   \
    PHAT_MIX(v0,v1,v2,v3); \
} STMT_END

#define PHAT_FINALIZE(v0,v1,v2,v3) STMT_START { \
    PHAT_MIX(v0,v1,v2,v3); \
    PHAT_MIX(v0,v1,v2,v3); \
} STMT_END

BEAGLE_STATIC_INLINE U32 phat_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U32 *seed= (U32 *)seed_ch;
    U32 v0= seed[0] + key_len;
    U32 v1= seed[1] ^ k0_U32;
    U32 v2= seed[0] ^ k1_U32;
    U32 v3= seed[1] - key_len;
    U32 vx;
    U32 vy;
    U32 vz;
    U32 len= key_len;
    U32 final;

    while ( len >= 12 ) {
        vx = U8TO32_LE(key); key += 4;
        vy = U8TO32_LE(key); key += 4;
        vz = U8TO32_LE(key); key += 4;
        PHAT_MIXV(v0,v1,v2,v3,vx,vy,vz);
        len -= 12;
    }
    vx= vy= 0;
    switch (len >> 2) {
        case 2: vx= U8TO32_LE(key); key += 4;
        case 1: vy= U8TO32_LE(key); key += 4;
    }
    vz = (~len << 24);
    switch (len & 3) {
        case 3: vz ^= (U32)key[2] << 16;
        case 2: vz ^= (U32)key[1] <<  8;
        case 1: vz ^= (U32)key[0];
        case 0:
        default: break;
    }

    v3 ^= 0xFF;
    PHAT_MIXV(v0,v1,v2,v3,vx,vy,vz);
    PHAT_FINALIZE(v0,v1,v2,v3);

    vx = (v0 ^ v1) ^ (v2 ^ v3);

    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx hh=%08lx - FINAL\n\n",
            (unsigned long)v0, (unsigned long)v1, (unsigned long)v2, (unsigned long)v3,
            (unsigned long)vx);

    return vx;
}

void phat_hash_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[2]= { seed_base ^ 0xE79C8683UL, seed_base ^ 0xAA8C6432UL };
    *((U32 *)out)= phat_hash((U8*)seed, (U8 *)key, len);
}
/*
#define PHAT4_MIXER 5
Alignment  0 -  1.446 bytes/cycle - 4137.59 MiB/sec @ 3 ghz
Alignment  1 -  1.446 bytes/cycle - 4138.36 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    27.45 cycles/hash
Small key speed test -    1-byte keys -    29.08 cycles/hash
Small key speed test -    4-byte keys -    26.96 cycles/hash
Small key speed test -   64-byte keys -    65.11 cycles/hash

#define PHAT4_MIXER 6
Alignment  0 -  1.482 bytes/cycle - 4239.62 MiB/sec @ 3 ghz
Alignment  1 -  1.482 bytes/cycle - 4240.03 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    27.02 cycles/hash
Small key speed test -    1-byte keys -    29.26 cycles/hash
Small key speed test -    4-byte keys -    26.91 cycles/hash
Small key speed test -   64-byte keys -    68.27 cycles/hash

#define PHAT4_MIXER 7
Alignment  0 -  1.458 bytes/cycle - 4170.82 MiB/sec @ 3 ghz
Alignment  1 -  1.423 bytes/cycle - 4070.41 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    26.71 cycles/hash
Small key speed test -    1-byte keys -    28.62 cycles/hash
Small key speed test -    4-byte keys -    27.07 cycles/hash
Small key speed test -   64-byte keys -    67.74 cycles/hash

#define PHAT4_MIXER 8
Alignment  0 -  1.568 bytes/cycle - 4486.96 MiB/sec @ 3 ghz
Alignment  1 -  1.568 bytes/cycle - 4486.50 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    28.45 cycles/hash
Small key speed test -    1-byte keys -    28.79 cycles/hash
Small key speed test -    4-byte keys -    26.87 cycles/hash
Small key speed test -   64-byte keys -    66.73 cycles/hash

#define PHAT4_MIXER 9
Alignment  0 -  1.568 bytes/cycle - 4486.68 MiB/sec @ 3 ghz
Alignment  1 -  1.568 bytes/cycle - 4487.18 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    28.76 cycles/hash
Small key speed test -    1-byte keys -    28.87 cycles/hash
Small key speed test -    4-byte keys -    26.67 cycles/hash
Small key speed test -   64-byte keys -    67.32 cycles/hash

#define PHAT4_MIXER 10
Alignment  0 -  1.568 bytes/cycle - 4486.89 MiB/sec @ 3 ghz
Alignment  1 -  1.568 bytes/cycle - 4486.67 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    26.39 cycles/hash
Small key speed test -    1-byte keys -    29.87 cycles/hash
Small key speed test -    4-byte keys -    26.72 cycles/hash
Small key speed test -   64-byte keys -    66.95 cycles/hash

#define PHAT4_MIXER 11
Alignment  0 -  1.568 bytes/cycle - 4487.22 MiB/sec @ 3 ghz
Alignment  1 -  1.569 bytes/cycle - 4487.58 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    27.25 cycles/hash
Small key speed test -    1-byte keys -    29.74 cycles/hash
Small key speed test -    4-byte keys -    26.99 cycles/hash
Small key speed test -   64-byte keys -    66.74 cycles/hash

*/
#define PHAT4_MIXER 6

#if PHAT4_MIXER == 0
#error Bad PHAT4 MIXER
#elif PHAT4_MIXER == 12
#define PHAT4_k0 k7_U32
#define PHAT4_k1 k4_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START {                        \
    WARN6("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(%d) %s\n",       \
            (unsigned long)v0, (unsigned long)v1,                       \
            (unsigned long)v2, (unsigned long)v3, PHAT4_MIXER, text);   \
    v2 ^= v0;                           \
    v1 = ROTL_32(v1,11) + v2;           \
    v3 ^= v1;                           \
    v2 = ROTL_32(v2, 5) - v3;           \
    v3 = ROTR_32(v3, 8) - v2;           \
    v0 ^= v3;                           \
    v3 = ROTL_32(v3,14) - v0;           \
    v3 = ROTR_32(v3,27);                \
    v0 -= v3;                           \
    v1 ^= v0;                           \
    v2 += v1;                           \
} STMT_END
#elif PHAT4_MIXER == 11
#define PHAT4_k0 k7_U32
#define PHAT4_k1 k4_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START { \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(11) %s\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3, text);   \
    v0 = ROTL_32(v0,13) + v3;   \
    v1 ^= v2;                   \
    v3 = ROTL_32(v3, 9) + v1;   \
    v2 ^= v0;                   \
    v0 = ROTL_32(v0,14) ^ v3;   \
    v1 = ROTL_32(v1,25) - v2;   \
    v3 ^= v1;                   \
    v2 = ROTL_32(v2, 4) - v0;   \
} STMT_END
#elif PHAT4_MIXER == 10
#define PHAT4_k0 k5_U32
#define PHAT4_k1 k2_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START { \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(10) %s\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3, text);   \
    v0 = ROTL_32(v0,13) - v3;   \
    v1 ^= v2;                   \
    v3 = ROTL_32(v3, 9) + v1;   \
    v2 ^= v0;                   \
    v0 = ROTL_32(v0,14) ^ v3;   \
    v1 = ROTL_32(v1,25) - v2;   \
    v3 ^= v1;                   \
    v2 = ROTL_32(v2, 4) - v0;   \
} STMT_END
#elif PHAT4_MIXER == 9
#define PHAT4_k0 k2_U32
#define PHAT4_k1 k7_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START { \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(9) %s\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3, text);   \
    v0 = ROTL_32(v0,13) + v3;   \
    v1 ^= v2;                   \
    v3 = ROTL_32(v3,23) + v1;   \
    v2 ^= v0;                   \
    v0 = ROTL_32(v0, 6) ^ v3;   \
    v1 = ROTL_32(v1,25) + v2;   \
    v3 ^= v1;                   \
    v2 = ROTL_32(v2, 9) - v0;   \
} STMT_END
#elif PHAT4_MIXER == 8
#define PHAT4_k0 k3_U32
#define PHAT4_k1 k6_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START { \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(8) %s\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3, text);   \
    v0 = ROTL_32(v0,13) + v3;   \
    v1 ^= v2;                   \
    v3 = ROTL_32(v3,10) + v1;   \
    v2 ^= v0;                   \
    v0 = ROTL_32(v0,14) ^ v3;   \
    v1 = ROTL_32(v1,25) + v2;   \
    v3 ^= v1;                   \
    v2 = ROTL_32(v2,17) - v0;   \
} STMT_END
#elif PHAT4_MIXER == 7
#define PHAT4_k0 k0_U32
#define PHAT4_k1 k7_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START { \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(7) %s\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3, text);   \
    v0 -= v3;                   \
    v1 = ROTL_32(v1, 7) ^ v2;   \
    v3 = ROTL_32(v3, 9) + v1;   \
    v2 = ROTL_32(v2,18) ^ v0;   \
    v0 = ROTL_32(v0,27) ^ v3;   \
    v1 = ROTL_32(v1, 5) + v2;   \
    v3 = ROTL_32(v3, 8) ^ v1;   \
    v2 += v0;   \
} STMT_END
#elif PHAT4_MIXER == 6
#define PHAT4_k0 k0_U32
#define PHAT4_k1 k7_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START {  \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(6) %s\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3, text);   \
    v0 = ROTL_32(v0,23) ^ v3;   \
    v1 = ROTL_32(v1, 5) + v2;   \
    v3 += v1;                   \
    v2 = ROTL_32(v2, 3) + v0;   \
    v0 = ROTL_32(v0,12) + v3;   \
    v1 ^= v2;                   \
    v3 = ROTL_32(v3,19) - v1;   \
    v2 = ROTL_32(v2,22) ^ v0;   \
} STMT_END
#elif PHAT4_MIXER == 5
#define PHAT4_k0 k0_U32
#define PHAT4_k1 k7_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START {  \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(5) %s\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3, text);   \
    v0 = ROTL_32(v0,20) - v3;   \
    v1 = ROTL_32(v1, 3) + v2;   \
    v3 = ROTL_32(v3,27) ^ v1;   \
    v2 ^= v0;                   \
    v0 -= v3;                   \
    v1 = ROTL_32(v1,14) + v2;   \
    v3 ^= v1;                   \
    v2 = ROTL_32(v2,24) ^ v0;   \
} STMT_END
#elif PHAT4_MIXER == 0
#define PHAT4_k0 k0_U32
#define PHAT4_k1 k7_U32
#define PHAT4_MIX(v0,v1,v2,v3,text) STMT_START {  \
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4(0) %s\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3, text);   \
} STMT_END
#else
#error "No PHAT4_MIXER defined"
#endif

#define PHAT4_FINALIZER 2

#if PHAT4_FINALIZER == 1
#define PHAT4_FINALIZE(v0,v1,v2,v3) STMT_START {   \
    WARN4("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4_FINALIZE(1)\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3);   \
    v0 ^= v3;                   \
    v0 += v2;                   \
    v1 += v0;                   \
    v3 = ROTL_32(v3,19) + v1;   \
    v2 += v3;                   \
    v1 = ROTL_32(v1,25) ^ v3;   \
    v3 = ROTL_32(v3, 5) - v1;   \
    v2 = ROTL_32(v2, 3) ^ v3;   \
    v3 = ROTL_32(v3,17) - v2;   \
    v2 = ROTL_32(v2,14) - v3;   \
    v0 ^= v2;                   \
    v1 ^= v0;                   \
    v2 = ROTL_32(v2,13) - v0;   \
    v0 = ROTL_32(v0,22) ^ v2;   \
    v1 = ROTL_32(v1,17) - v0;   \
    v3 ^= v1;                   \
    v0 = ROTL_32(v0, 6);        \
    v1 = ROTL_32(v1,20);        \
    v2 -= v0;                   \
} STMT_END
#elif PHAT4_FINALIZER == 2
#define PHAT4_FINALIZE(v0,v1,v2,v3) STMT_START {   \
    WARN4("v0=%08lx v1=%08lx v2=%08lx v3=%08lx - PHAT4_FINALIZE(2)\n", \
            (unsigned long)v0, (unsigned long)v1,    \
            (unsigned long)v2, (unsigned long)v3);   \
    v1 += v3;                   \
    v0 ^= v2;                   \
    v1 = ROTL_32(v1, 6) - v0;   \
    v3 -= v1;                   \
    v1 = ROTL_32(v1,10) ^ v3;   \
    v3 = ROTL_32(v3, 5) + v1;   \
    v2 = ROTL_32(v2,13) ^ v3;   \
    v3 = ROTL_32(v3,17) + v2;   \
    v2 = ROTL_32(v2,15) ^ v3;   \
    v0 += v2;                   \
    v1 ^= v0;                   \
    v2 = ROTL_32(v2, 9) - v1;   \
    v0 = ROTL_32(v0,22) + v2;   \
    v1 = ROTL_32(v1,18) - v0;   \
    v3 = ROTL_32(v3,22) + v1;   \
    v0 = ROTL_32(v0, 4);        \
    v1 = ROTL_32(v1,11) - v0;   \
} STMT_END
#else
#define PHAT4_FINALIZE(v0,v1,v2,v3) STMT_START {   \
    PHAT4_MIX(v0,v1,v2,v3,"FINALIZE MIX #1");\
    PHAT4_MIX(v1,v2,v3,v0,"FINALIZE MIX #2");\
    PHAT4_MIX(v0,v1,v2,v3,"FINALIZE MIX #3");\
} STMT_END
#endif
#define DO_PHAT4_MIX(v0,v1,v2,v3,text) STMT_START { \
    PHAT4_MIX(v0,v1,v2,v3,"" text);\
} STMT_END

BEAGLE_STATIC_INLINE U32 phat4_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U32 *seed= (U32 *)seed_ch;
    U32 v0= seed[0];
    U32 v1= seed[1] + key_len;
    U32 v2= (seed[0] ^ PHAT4_k0) - key_len;
    U32 v3= seed[1] ^ PHAT4_k1;
    U32 final;
    U32 len= key_len;

    while ( len >= 16 ) {
        v3 += U8TO32_LE(key); key+=4;
        v2 -= U8TO32_LE(key); key+=4;
        DO_PHAT4_MIX(v0,v1,v2,v3,"MIX v3/v2");
        v1 += U8TO32_LE(key); key+=4;
        v0 -= U8TO32_LE(key); key+=4;
        DO_PHAT4_MIX(v0,v1,v2,v3,"MIX v1/v0");
        len -= 16;
    }
    switch (len >> 2) {
        case 3: v3 += U8TO32_LE(key); key+=4;
        case 2: v2 -= U8TO32_LE(key); key+=4;
        DO_PHAT4_MIX(v0,v1,v2,v3,"MIX TAIL v3/v2");
        case 1: v1 += U8TO32_LE(key); key+=4;
        case 0:
        default: break;
    }
    v0 -= (len+1) << 24;
    switch (len & 3) {
        case 3: v0 -= (U32)key[2] << 16;
        case 2: v0 -= (U32)key[1] <<  8;
        case 1: v0 -= (U32)key[0];
        case 0:
        default: break;
    }
    DO_PHAT4_MIX(v0,v1,v2,v3,"MIX TAIL v1/v0");
    v3 ^= 0xFF;
    PHAT4_FINALIZE(v0,v1,v2,v3);

    final = (v1 ^ v2) ^ (v0 ^ v3);
    WARN5("v0=%08lx v1=%08lx v2=%08lx v3=%08lx hh=%08lx - PHAT4 RESULT\n\n",
            (unsigned long)v0, (unsigned long)v1,
            (unsigned long)v2, (unsigned long)v3,
            (unsigned long)final);

    return final;
}

void phat4_hash_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[2]= { seed_base, seed_base };
    *((U32 *)out)= phat4_hash((U8*)seed, (U8 *)key, len);
}


#ifdef __cplusplus
}
#endif

