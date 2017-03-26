#ifndef DEBUG_PHAT_HASH
#define DEBUG_PHAT_HASH 0

#if DEBUG_PHAT_HASH == 1
#include <stdio.h>
#define PHAT_WARN6(pat,v0,v1,v2,v3,v4,v5)    printf(pat, v0, v1, v2, v3, v4, v5)
#define PHAT_WARN5(pat,v0,v1,v2,v3,v4)       printf(pat, v0, v1, v2, v3, v4)
#define PHAT_WARN4(pat,v0,v1,v2,v3)          printf(pat, v0, v1, v2, v3)
#define PHAT_WARN3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#define PHAT_WARN2(pat,v0,v1)                printf(pat, v0, v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#elif DEBUG_PHAT_HASH == 2
#define PHAT_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define PHAT_WARN5(pat,v0,v1,v2,v3,v4)
#define PHAT_WARN4(pat,v0,v1,v2,v3)
#define PHAT_WARN3(pat,v0,v1,v2)
#define PHAT_WARN2(pat,v0,v1)
#define NOTE3(pat,v0,v1,v2)             printf(pat, v0, v1, v2)
#else
#define PHAT_WARN6(pat,v0,v1,v2,v3,v4,v5)
#define PHAT_WARN5(pat,v0,v1,v2,v3,v4)
#define PHAT_WARN4(pat,v0,v1,v2,v3)
#define PHAT_WARN3(pat,v0,v1,v2)
#define NOTE3(pat,v0,v1,v2)
#define PHAT_WARN2(pat,v0,v1)
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

#ifndef PHAT_STATIC_INLINE
#ifdef PERL_STATIC_INLINE
#define PHAT_STATIC_INLINE PERL_STATIC_INLINE
#else
#define PHAT_STATIC_INLINE static inline
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

#define PHAT_K0_U32 0xE597A751UL
#define PHAT_K1_U32 0xA9166027UL
#define PHAT_K2_U32 0xBA14E5F7UL
#define PHAT_K3_U32 0x8BF5E321UL
#define PHAT_K4_U32 0xFEC63225UL
#define PHAT_K5_U32 0xC3D00531UL
#define PHAT_K6_U32 0xD9FBAD75UL
#define PHAT_K7_U32 0x9BFB6FEBUL

/*
#define PHAT_MIXER 5
Alignment  0 -  1.446 bytes/cycle - 4137.59 MiB/sec @ 3 ghz
Alignment  1 -  1.446 bytes/cycle - 4138.36 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    27.45 cycles/hash
Small key speed test -    1-byte keys -    29.08 cycles/hash
Small key speed test -    4-byte keys -    26.96 cycles/hash
Small key speed test -   64-byte keys -    65.11 cycles/hash

#define PHAT_MIXER 6
Alignment  0 -  1.482 bytes/cycle - 4239.62 MiB/sec @ 3 ghz
Alignment  1 -  1.482 bytes/cycle - 4240.03 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    27.02 cycles/hash
Small key speed test -    1-byte keys -    29.26 cycles/hash
Small key speed test -    4-byte keys -    26.91 cycles/hash
Small key speed test -   64-byte keys -    68.27 cycles/hash

#define PHAT_MIXER 7
Alignment  0 -  1.458 bytes/cycle - 4170.82 MiB/sec @ 3 ghz
Alignment  1 -  1.423 bytes/cycle - 4070.41 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    26.71 cycles/hash
Small key speed test -    1-byte keys -    28.62 cycles/hash
Small key speed test -    4-byte keys -    27.07 cycles/hash
Small key speed test -   64-byte keys -    67.74 cycles/hash

#define PHAT_MIXER 8
Alignment  0 -  1.568 bytes/cycle - 4486.96 MiB/sec @ 3 ghz
Alignment  1 -  1.568 bytes/cycle - 4486.50 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    28.45 cycles/hash
Small key speed test -    1-byte keys -    28.79 cycles/hash
Small key speed test -    4-byte keys -    26.87 cycles/hash
Small key speed test -   64-byte keys -    66.73 cycles/hash

#define PHAT_MIXER 9
Alignment  0 -  1.568 bytes/cycle - 4486.68 MiB/sec @ 3 ghz
Alignment  1 -  1.568 bytes/cycle - 4487.18 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    28.76 cycles/hash
Small key speed test -    1-byte keys -    28.87 cycles/hash
Small key speed test -    4-byte keys -    26.67 cycles/hash
Small key speed test -   64-byte keys -    67.32 cycles/hash

#define PHAT_MIXER 10
Alignment  0 -  1.568 bytes/cycle - 4486.89 MiB/sec @ 3 ghz
Alignment  1 -  1.568 bytes/cycle - 4486.67 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    26.39 cycles/hash
Small key speed test -    1-byte keys -    29.87 cycles/hash
Small key speed test -    4-byte keys -    26.72 cycles/hash
Small key speed test -   64-byte keys -    66.95 cycles/hash

#define PHAT_MIXER 11
Alignment  0 -  1.568 bytes/cycle - 4487.22 MiB/sec @ 3 ghz
Alignment  1 -  1.569 bytes/cycle - 4487.58 MiB/sec @ 3 ghz
Small key speed test -    0-byte keys -    27.25 cycles/hash
Small key speed test -    1-byte keys -    29.74 cycles/hash
Small key speed test -    4-byte keys -    26.99 cycles/hash
Small key speed test -   64-byte keys -    66.74 cycles/hash

*/
#define PHAT_MIXER 6

