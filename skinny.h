/**
 * Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */
typedef unsigned char ubyte;



/** perm assumes x is organized (x7,..., x0)
 */
void subCells(ubyte *x);

ubyte* generateRoundKeys(const ubyte* tweakey);
void addTweakey(ubyte* state, ubyte* tweakey);
/**
 * @param input Input array to permute elements
 * @param perm array of indicies for permutation
 * @param length length of input and perm array
 * this func assumes element numbering as following 
 * (0,....,  length - 1)
 */
void permuteNumbers(ubyte* input, int *perm, int length);

void shiftRows(ubyte* input);

void mixColumns(ubyte* input);
/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k);
