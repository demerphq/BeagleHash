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

/*---*/

const U64 stadtx_expected_state[4] = 
    { 0x0d42d19b274a6366, 0x7cdddde71be117f0, 0x1f4db39c60100ad2, 0xd430c8ca8ac732eb };
const U64 stadtx_expected_hash = 0x5f7aa2a6f52125cdUl;

/*---*/

const U32 zaphod32_expected_state[3] = { 0xdaa1089c, 0xae1b8e20, 0x8dfb952e };
const U32 zaphod32_expected_hash = 0xc6f3412c;

/*---*/

const U64 zaphod64_expected_state[3] = 
    { 0x7348467c9465b05a, 0xd3d08bc39fb12b4e, 0x67a0860b5474f3c7 };
const U64 zaphod64_expected_hash = 0xbe93e83446aeefd4;

/*---*/
typedef void (*pfSeedState) ( const U8 *seed, U8 *state );
typedef U32 (*pfHash32) ( const U8 *state, const U8 *blob, const STRLEN len);
typedef U64 (*pfHash64) ( const U8 *state, const U8 *blob, const STRLEN len);

/*---*/

char *test_string = "The shaved yak drank from the bitter well";
STRLEN test_string_len;
const U64 seed[3] = { 0x1234567890123456UL, 0x9876543210ABCDEF, 0xABCDEF0123456789 };


int testnum = 0;
int num_failed = 0;

void test64 (
        const char* name,
        pfSeedState seed_state, pfHash64 hash, 
        const U64 *seed, const U64 *expected_state, const U64 expected_hash, 
        const int seedwords, const int statewords) 
{
    U64 state[4];
    U64 a64;
    int failed_result;

    int n= testnum++;
    seed_state((U8*)seed,(U8*)state);
    failed_result = memcmp(expected_state, state, statewords * sizeof(U64))!=0;
    if (failed_result)
        printf("#%s failed_result test %d:\n", name, n);
    print64(" seed:", seed, seedwords);
    print64("state:", state, statewords);
    if (failed_result) {
        print64(" want:", expected_state, statewords);
        printf("not ");
        num_failed++;
    }
    printf("ok %d - %s state seeded as expected\n", n, name);

    n= testnum++;
    a64= hash((U8*)state,test_string,test_string_len);
    failed_result = a64 != expected_hash;
    if (failed_result)
        printf("#%s failed_result test %d:\n", name, n);
    printf("#hash: 0x%016lx\n",a64);
    if (failed_result){
        printf("#want: 0x%016lx\nnot", expected_hash);
        num_failed++;
    }
}

void test32 (
        const char *name,
        pfSeedState seed_state, pfHash32 hash, 
        const U32 *seed, const U32 *expected_state, const U32 expected_hash, 
        const int seedwords, const int statewords) 
{
    U32 state[4];
    U32 a32;
    int failed_result;

    int n= testnum++;
    seed_state((U8*)seed,(U8*)state);
    failed_result = memcmp(expected_state, state, statewords * sizeof(U32))!=0;
    if (failed_result)
        printf("#%s failed_result test %d:\n", name, n);
    print32(" seed:", seed, seedwords);
    print32("state:", state, statewords);
    if (failed_result) {
        print64(" want:", expected_state, statewords);
        printf("not ");
    }
    printf("ok %d - %s state seeded as expected\n", n, name);

    n= testnum++;
    a32= hash((U8*)state,test_string,test_string_len);
    failed_result = a32 != expected_hash;
    if (failed_result)
        printf("#%s failed_result test %d:\n", name, n);
    printf("#hash: 0x%08x\n",a32);
    if (failed_result)
        printf("#want: 0x%08x\nnot", expected_hash);
}

int main(int argc, char **argv) {
    U64 state[4];
    U64 a64;
    U32 a32;

    test_string_len = strlen(test_string);

    test64("StadtX", stadtx_seed_state,stadtx_hash_with_state,
            seed, stadtx_expected_state, stadtx_expected_hash, 2,4);
    
    test32("Zaphod32", zaphod32_seed_state,zaphod32_hash_with_state,
            (U32*)seed, zaphod32_expected_state, zaphod32_expected_hash, 3,3);
    
    test64("Zaphod64", zaphod64_seed_state, zaphod64_hash_with_state,
            seed, zaphod64_expected_state, zaphod64_expected_hash, 3, 3);

}
