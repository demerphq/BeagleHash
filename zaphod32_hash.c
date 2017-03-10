#ifdef __cplusplus
extern "C" {
#endif

#include "beagle_hash.h"
#include <stdio.h>

#define SCRAMBLE32(v,prime) STMT_START {  \
    v ^= (v>>9);                        \
    v ^= (v<<21);                       \
    v ^= (v>>16);                       \
    v *= prime;                         \
    v ^= (v>>17);                       \
    v ^= (v<<15);                       \
    v ^= (v>>23);                       \
} STMT_END

#define ZAPHOD32_FINALIZE(v0,v1,v2) STMT_START {          \
    WARN3("v0=%08x v1=%08x v2=%08x - ZAPHOD FINALIZE\n", \
            (unsigned int)v0, (unsigned int)v1, (unsigned int)v2);  \
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

#define ZAPHOD32_MIX(v0,v1,v2,text) STMT_START {                              \
    WARN4("v0=%08x v1=%08x v2=%08x - ZAPHOD %s MIX\n",                   \
            (unsigned int)v0,(unsigned int)v1,(unsigned int)v2, text );  \
    v0 = ROTL_32(v0,16) - v2;   \
    v1 = ROTR_32(v1,13) ^ v2;   \
    v2 = ROTL_32(v2,17) + v1;   \
    v0 = ROTR_32(v0, 2) + v1;   \
    v1 = ROTR_32(v1,17) - v0;   \
    v2 = ROTR_32(v2, 7) ^ v0;   \
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
    WARN3("v0=%08x v1=%08x v2=%08x - ZAPHOD SEED-STATE SCRAMBLE\n",
            (unsigned int)state[0], (unsigned int)state[1], (unsigned int)state[2]);
    SCRAMBLE32(state[0],0x9BD7E6C1UL);
    SCRAMBLE32(state[1],0x8019E4BBUL);
    SCRAMBLE32(state[2],0xE2607E49UL);
    WARN3("v0=%08x v1=%08x v2=%08x - ZAPHOD SEED-STATE FINAL\n",
            (unsigned int)state[0], (unsigned int)state[1], (unsigned int)state[2]);
}

BEAGLE_STATIC_INLINE U32 zaphod32_hash_with_state(
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

    WARN4("v0=%08x v1=%08x v2=%08x ln=%08x HASH START\n",
            (unsigned int)state[0], (unsigned int)state[1], (unsigned int)state[2], (unsigned int)key_len);
    switch (key_len) {
        case 8: v1 -= ((U32)key[7]) << 24;
        case 7: v0 += ((U32)key[6]) << 24;
        case 6: v1 -= ((U32)key[5]) << 16;
        case 5: v0 += ((U32)key[4]) << 16;
        case 4: v1 -= ((U32)key[3]) << 8;
        case 3: v0 += ((U32)key[2]) << 8;
        case 2: v1 -= ((U32)key[1]) << 0;
        case 1: v0 += ((U32)key[0]) << 0;
        case 0: break;
        case 15: /* fallthrough */
        case 14: /* fallthrough */
        case 13: /* fallthrough */
        case 12: goto zaphod32_read3;
        case 11: /* fallthrough */
        case 10:
        case  9: goto zaphod32_read2;
        default:{
            do {
                v1 -= U8TO32_LE(key); key += 4;
                v0 += U8TO32_LE(key); key += 4;
                ZAPHOD32_MIX(v0,v1,v2,"MIX 2-WORDS A");
                v1 -= U8TO32_LE(key); key += 4;
                v0 += U8TO32_LE(key); key += 4;
                ZAPHOD32_MIX(v0,v1,v2,"MIX 2-WORDS B");
                len -= 16;
            } while ( len >= 16 );
            switch ( len >> 2 ) {
                case 3:
                zaphod32_read3:
                        v1 -= U8TO32_LE(key); key += 4;
                        v0 += U8TO32_LE(key); key += 4;
                        ZAPHOD32_MIX(v0,v1,v2,"MIX 2-WORDS C");
                case 1:
                        v1 -= U8TO32_LE(key); key += 4;
                        break;
                case 2:
                zaphod32_read2:
                        v1 -= U8TO32_LE(key); key += 4;
                        v0 += U8TO32_LE(key); key += 4;
                        ZAPHOD32_MIX(v0,v1,v2,"MIX 2-WORDS D");
                        break;
                case 0:
                default: break;
            }
            v0 += ((key_len+1) << 24);
            switch (len & 0x3) {
                case 3: v0 += (U32)key[2] << 16;
                case 2: v0 += (U32)key[1] <<  8;
                case 1: v0 += (U32)key[0];
                        break;
                case 0:
                default:v2 ^= 0xFF;
                        break;
            }
        }
    }
    ZAPHOD32_FINALIZE(v0,v1,v2);
    hash = v0 ^ v1 ^ v2;

    WARN4("v0=%08x v1=%08x v2=%08x hh=%08x - FINAL\n\n",
            (unsigned int)v0, (unsigned int)v1, (unsigned int)v2,
            (unsigned int)hash);
    return hash;
}

BEAGLE_STATIC_INLINE U32 zaphod32_hash(
    const U8 *seed_ch,
    const U8 *key,
    const STRLEN key_len
) {
    U32 state[3];
    zaphod32_seed_state(seed_ch,(U8*)state);
    return zaphod32_hash_with_state((U8*)state,key,key_len);
}

void zaphod32_hash_smhasher_test(const void *key, STRLEN len, U32 seed_base, void *out) {
    U32 seed[3]= { seed_base, 1, 2, };

    *((U32 *)out)= zaphod32_hash((U8*)seed, (U8 *)key, len);
}

void zaphod32_seed_state_smhasher_test(int in_bits, const void *seed, void *state) {
    zaphod32_seed_state((U8*)seed,(U8*)state);
}

void zaphod32_hash_with_state_smhasher_test(const void *key, STRLEN len, const void *seed, void *out) {
    *((U32 *)out)= zaphod32_hash_with_state((U8*)seed, (U8 *)key, len);
}

#ifdef __cplusplus
}
#endif

