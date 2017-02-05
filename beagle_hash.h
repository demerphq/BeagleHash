#ifndef BEAGLE_HASH_H
#define BEAGLE_HASH_H

#ifndef DEBUG_BEAGLE_HASH
#define DEBUG_BEAGLE_HASH 0
#endif

#if DEBUG_BEAGLE_HASH == 1
#define WARN3(pat,v0,v1,v2)         printf(pat, v0, v1, v2)
#define NOTE3(pat,v0,v1,v2)         printf(pat, v0, v1, v2)
#elif DEBUG_BEAGLE_HASH == 2
#define WARN3(pat,v0,v1,v2)
#define NOTE3(pat,v0,v1,v2)         printf(pat, v0, v1, v2)
#else
#define WARN3(pat,v0,v1,v2)
#define NOTE3(pat,v0,v1,v2)
#endif

#include <stdint.h>

#define _ROTL_SIZED(x,r,s) ( (x >> r) | (x << (s - r)) )
#define ROTL_32(x,r) _ROTL_SIZED(x,r,32)
#define ROTL_64(x,r) _ROTL_SIZED(x,r,64)


#if !defined(U64)
  #define U64 uint64_t
#endif

#if !defined(U32)
  #define U32 uint32_t
#endif

#if !defined(U8)
    #define U8 unsigned char
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

#ifdef __cplusplus
extern "C" {
#endif

void beagle_hash32_64_a_smhasher_test(const void *key, int len, U32 seed_base, void *out);
void beagle_hash32_96_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out);
void beagle_hash32_112_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out);
void beagle_hash32_127_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out);
void beagle_hash64_64_a_smhasher_test(const void *key, int len, U32 seed_base, void *out);
void beagle_hash64_96_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out);
void beagle_hash64_112_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out);
void beagle_hash64_127_a_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out);

#ifdef __cplusplus
}
#endif


#endif
