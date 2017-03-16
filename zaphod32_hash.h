#ifndef DEBUG_ZAPHOD32_HASH
#define DEBUG_ZAPHOD32_HASH 0
#endif

#if DEBUG_ZAPHOD32_HASH == 1
#include <stdio.h>
#define ZAPHOD32_WARN6(pat,v0,v1,v2,v3,v4,v5)    printf(pat, v0, v1, v2, v3, v4, v5)
#define ZAPHOD32_WARN5(pat,v0,v1,v2,v3,v4)       printf(pat, v0, v1, v2, v3, v4)
#define ZAPHOD32_WARN4(pat,v0,v1,v2,v3)          printf(pat, v0, v1, v2, v3)
#define ZAPHOD32_WARN3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#define ZAPHOD32_WARN2(pat,v0,v1)                printf(pat, v0, v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#elif DEBUG_ZAPHOD32_HASH == 2
#define ZAPHOD32_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define ZAPHOD32_WARN5(pat,v0,v1,v2,v3,v4)
#define ZAPHOD32_WARN4(pat,v0,v1,v2,v3)
#define ZAPHOD32_WARN3(pat,v0,v1,v2)
#define ZAPHOD32_WARN2(pat,v0,v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#else
#define ZAPHOD32_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define ZAPHOD32_WARN5(pat,v0,v1,v2,v3,v4)
#define ZAPHOD32_WARN4(pat,v0,v1,v2,v3)
#define ZAPHOD32_WARN3(pat,v0,v1,v2)
#define NOTE3(pat,v0,v1,v2)
#define ZAPHOD32_WARN2(pat,v0,v1)
#endif

#ifndef ROTL32
#define _ROTL_SIZED(x,r,s) ( ((x) << (r)) | ((x) >> ((s) - (r))) )
#define _ROTR_SIZED(x,r,s) ( ((x) << ((s) - (r))) | ((x) >> (r)) )
#define ROTL32(x,r) _ROTL_SIZED(x,r,32)
#define ROTR32(x,r) _ROTR_SIZED(x,r,32)
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

#ifndef ZAPHOD32_STATIC_INLINE
#ifdef PERL_STATIC_INLINE
#define ZAPHOD32_STATIC_INLINE PERL_STATIC_INLINE
#else
#define ZAPHOD32_STATIC_INLINE static inline
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

#define ZAPHOD_SCRAMBLE32(v,prime) STMT_START {  \
    v ^= (v>>9);                        \
    v ^= (v<<21);                       \
    v ^= (v>>16);                       \
    v *= prime;                         \
    v ^= (v>>17);                       \
    v ^= (v<<15);                       \
    v ^= (v>>23);                       \
} STMT_END

#define ZAPHOD32_FINALIZE(v0,v1,v2) STMT_START {          \
    ZAPHOD32_WARN3("v0=%08x v1=%08x v2=%08x - ZAPHOD32 FINALIZE\n", \
            (unsigned int)v0, (unsigned int)v1, (unsigned int)v2);  \
    v2 += v0;                       \
    v1 -= v2;                       \
    v1 = ROTL32(v1,  6);           \
    v2 ^= v1;                       \
    v2 = ROTL32(v2, 28);           \
    v1 ^= v2;                       \
    v0 += v1;                       \
    v1 = ROTL32(v1, 24);           \
    v2 += v1;                       \
    v2 = ROTL32(v2, 18) + v1;      \
    v0 ^= v2;                       \
    v0 = ROTL32(v0, 20);           \
    v2 += v0;                       \
    v1 ^= v2;                       \
    v0 += v1;                       \
    v0 = ROTL32(v0,  5);           \
    v2 += v0;                       \
    v2 = ROTL32(v2, 22);           \
    v0 -= v1;                       \
    v1 -= v2;                       \
    v1 = ROTL32(v1, 17);           \
} STMT_END

