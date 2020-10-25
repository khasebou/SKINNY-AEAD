#include <stdint.h>
#include "skinny.h"



/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k) {

}

ubyte* subCells(ubyte *x);
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
    const int permutationTwo[] = {7, 6, 5, 4, 3, 2, 1, 0};
    for(int i = 0; i < 3; ++i)
    {
        x = permuteBits(x, permutationOne);
        x = sboxLookup[x];
    }
    x = sboxLookup[x];
    x = permutationOne(x, permutationTwo);
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

ubyte* createNextTweakKeyRow(ubyte* keyRow)
{
    int rowLength = 8;
    const int perm[] = {0, 6, 7, 5, 4, 3, 2, 1};
    
    ubyte* result = permuteNumbers(keyRow, perm, rowLength);
    result[0] = keyRow[0] ^ keyRow[6];

    return result;
}

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