#include <stdint.h>
#include <string.h>
#include "skinny.h"

const ubyte sboxLookupTable[256] = {
0x65 ,0x4c ,0x6a ,0x42 ,0x4b ,0x63 ,0x43 ,0x6b ,0x55 ,0x75 ,0x5a ,0x7a ,0x53 ,0x73 ,0x5b ,0x7b ,
0x35 ,0x8c ,0x3a ,0x81 ,0x89 ,0x33 ,0x80 ,0x3b ,0x95 ,0x25 ,0x98 ,0x2a ,0x90 ,0x23 ,0x99 ,0x2b ,
0xe5 ,0xcc ,0xe8 ,0xc1 ,0xc9 ,0xe0 ,0xc0 ,0xe9 ,0xd5 ,0xf5 ,0xd8 ,0xf8 ,0xd0 ,0xf0 ,0xd9 ,0xf9 ,
0xa5 ,0x1c ,0xa8 ,0x12 ,0x1b ,0xa0 ,0x13 ,0xa9 ,0x05 ,0xb5 ,0x0a ,0xb8 ,0x03 ,0xb0 ,0x0b ,0xb9 ,
0x32 ,0x88 ,0x3c ,0x85 ,0x8d ,0x34 ,0x84 ,0x3d ,0x91 ,0x22 ,0x9c ,0x2c ,0x94 ,0x24 ,0x9d ,0x2d ,
0x62 ,0x4a ,0x6c ,0x45 ,0x4d ,0x64 ,0x44 ,0x6d ,0x52 ,0x72 ,0x5c ,0x7c ,0x54 ,0x74 ,0x5d ,0x7d ,
0xa1 ,0x1a ,0xac ,0x15 ,0x1d ,0xa4 ,0x14 ,0xad ,0x02 ,0xb1 ,0x0c ,0xbc ,0x04 ,0xb4 ,0x0d ,0xbd ,
0xe1 ,0xc8 ,0xec ,0xc5 ,0xcd ,0xe4 ,0xc4 ,0xed ,0xd1 ,0xf1 ,0xdc ,0xfc ,0xd4 ,0xf4 ,0xdd ,0xfd ,
0x36 ,0x8e ,0x38 ,0x82 ,0x8b ,0x30 ,0x83 ,0x39 ,0x96 ,0x26 ,0x9a ,0x28 ,0x93 ,0x20 ,0x9b ,0x29 ,
0x66 ,0x4e ,0x68 ,0x41 ,0x49 ,0x60 ,0x40 ,0x69 ,0x56 ,0x76 ,0x58 ,0x78 ,0x50 ,0x70 ,0x59 ,0x79 ,
0xa6 ,0x1e ,0xaa ,0x11 ,0x19 ,0xa3 ,0x10 ,0xab ,0x06 ,0xb6 ,0x08 ,0xba ,0x00 ,0xb3 ,0x09 ,0xbb ,
0xe6 ,0xce ,0xea ,0xc2 ,0xcb ,0xe3 ,0xc3 ,0xeb ,0xd6 ,0xf6 ,0xda ,0xfa ,0xd3 ,0xf3 ,0xdb ,0xfb ,
0x31 ,0x8a ,0x3e ,0x86 ,0x8f ,0x37 ,0x87 ,0x3f ,0x92 ,0x21 ,0x9e ,0x2e ,0x97 ,0x27 ,0x9f ,0x2f ,
0x61 ,0x48 ,0x6e ,0x46 ,0x4f ,0x67 ,0x47 ,0x6f ,0x51 ,0x71 ,0x5e ,0x7e ,0x57 ,0x77 ,0x5f ,0x7f ,
0xa2 ,0x18 ,0xae ,0x16 ,0x1f ,0xa7 ,0x17 ,0xaf ,0x01 ,0xb2 ,0x0e ,0xbe ,0x07 ,0xb7 ,0x0f ,0xbf ,
0xe2 ,0xca ,0xee ,0xc6 ,0xcf ,0xe7 ,0xc7 ,0xef ,0xd2 ,0xf2 ,0xde ,0xfe ,0xd7 ,0xf7 ,0xdf ,0xff
};

const ubyte roundConstantsLookupTable[] = {
    0x01,0x03,0x07,0x0F,0x1F,0x3E,0x3D,0x3B,
    0x37,0x2F,0x1E,0x3C,0x39,0x33,0x27,0x0E,
    0x1D,0x3A,0x35,0x2B,0x16,0x2C,0x18,0x30,
    0x21,0x02,0x05,0x0B,0x17,0x2E,0x1C,0x38,
    0x31,0x23,0x06,0x0D,0x1B,0x36,0x2D,0x1A,
    0x34,0x29,0x12,0x24,0x08,0x11,0x22,0x04,
    0x09,0x13,0x26,0x0C,0x19,0x32,0x25,0x0A,
    0x15,0x2A,0x14,0x28,0x10,0x20
};