#define ZAPHOD32_MIX(v0,v1,v2,text) STMT_START {                              \
    ZAPHOD32_WARN4("v0=%08x v1=%08x v2=%08x - ZAPHOD32 %s MIX\n",                   \
            (unsigned int)v0,(unsigned int)v1,(unsigned int)v2, text );  \
    v0 = ROTL32(v0,16) - v2;   \
    v1 = ROTR32(v1,13) ^ v2;   \
    v2 = ROTL32(v2,17) + v1;   \
    v0 = ROTR32(v0, 2) + v1;   \
    v1 = ROTR32(v1,17) - v0;   \
    v2 = ROTR32(v2, 7) ^ v0;   \
} STMT_END


void zaphod32_seed_state (
    const U8 *seed_ch,
    const U8 *state_ch
) {
    U32 *seed= (U32 *)seed_ch;
    U32 *state= (U32 *)state_ch;
    state[0]= seed[0];
    state[1]= seed[1];
    state[2]= seed[2] << 1; /* lose the high bit of the seed - little endian */
    state[2] |= ((~seed[2]) & 0x01); /* and add in the reverse of the low bit */
    /* at this point we are guaranteed that seed[2] has at least one 0-bit,
     * and at least one 1-bit. Which means the overall seed does too. */
    /* then mix the shift out of them */
    ZAPHOD32_MIX(state[0],state[1],state[2],"SEED-STATE 1/3");
    ZAPHOD32_MIX(state[0],state[1],state[2],"SEED-STATE 2/3");
    ZAPHOD32_MIX(state[0],state[1],state[2],"SEED-STATE 3/3");
    /* and then scramble them too for good measure */
    ZAPHOD32_WARN3("v0=%08x v1=%08x v2=%08x - ZAPHOD32 SEED-STATE SCRAMBLE\n",
            (unsigned int)state[0], (unsigned int)state[1], (unsigned int)state[2]);
    ZAPHOD_SCRAMBLE32(state[0],0x9BD7E6C1UL);
    ZAPHOD_SCRAMBLE32(state[1],0x8019E4BBUL);
    ZAPHOD_SCRAMBLE32(state[2],0xE2607E49UL);
    ZAPHOD32_WARN3("v0=%08x v1=%08x v2=%08x - ZAPHOD32 SEED-STATE FINAL\n",
            (unsigned int)state[0], (unsigned int)state[1], (unsigned int)state[2]);
}

ZAPHOD32_STATIC_INLINE U32 zaphod32_hash_with_state(
    const U8 *state_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U32 *state= (U32 *)state_ch;
    U32 v0= state[0];
    U32 v1= state[1];
    U32 v2= state[2] ^ (0xC41A7AB1UL * (key_len + 1));
    U32 len= key_len;
    U32 hash;

    ZAPHOD32_WARN4("v0=%08x v1=%08x v2=%08x ln=%08x HASH START\n",
            (unsigned int)state[0], (unsigned int)state[1],
            (unsigned int)state[2], (unsigned int)key_len);

    while ( len >= 8 ){
        v1 -= U8TO32_LE(key); key += 4;
        v0 += U8TO32_LE(key); key += 4;
        ZAPHOD32_MIX(v0,v1,v2,"MIX 2-WORDS A");
        len -= 8;
    }

    if (len >= 4 ) {
        v1 -= U8TO32_LE(key); key += 4;
    }

    v0 += (U32)(len+1) << 24;
    switch (len & 0x3) {
        case 3: v0 += (U32)key[2] << 16;
        case 2: v0 += (U32)U8TO16_LE(key);
                break;
        case 1: v0 += (U32)key[0];
                break;
        case 0: v2 ^= 0xFF;
    }

    ZAPHOD32_FINALIZE(v0,v1,v2);
    hash = v0 ^ v1 ^ v2;

    ZAPHOD32_WARN4("v0=%08x v1=%08x v2=%08x hh=%08x - FINAL\n\n",
            (unsigned int)v0, (unsigned int)v1, (unsigned int)v2,
            (unsigned int)hash);

    return hash;
}

ZAPHOD32_STATIC_INLINE U32 zaphod32_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U32 state[3];
    zaphod32_seed_state(seed_ch,(U8*)state);
    return zaphod32_hash_with_state((U8*)state,key,key_len);
}

