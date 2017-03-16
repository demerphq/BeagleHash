#include <stdio.h>
#include "stadtx_hash.h"
#include "zaphod32_hash.h"
#include "zaphod64_hash.h"
#include "phat_hash.h"
#include "sbox_hash.h"
#include "beagle_hash.h"


void print64(const char *str, const void *b, int num) {
    U64 *blocks = (U64 *)b;
    printf("# %s { ", str);
    while ( num-- > 0 ) {
        printf("0x%016lx%s", *blocks++, num > 0 ? ", " : "");
    }
    printf(" }\n");
}

void print32(const char *str, const void *b, int num) {
    U32 *blocks = (U32 *)b;
    printf("# %s { ", str);
    while ( num-- > 0 ) {
        printf("0x%08x%s", *blocks++, num > 0 ? ", " : "");
    }
    printf(" }\n");
}

char *test_string = "The shaved yak drank from the bitter well";
STRLEN test_string_len;
const U64 seed[3] = { 0x1234567890123456UL, 0x9876543210ABCDEF, 0xABCDEF0123456789 };

const U64 stadtx_state_expected[4] = { 0x0d42d19b274a6366, 0x7cdddde71be117f0, 0x1f4db39c60100ad2, 0xd430c8ca8ac732eb };
const U64 stadtx_hash_expected = 0x5f7aa2a6f52125cdUl;
const U32 zaphod32_hash_expected = 0x8247da16;
const U32 zaphod32_state_expected[3] = { 0x2c2395a1, 0x05820670, 0x3fb4c1f7 };
const U64 zaphod64_hash_expected = 0xb1085ad71777a135;
const U64 zaphod64_state_expected[3] = { 0x7348467c9465b05a, 0xd3d08bc39fb12b4e, 0xc7f77fbd0e4a525f };

int main(int argc, char **argv) {
    U64 state[4];
    U64 a64;
    U32 a32;
    int failed;

    test_string_len = strlen(test_string);

    stadtx_seed_state((U8*)seed,(U8*)state);
    failed = memcmp(stadtx_state_expected, state, 4 * sizeof(U64))!=0;
    if (failed) {
        printf("#Failed test 1:\n");
        print64(" seed:", seed, 2);
        print64("state:", state, 4);
        print64(" want:", stadtx_state_expected, 4);
        printf("not ");
    }
    printf("ok 1 - stadtx seed_stats\n");

    a64= stadtx_hash_with_state((U8*)state,test_string,test_string_len);
    failed = a64 != stadtx_hash_expected;
    if (failed){
        printf("#Failed test 2:\n");
        printf("#got : 0x%016lx\n#want: 0x%016lx\n", a64, stadtx_hash_expected);
        printf("not ");
    }
    printf("ok 2 - stadtx hash test string hash\n");

    zaphod32_seed_state((U8*)seed,(U8*)state);
    failed = memcmp(zaphod32_state_expected, state, 3 * sizeof(U32))!=0;
    if (failed) {
        printf("#Failed test 3:\n");
        print32(" seed:", seed, 3);
        print32("state:", state, 3);
        print32(" want:", zaphod32_state_expected, 3);
        printf("not ");
    }
    printf("ok 3 - zaphod32 seed_stats\n");
    
    a32= zaphod32_hash_with_state((U8*)state,test_string,test_string_len);
    failed = a32 != zaphod32_hash_expected;
    if (failed) {
        printf("#Failed test 4:\n");
        printf("#got : 0x%08x\n#want: 0x%08x\n", a32, zaphod32_hash_expected);
        printf("not ");
    }
    printf("ok 4 - zaphod32 hash test string hash\n");
    
    zaphod64_seed_state((U8*)seed,(U8*)state);
    failed = memcmp(zaphod64_state_expected, state, 3 * sizeof(U64))!=0;
    if (failed) {
        printf("#Failed test 5:\n");
        print64(" seed:", seed, 3);
        print64("state:", state, 3);
        print64(" want:", zaphod64_state_expected, 3);
        printf("not ");
    }
    printf("ok 5 - zaphod64 seed_stats\n");
    
    a64= zaphod64_hash_with_state((U8*)state,test_string,test_string_len);
    failed = a64 != zaphod64_hash_expected;
    if (failed) {
        printf("#Failed test 6:\n");
        printf("#got : 0x%016lx\n#want: 0x%016lx\n", a64, zaphod64_hash_expected);
        printf("not ");
    }
    printf("ok 6 - zaphod64 hash test string hash\n");

}