/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k) {
    const int rounds = 56;
    for(int i = 0; i < rounds; ++i)
    {
        ubyte* subCellOut = subCells(p);
        ubyte* addConstsOut = AddConstants(subCellOut, i);
        ubyte* addRoundTwekOut = AddRoundTweakey(addConstsOut, k);
        ubyte* shiftRowOut = shiftRows(addRoundTwekOut);
        ubyte* mixColumnsOut = mixColumns(shiftRowOut);

        ubyte* nextTweakeyOut = computeNextTweakey(k);

        memcpy(p, mixColumnsOut, sizeof(ubyte) * 16);
        memcpy(k, nextTweakeyOut, sizeof(ubyte) * 48);

        free(subCellOut);
        free(addConstsOut);
        free(addRoundTwekOut);
        free(shiftRowOut);
        free(mixColumnsOut);
        free(nextTweakeyOut);
    }
    memcpy(c, p, sizeof(ubyte) * 16);
}

ubyte* computeNextTweakey(ubyte *p)
{
    const int perm[] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 
            1, 2, 3, 4, 5, 6, 7};
    ubyte* result = malloc(sizeof(ubyte) * 48);

    for(int j = 0; j < 3; ++j)
    {
        ubyte* permOut = permuteNumbers(p + j * 16, perm, 16);
        memcpy(result + j * 16, permOut, sizeof(ubyte) * 16);
        free(permOut);
    }

    const int bitPerm[] = {0, 7, 6, 5, 4, 3, 2, 1};

    for(int i = 0; i < 16 * 2; ++i)
    {
        ubyte msb = (result[i]>>6 & 1) ^ (result[i] & 1);
        result[i] = permuteBits(result[i], bitPerm);
        result[i] = result[i] | msb << 7;
    }

    return result;
}

ubyte* subCells(ubyte *x)
{
    const int length = 16;
    ubyte* result = malloc(sizeof(ubyte) * length);
    for(int i = 0; i < length; ++i)
    {
        result = subCell(x[i]);
    }
    return result;
}

ubyte subCell(ubyte x)
{
    const int permutationOne[] = {2, 1, 7, 6, 4, 0, 3, 5};
    const int permutationTwo[] = {7, 6, 5, 4, 3, 1, 2, 0};
    for(int i = 0; i < 3; ++i)
    {
        x = sboxLookupTable[x];
        x = permuteBits(x, permutationOne);
    }
    x = sboxLookupTable[x];
    x = permuteBits(x, permutationTwo);
    return x;
}

ubyte permuteBits(ubyte x, int *perm)
{
    ubyte result = 0;
    for(int i = 0; i < 8; ++i)
    {
        ubyte bitVal = 1 & (x >> perm[i]);
        result |= (bitVal << (7 - i));
    }
    return result;
}

RoundConstants generateLFSRConstants(int roundNumber)
{
    ubyte state = roundConstantsLookupTable[roundNumber];
    RoundConstants result;
    
    result.c0 = 0x2;
    result.c1 = (state & 0b1110) >> 1;
    result.c2 = (state & 0b100000) | (state & 0b10000);
    result.c2 = result.c2 >> 4;

    return result;
}

ubyte* AddConstants(ubyte *x, int roundNumber)
{
    const int length = 16;

    RoundConstants consts = generateLFSRConstants(roundNumber);
    ubyte* result = malloc(sizeof(ubyte) * length);

    memcpy(result, x, sizeof(ubyte) * length);
    result[0] ^= consts.c0;
    result[4] ^= consts.c1;
    result[8] ^= consts.c2;

    return result;
}

ubyte* AddRoundTweakey(ubyte* input, ubyte* tweakey)
{
    const int length = 16;
    ubyte* result = malloc(sizeof(ubyte) * length);

    for(int i = 0; i < length; ++i)
    {
        result[i] = input[i] ^ tweakey[i] ^ 
            tweakey[i + 16] ^ tweakey[i + 32];
    }
    return result;
}

ubyte* permuteNumbers(ubyte* input, int *perm, int length)
{
    ubyte* result = malloc(sizeof(ubyte) * length);

    for(int i = 0; i < length; ++i)
    {
        result[i] = input[perm[i]];
    }

    return result;
}

// ubyte* createNextTweakKeyRow(ubyte* keyRow)
// {
//     int rowLength = 8;
//     const int perm[] = {0, 6, 7, 5, 4, 3, 2, 1};
    
//     ubyte* result = permuteNumbers(keyRow, perm, rowLength);
//     result[0] = keyRow[0] ^ keyRow[6];

//     return result;
// }

ubyte* shiftRows(ubyte* input)
{
    int rowLength = 16;
    const int perm[] = {0, 1, 2, 3, 7, 4, 5, 6, 10, 11, 
        8, 9, 13, 14, 15, 12};
    ubyte* result = permuteNumbers(input, perm, rowLength);
    return result;
}

ubyte* mixColumns(ubyte* input)
{
    const int length = 16;
    const int sideLen = 4;
    ubyte* result = malloc(sizeof(ubyte) * length);
    ubyte m[] = {
        1, 0, 1, 1,
        1, 0, 0, 0,
        0, 1, 1, 0,
        1, 0, 1, 0
    };
    memset(result, 0, sizeof(ubyte) * length);

    ubyte* rTemp = result;
    for(int i = 0; i < sideLen; ++i)
    {
        for(int j = 0; j < sideLen; ++j)
        {
            for(int k = 0; k < sideLen; ++k)
            {
                *rTemp ^=  m[i * 4 + k] * input[j * 4 + k]; 
            }
            rTemp++;
        }
    }

    return result;
}