#if PHAT_MIXER == 0
#error Bad PHAT MIXER
#elif PHAT_MIXER == 12
#define PHAT_k0 PHAT_K7_U32
#define PHAT_k1 PHAT_K4_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START {                        \
    PHAT_WARN6("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(%d) %s\n",       \
            (unsigned int)v0, (unsigned int)v1,                       \
            (unsigned int)v2, (unsigned int)v3, PHAT_MIXER, text);   \
    v2 ^= v0;                           \
    v1 = ROTL32(v1,11) + v2;           \
    v3 ^= v1;                           \
    v2 = ROTL32(v2, 5) - v3;           \
    v3 = ROTR32(v3, 8) - v2;           \
    v0 ^= v3;                           \
    v3 = ROTL32(v3,14) - v0;           \
    v3 = ROTR32(v3,27);                \
    v0 -= v3;                           \
    v1 ^= v0;                           \
    v2 += v1;                           \
} STMT_END
#elif PHAT_MIXER == 11
#define PHAT_k0 PHAT_K7_U32
#define PHAT_k1 PHAT_K4_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START { \
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(11) %s\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3, text);   \
    v0 = ROTL32(v0,13) + v3;   \
    v1 ^= v2;                   \
    v3 = ROTL32(v3, 9) + v1;   \
    v2 ^= v0;                   \
    v0 = ROTL32(v0,14) ^ v3;   \
    v1 = ROTL32(v1,25) - v2;   \
    v3 ^= v1;                   \
    v2 = ROTL32(v2, 4) - v0;   \
} STMT_END
#elif PHAT_MIXER == 10
#define PHAT_k0 PHAT_K5_U32
#define PHAT_k1 PHAT_K2_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START { \
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(10) %s\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3, text);   \
    v0 = ROTL32(v0,13) - v3;   \
    v1 ^= v2;                   \
    v3 = ROTL32(v3, 9) + v1;   \
    v2 ^= v0;                   \
    v0 = ROTL32(v0,14) ^ v3;   \
    v1 = ROTL32(v1,25) - v2;   \
    v3 ^= v1;                   \
    v2 = ROTL32(v2, 4) - v0;   \
} STMT_END
#elif PHAT_MIXER == 9
#define PHAT_k0 PHAT_K2_U32
#define PHAT_k1 PHAT_K7_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START { \
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(9) %s\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3, text);   \
    v0 = ROTL32(v0,13) + v3;   \
    v1 ^= v2;                   \
    v3 = ROTL32(v3,23) + v1;   \
    v2 ^= v0;                   \
    v0 = ROTL32(v0, 6) ^ v3;   \
    v1 = ROTL32(v1,25) + v2;   \
    v3 ^= v1;                   \
    v2 = ROTL32(v2, 9) - v0;   \
} STMT_END
#elif PHAT_MIXER == 8
#define PHAT_k0 PHAT_K3_U32
#define PHAT_k1 PHAT_K6_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START { \
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(8) %s\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3, text);   \
    v0 = ROTL32(v0,13) + v3;   \
    v1 ^= v2;                   \
    v3 = ROTL32(v3,10) + v1;   \
    v2 ^= v0;                   \
    v0 = ROTL32(v0,14) ^ v3;   \
    v1 = ROTL32(v1,25) + v2;   \
    v3 ^= v1;                   \
    v2 = ROTL32(v2,17) - v0;   \
} STMT_END
#elif PHAT_MIXER == 7
#define PHAT_k0 PHAT_K0_U32
#define PHAT_k1 PHAT_K7_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START { \
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(7) %s\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3, text);   \
    v0 -= v3;                   \
    v1 = ROTL32(v1, 7) ^ v2;   \
    v3 = ROTL32(v3, 9) + v1;   \
    v2 = ROTL32(v2,18) ^ v0;   \
    v0 = ROTL32(v0,27) ^ v3;   \
    v1 = ROTL32(v1, 5) + v2;   \
    v3 = ROTL32(v3, 8) ^ v1;   \
    v2 += v0;   \
} STMT_END
#elif PHAT_MIXER == 6
#define PHAT_k0 PHAT_K0_U32
#define PHAT_k1 PHAT_K7_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START {  \
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(6) %s\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3, text);   \
    v0 = ROTL32(v0,23) ^ v3;   \
    v1 = ROTL32(v1, 5) + v2;   \
    v3 += v1;                   \
    v2 = ROTL32(v2, 3) + v0;   \
    v0 = ROTL32(v0,12) + v3;   \
    v1 ^= v2;                   \
    v3 = ROTL32(v3,19) - v1;   \
    v2 = ROTL32(v2,22) ^ v0;   \
} STMT_END
#elif PHAT_MIXER == 5
#define PHAT_k0 PHAT_K0_U32
#define PHAT_k1 PHAT_K7_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START {  \
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(5) %s\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3, text);   \
    v0 = ROTL32(v0,20) - v3;   \
    v1 = ROTL32(v1, 3) + v2;   \
    v3 = ROTL32(v3,27) ^ v1;   \
    v2 ^= v0;                   \
    v0 -= v3;                   \
    v1 = ROTL32(v1,14) + v2;   \
    v3 ^= v1;                   \
    v2 = ROTL32(v2,24) ^ v0;   \
} STMT_END
#elif PHAT_MIXER == 0
#define PHAT_k0 PHAT_K0_U32
#define PHAT_k1 PHAT_K7_U32
#define PHAT_MIX(v0,v1,v2,v3,text) STMT_START {  \
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT(0) %s\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3, text);   \
} STMT_END
#else
#error "No PHAT_MIXER defined"
#endif

