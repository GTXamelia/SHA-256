// Header file
#include "sha256.h"

void sha256(FILE* msgf, HANDLE hConsole, WORD saved_attributes);

int nextmsgblock(FILE *f, union msgblock *M, enum status *S, uint64_t *nobits);

int main(int argc, char *argv[]) {

    // Windows console settings
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    // Save current attributes
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    // File parameter
    FILE* msgf;

    // User input char array
    char userInput[32];

    if (argv[1] == NULL){
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        printf("Please enter the name of the file you would like to hash: ");
        SetConsoleTextAttribute(hConsole, saved_attributes);
        scanf("%s", userInput);

        // Open file
        msgf = fopen(userInput, "r");
    } else {
        // Open file
        msgf = fopen(argv[1], "r");
    }

    if (msgf == NULL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("Error reading file. Please ensure the filename is correct! \n");
    }else {
        // Run the secure hash algorithm on the file
        sha256(msgf, hConsole, saved_attributes);

        // Close the file
        fclose(msgf);
    }

    // Restore original console settings before exit
    SetConsoleTextAttribute(hConsole, saved_attributes);

    return 0;
}// main function


void sha256(FILE* msgf, HANDLE hConsole, WORD saved_attributes) {

    // The current message block
    union msgblock M;
    
    // The number of bits read from the file
    uint64_t nobits = 0;

    // The status of the message blocks, in terms of padding
    enum status S = READ;

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

    // For looping
    int i = 0, t;
    int bytes;

    // Loop through message block as per page 22
    while(nextmsgblock(msgf, &M, &S, &nobits)) {

        i++;
    
        // From page 22, W[t] = M[t] for 0 <= t <= 15
        for(t = 0; t < 16; t++){
            W[t] = SWAP_UINT32(M.t[t]) ;
        };// for

        // from page 22, W[t] = ...
        for (t = 16; t < 64; t++){
            W[t] = SIG1(W[t - 2]) + W[t - 7] + SIG0(W[t - 15]) + W[t - 16];
        };// for

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
            T1 = h + sig1(e) + Ch(e, f, g) + K[t] + W[t];
            T2 = sig0(a) + Maj(a, b, c);
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

        bytes = 64 * i;
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("\n%d bytes read, File Hash:\n", bytes);

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    printf("[%08x%08x%08x%08x%08x%08x%08x%08x]\n\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);

}// sha256 function

int nextmsgblock(FILE *msgf, union msgblock *M, enum status *S, uint64_t *nobits) {

    // The number of bytes we get fread
    uint64_t nobytes;

    // For looping
    int i;

    // If we finished all the message block, then S should finish
    if (*S == FINISH) {
        return 0;
    }

    // Otherwise check if we need another blockfull of padding
    if (*S == PAD0 || *S == PAD1) {
        // Set first 56 bytes to all zero bytes
        for (i = 0; i < 56; i++) {
            M->e[i] = 0x80;
        }
           
        // Set the last 64 bits to the number of bits in the file (should be big-endian)
        M->s[7] = SWAP_UINT64(*nobits);

        // Tell S we are finished
        *S = FINISH;
        
        // If S was PAD1 then set the first bit of M to one
        if (*S == PAD1) 
            M->e[0] = 0x80;

        // Keep the loop in sha256 going for one more iteration
        return 1;
    }

    // If we get down here, we haven't finished reading the file
    nobytes = fread(M->e, 1, 64, msgf);

    // Keep track of the number of bytes we've read
    *nobits = *nobits + (nobytes * 8);

    // If we read less than 56 bytes, we can put all padding in this message block
    if (nobytes < 56) {
        // Add the one bit, as per the standard
        M->e[nobytes] = 0x80;

        // Add zero bits until the last 64
        while (nobytes < 56) {
            nobytes = nobytes + 1;
            M->e[nobytes] = 0x00;
        }

        // Append the file size in bits as a (should be big endian) unsigned 64 bit int
        M->s[7] = SWAP_UINT64(*nobits);

        // Tell S we are finished
        *S = FINISH;
    // Otherwise check if we can put some padding into this message block
    } else if (nobytes < 64) {
        // Tell S we need another message block with padding but no one bit
        *S = PAD0;
        // Put the one bit into the current block
        M->e[nobytes] = 0x80;
        // Pad the rest of the block with zero bits
        while (nobytes < 64) {
            nobytes = nobytes + 1;
            M->e[nobytes] = 0x80;
        }
    // Otherwise check if we're just at the end of the file
    } else if (feof(msgf)) {
        *S = PAD1;
    }

    // Continue while loop
    return 1;
}// nextmsgblock function