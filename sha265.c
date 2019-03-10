// The usual input/output header file
#include <stdio.h>
// For using fixed bit length integers
#include <stdint.h>

void sha256();

// See section 4.1.2 for definitions 
uint32_t sig0(uint_t x);
uint32_t sig1(uint_t x);

// See section 3.2 for definitions
uint_t rotr(uint_t n, uint_t x);
uint_t shr(uint_t n, uint_t x);

// See section 4.1.2 for definitions 
uint_t SIG0(uint_t x);
uint_t SIG1(uint_t x);

// See section 4.1.2 for definitions 
uint_t Ch(uint_t x, uint_t y, uint_t z);
uint_t Maj(uint_t x, uint_t y, uint_t z);

int main(int argc, char *argv[]) {
    
    sha256();

    return 0;
}// main function


void sha256() {
    // Message schedule (section 6.2)
    uint32_t W[64];
    // Working variables (section 6.2))
    uint32_t a, b, c, d, e, f, g, h;
    // Temp variables (section 6.2)
    uint32_t T1, T2;
    
    // The hash value (section 6.2)
    // The values come from section 5.3.3
    uint32_t H[8] = {
        0x6a09e667, 
        0xbb67ae85, 
        0x3c6ef372, 
        0xa54ff53a,
        0x510e527f, 
        0x9b05688c, 
        0x1f83d9ab,
        0x5be0cd19
    };

    // The current message block
    uint32_t M[16];

    // For looping
    int t;
    
    // From page 22, W[t] = M[t] for 0 <= t <= 15
    for(t = 0; t < 16; t++){
        W[t] = M[t];
    }// for

    // from page 22, W[t] = ...
    for (t = 16; t < 64; t++){
        W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
    }// for

    // Initialize a, b, c, d, e, f, g, h as per step 2, page 22 
    a = H[0]; 
    b = H[1]; 
    c = H[2]; 
    d = H[3]; 
    e = H[4]; 
    f = H[5];
    g = H[6];
    h = H[7];

    // Step 3
    for(t = 0; t < 64; t++){
        T1 = h + SIG1(e) + Ch(e, f, g) + K[t] + W[t];
        T2 = SIG0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    //  Step 4.
    H[0] = a + H[0];
    H[1] = b + H[1];
    H[2] = c + H[2];
    H[3] = d + H[3];
    H[4] = e + H[4];
    H[5] = f + H[5];
    H[6] = g + H[6];
    H[7] = h + H[7];


}// sha256 function

// See section 3.2 and 4.1.2 for definitions
uint_t rotr(uint_t n, uint_t x){
    return (x >> n) | (x << (32 - n));
}// rotr function

// See section 3.2 and 4.1.2 for definitions
uint_t shr(uint_t n, uint_t x){
    return (x >> n);
}// shr function

// See section 3.2 and 4.1.2 for definitions
uint32_t sig0(uint_t x){
    return (rotr(7, x) ^ rotr(18, x) ^ shr(3, x));
}// sig0 function

// See section 3.2 and 4.1.2 for definitions
uint32_t sig1(uint_t x){
    return (rotr(17, x) ^ rotr(19, x) ^ shr(10, x));
}// sig1 function

uint_t SIG0(uint_t x){
    return (rotr(2, x) ^ rotr(13, x) ^ rotr(22, x));
}// SIG0 function

uint_t SIG1(uint_t x){
    return (rotr(6, x) ^ rotr(11, x) ^ rotr(25, x));
}// SIG1 function

uint_t Ch(uint_t x, uint_t y, uint_t z){
    return ((x & y) ^ ((!x) & z));
}// Ch function

uint_t Maj(uint_t x, uint_t y, uint_t z){
    return ((x & y) ^ (x & z) ^ (y & z));
}// Maj function