#define PHAT_FINALIZER 2

#if PHAT_FINALIZER == 1
#define PHAT_FINALIZE(v0,v1,v2,v3) STMT_START {   \
    PHAT_WARN4("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT_FINALIZE(1)\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3);   \
    v0 ^= v3;                   \
    v0 += v2;                   \
    v1 += v0;                   \
    v3 = ROTL32(v3,19) + v1;   \
    v2 += v3;                   \
    v1 = ROTL32(v1,25) ^ v3;   \
    v3 = ROTL32(v3, 5) - v1;   \
    v2 = ROTL32(v2, 3) ^ v3;   \
    v3 = ROTL32(v3,17) - v2;   \
    v2 = ROTL32(v2,14) - v3;   \
    v0 ^= v2;                   \
    v1 ^= v0;                   \
    v2 = ROTL32(v2,13) - v0;   \
    v0 = ROTL32(v0,22) ^ v2;   \
    v1 = ROTL32(v1,17) - v0;   \
    v3 ^= v1;                   \
    v0 = ROTL32(v0, 6);        \
    v1 = ROTL32(v1,20);        \
    v2 -= v0;                   \
} STMT_END
#elif PHAT_FINALIZER == 2
#define PHAT_FINALIZE(v0,v1,v2,v3) STMT_START {   \
    PHAT_WARN4("v0=%08x v1=%08x v2=%08x v3=%08x - PHAT_FINALIZE(2)\n", \
            (unsigned int)v0, (unsigned int)v1,    \
            (unsigned int)v2, (unsigned int)v3);   \
    v1 += v3;                   \
    v0 ^= v2;                   \
    v1 = ROTL32(v1, 6) - v0;   \
    v3 -= v1;                   \
    v1 = ROTL32(v1,10) ^ v3;   \
    v3 = ROTL32(v3, 5) + v1;   \
    v2 = ROTL32(v2,13) ^ v3;   \
    v3 = ROTL32(v3,17) + v2;   \
    v2 = ROTL32(v2,15) ^ v3;   \
    v0 += v2;                   \
    v1 ^= v0;                   \
    v2 = ROTL32(v2, 9) - v1;   \
    v0 = ROTL32(v0,22) + v2;   \
    v1 = ROTL32(v1,18) - v0;   \
    v3 = ROTL32(v3,22) + v1;   \
    v0 = ROTL32(v0, 4);        \
    v1 = ROTL32(v1,11) - v0;   \
} STMT_END
#else
#define PHAT_FINALIZE(v0,v1,v2,v3) STMT_START {   \
    PHAT_MIX(v0,v1,v2,v3,"FINALIZE MIX #1");\
    PHAT_MIX(v1,v2,v3,v0,"FINALIZE MIX #2");\
    PHAT_MIX(v0,v1,v2,v3,"FINALIZE MIX #3");\
} STMT_END
#endif
#define DO_PHAT_MIX(v0,v1,v2,v3,text) STMT_START { \
    PHAT_MIX(v0,v1,v2,v3,"" text);\
} STMT_END

