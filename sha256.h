// For using fixed bit length integers
#include <stdio.h>

// The usual input/output header file
#include <stdint.h>

// Win32 API
#include <windows.h>

// See section 3.2 for definitions
#define rotr(x, n) (((x) >> n) | ((x) << (32 - n)))
#define shr(x, n) (x >> n)

// See section 4.1.2 for definitions 
#define sig0(x) (rotr(7, x) ^ rotr(18, x) ^ shr(3, x))
#define sig1(x) (rotr(17, x) ^ rotr(19, x) ^ shr(10, x))

// See section 4.1.2 for definitions 
#define SIG0(x) (rotr(2, x) ^ rotr(13, x) ^ rotr(22, x))
#define SIG1(x) (rotr(6, x) ^ rotr(11, x) ^ rotr(25, x))

// See section 4.1.2 for definitions 
#define Ch(x, y, z) ((x & y) ^ ((!x) & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

// Represents a message block
union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

// A flag for where we are in the reading file
enum status {READ, PAD0, PAD1, FINISH};