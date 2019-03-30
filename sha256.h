// For using fixed bit length integers
#include <stdio.h>

// The usual input/output header file
#include <stdint.h>

// Win32 API
#include <windows.h>

#include<unistd.h>

#define uchar unsigned char
#define uint unsigned int

// See section 3.2 for definitions
#define rotr(a,b) (((a) >> (b)) | ((a) << (32-(b))))

// See section 4.1.2 for definitions 
#define SIG0(x) (rotr(x,2) ^ rotr(x,13) ^ rotr(x,22))
#define SIG1(x) (rotr(x,6) ^ rotr(x,11) ^ rotr(x,25))

// See section 4.1.2 for definitions 
#define sig0(x) (rotr(x,7) ^ rotr(x,18) ^ ((x) >> 3))
#define sig1(x) (rotr(x,17) ^ rotr(x,19) ^ ((x) >> 10))

// See section 4.1.2 for definitions 
#define Ch(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

// Used for fixing endiness
#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
#define SWAP_UINT64(x) \
( (((x) >> 56) & 0x00000000000000FF) | (((x) >> 40) & 0x000000000000FF00) | \
  (((x) >> 24) & 0x0000000000FF0000) | (((x) >>  8) & 0x00000000FF000000) | \
  (((x) <<  8) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) | \
  (((x) << 40) & 0x00FF000000000000) | (((x) << 56) & 0xFF00000000000000) )

// Represents a message block
union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

// A flag for where we are in the reading file
enum status {READ, PAD0, PAD1, FINISH};

// The K Constants defined in section 4.2.2
static  uint32_t K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};