PHAT_STATIC_INLINE U32 phat_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U32 *seed= (U32 *)seed_ch;
    U32 v0= seed[0] ^ (0x940CF22FUL * (key_len+1));
    U32 v1= seed[1] ^ (0x82AD1DB1UL * (key_len+2));
    U32 v2= seed[2] ^ (0xA250D491UL * (key_len+3));
    U32 v3= seed[0] ^ (0xBAC9A9D9UL * (key_len+4));
    U32 final;
    U32 len= key_len;

    switch (len) {
        case 12: v3 += ((U32)key[11] << 16);
        case 11: v2 -= ((U32)key[10] << 16);
        case 10: v1 += ((U32)key[9]  << 16);
        case  9: v0 -= ((U32)key[8]  << 16);
        case  8: v3 += ((U32)key[7]  << 8);
        case  7: v2 -= ((U32)key[6]  << 8);
        case  6: v1 += ((U32)key[5]  << 8);
        case  5: v0 -= ((U32)key[4]  << 8);
        case  4: v3 += (U32)key[3];
        case  3: v2 -= (U32)key[2];
        case  2: v1 += (U32)key[1];
        case  1: v0 -= (U32)key[0];
        case  0: break;
        case 13: /*fallthrough*/
        case 14: /*fallthrough*/
        case 15: goto phat_read3;
        case 16:
        default:
        do {
            v3 += U8TO32_LE(key); key+=4;
            v2 += U8TO32_LE(key); key+=4;
            DO_PHAT_MIX(v0,v1,v2,v3,"MIX v3/v2");
            v1 += U8TO32_LE(key); key+=4;
            v0 += U8TO32_LE(key); key+=4;
            DO_PHAT_MIX(v0,v1,v2,v3,"MIX v1/v0");
            len -= 16;
        } while ( len >= 16 );
        switch (len >> 2) {
             phat_read3:
            case 3: v3 += U8TO32_LE(key); key+=4;
            case 2: v2 += U8TO32_LE(key); key+=4;
            DO_PHAT_MIX(v0,v1,v2,v3,"MIX TAIL v3/v2");
            case 1: v1 += U8TO32_LE(key); key+=4;
            case 0:
            default: break;
        }
        v0 += ((len+1) << 24);
        switch (len & 3) {
            case 3: v0 += (U32)key[2] << 16;
            case 2: v0 += (U32)key[1] <<  8;
            case 1: v0 += (U32)key[0];
            case 0:
            default: break;
        }
        v3 ^= 0xFF;
        DO_PHAT_MIX(v0,v1,v2,v3,"MIX TAIL v1/v0");
    }
    PHAT_FINALIZE(v0,v1,v2,v3);

    final = (v1 ^ v2) ^ (v0 ^ v3);
    PHAT_WARN5("v0=%08x v1=%08x v2=%08x v3=%08x hh=%08x - PHAT RESULT\n\n",
            (unsigned int)v0, (unsigned int)v1,
            (unsigned int)v2, (unsigned int)v3,
            (unsigned int)final);

    return final;
}

#endif
