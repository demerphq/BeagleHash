#ifndef BEAGLE_HASH_H
#define BEAGLE_HASH_H

#ifndef DEBUG_BEAGLE_HASH
#define DEBUG_BEAGLE_HASH 0
#endif

#if DEBUG_BEAGLE_HASH == 1
#define WARN6(pat,v0,v1,v2,v3,v4,v5)    printf(pat, v0, v1, v2, v3, v4, v5)
#define WARN5(pat,v0,v1,v2,v3,v4)       printf(pat, v0, v1, v2, v3, v4)
#define WARN4(pat,v0,v1,v2,v3)          printf(pat, v0, v1, v2, v3)
#define WARN3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#define WARN2(pat,v0,v1)                printf(pat, v0, v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#elif DEBUG_BEAGLE_HASH == 2
#define WARN6(pat,v0,v1,v2,v3,v4,v5)
#define WARN5(pat,v0,v1,v2,v3,v4)
#define WARN4(pat,v0,v1,v2,v3)
#define WARN3(pat,v0,v1,v2)
#define WARN2(pat,v0,v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#else
#define WARN6(pat,v0,v1,v2,v3,v4,v5)
#define WARN5(pat,v0,v1,v2,v3,v4)
#define WARN4(pat,v0,v1,v2,v3)
#define WARN3(pat,v0,v1,v2)
#define NOTE3(pat,v0,v1,v2)
#define WARN2(pat,v0,v1)
#endif

#include <stdint.h>

#define _ROTL_SIZED(x,r,s) ( ((x) << (r)) | ((x) >> ((s) - (r))) )
#define _ROTR_SIZED(x,r,s) ( ((x) << ((s) - (r))) | ((x) >> (r)) )
#define ROTL_32(x,r) _ROTL_SIZED(x,r,32)
#define ROTL_64(x,r) _ROTL_SIZED(x,r,64)
#define ROTR_32(x,r) _ROTR_SIZED(x,r,32)
#define ROTR_64(x,r) _ROTR_SIZED(x,r,64)

#define _XORSHIFT(v, s1, s2, s3) STMT_START { \
        v ^= ( v << s1 );   \
        v ^= ( v >> s2 );   \
        v ^= ( v << s3 );   \
} STMT_END
#define XORSHIFT0(v) _XORSHIFT(v,13,17,5)
#define XORSHIFT1(v) _XORSHIFT(v,11,17,13)
#define XORSHIFT2(v) _XORSHIFT(v,2,15,25)
#define XORSHIFT3(v) _XORSHIFT(v,12,9,23)

#define XORSHIFT128plus(s0,s1) STMT_START {        \
    U64 x = s0;                                     \
    U64 const y = s1;                               \
    s0 = y;                                         \
    x ^= x << 23; /* a */                           \
    s1 = x ^ y ^ (x >> 17) ^ (y >> 26); /* b, c */  \
} STMT_END


#if !defined(U64)
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

#define BEAGLE_STATIC_INLINE static inline

#ifndef STMT_START
#define STMT_START do
#define STMT_END while(0)
#endif

#define U8TO64_LE(ptr)  (*((const U64 *)(ptr)))
#define U8TO32_LE(ptr)  (*((const U32 *)(ptr)))

#ifdef __cplusplus
extern "C" {
#endif

void beagle_hash_32_32_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_32_64_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_32_96_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_32_112_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_32_127_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_32_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_64_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_96_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_112_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);
void beagle_hash_64_127_a_smhasher_test (const void *key, STRLEN len, U32 seed, void *out);

void beagle_hash_with_state_32_128_a_smhasher ( const void *blob, const int len, const void *seed, void *out );
void beagle_hash_with_state_64_128_a_smhasher ( const void *blob, const int len, const void *seed, void *out );
void beagle_seed_state_128_a_smhasher ( const int in_bits, const void *seed, const void *state );

#ifdef __cplusplus
}
#endif

#endif
