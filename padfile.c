#include <stdio.h>
#include <stdint.h>

union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint_64_t s[8];
};

int main(int argc, char *argv[]) {

}