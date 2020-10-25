/**
 * Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */
typedef unsigned char ubyte;

const ubyte sboxLookupTable[256] = {
0 x65 ,0 x4c ,0 x6a ,0 x42 ,0 x4b ,0 x63 ,0 x43 ,0 x6b ,0 x55 ,0 x75 ,0 x5a ,0 x7a ,0 x53 ,0 x73 ,0 x5b ,0 x7b ,
0 x35 ,0 x8c ,0 x3a ,0 x81 ,0 x89 ,0 x33 ,0 x80 ,0 x3b ,0 x95 ,0 x25 ,0 x98 ,0 x2a ,0 x90 ,0 x23 ,0 x99 ,0 x2b ,
0 xe5 ,0 xcc ,0 xe8 ,0 xc1 ,0 xc9 ,0 xe0 ,0 xc0 ,0 xe9 ,0 xd5 ,0 xf5 ,0 xd8 ,0 xf8 ,0 xd0 ,0 xf0 ,0 xd9 ,0 xf9 ,
0 xa5 ,0 x1c ,0 xa8 ,0 x12 ,0 x1b ,0 xa0 ,0 x13 ,0 xa9 ,0 x05 ,0 xb5 ,0 x0a ,0 xb8 ,0 x03 ,0 xb0 ,0 x0b ,0 xb9 ,
0 x32 ,0 x88 ,0 x3c ,0 x85 ,0 x8d ,0 x34 ,0 x84 ,0 x3d ,0 x91 ,0 x22 ,0 x9c ,0 x2c ,0 x94 ,0 x24 ,0 x9d ,0 x2d ,
0 x62 ,0 x4a ,0 x6c ,0 x45 ,0 x4d ,0 x64 ,0 x44 ,0 x6d ,0 x52 ,0 x72 ,0 x5c ,0 x7c ,0 x54 ,0 x74 ,0 x5d ,0 x7d ,
0 xa1 ,0 x1a ,0 xac ,0 x15 ,0 x1d ,0 xa4 ,0 x14 ,0 xad ,0 x02 ,0 xb1 ,0 x0c ,0 xbc ,0 x04 ,0 xb4 ,0 x0d ,0 xbd ,
0 xe1 ,0 xc8 ,0 xec ,0 xc5 ,0 xcd ,0 xe4 ,0 xc4 ,0 xed ,0 xd1 ,0 xf1 ,0 xdc ,0 xfc ,0 xd4 ,0 xf4 ,0 xdd ,0 xfd ,
0 x36 ,0 x8e ,0 x38 ,0 x82 ,0 x8b ,0 x30 ,0 x83 ,0 x39 ,0 x96 ,0 x26 ,0 x9a ,0 x28 ,0 x93 ,0 x20 ,0 x9b ,0 x29 ,
0 x66 ,0 x4e ,0 x68 ,0 x41 ,0 x49 ,0 x60 ,0 x40 ,0 x69 ,0 x56 ,0 x76 ,0 x58 ,0 x78 ,0 x50 ,0 x70 ,0 x59 ,0 x79 ,
0 xa6 ,0 x1e ,0 xaa ,0 x11 ,0 x19 ,0 xa3 ,0 x10 ,0 xab ,0 x06 ,0 xb6 ,0 x08 ,0 xba ,0 x00 ,0 xb3 ,0 x09 ,0 xbb ,
0 xe6 ,0 xce ,0 xea ,0 xc2 ,0 xcb ,0 xe3 ,0 xc3 ,0 xeb ,0 xd6 ,0 xf6 ,0 xda ,0 xfa ,0 xd3 ,0 xf3 ,0 xdb ,0 xfb ,
0 x31 ,0 x8a ,0 x3e ,0 x86 ,0 x8f ,0 x37 ,0 x87 ,0 x3f ,0 x92 ,0 x21 ,0 x9e ,0 x2e ,0 x97 ,0 x27 ,0 x9f ,0 x2f ,
0 x61 ,0 x48 ,0 x6e ,0 x46 ,0 x4f ,0 x67 ,0 x47 ,0 x6f ,0 x51 ,0 x71 ,0 x5e ,0 x7e ,0 x57 ,0 x77 ,0 x5f ,0 x7f ,
0 xa2 ,0 x18 ,0 xae ,0 x16 ,0 x1f ,0 xa7 ,0 x17 ,0 xaf ,0 x01 ,0 xb2 ,0 x0e ,0 xbe ,0 x07 ,0 xb7 ,0 x0f ,0 xbf ,
0 xe2 ,0 xca ,0 xee ,0 xc6 ,0 xcf ,0 xe7 ,0 xc7 ,0 xef ,0 xd2 ,0 xf2 ,0 xde ,0 xfe ,0 xd7 ,0 xf7 ,0 xdf ,0 xff
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

/** perm assumes x is organized (x7,..., x0)
 */
ubyte permuteBits(ubyte x, int *perm);
ubyte* subCells(ubyte *x);
ubyte subCell(ubyte x);

typedef struct skinny
{
    int c0, c1, c2;
} RoundConstants;

ubyte* AddConstants(ubyte *x, int roundNumber);
RoundConstants generateLFSRConstants(int roundNumber);

ubyte* AddRoundTweakey(ubyte* input, ubyte* tweakey);
/**
 * @param input Input array to permute elements
 * @param perm array of indicies for permutation
 * @param length length of input and perm array
 * this func assumes element numbering as following 
 * (0,....,  length - 1)
 */
ubyte* permuteNumbers(ubyte* input, int *perm, int length);

ubyte* createNextTweakKeyRow(ubyte* key);

/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k);
