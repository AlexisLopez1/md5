#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TestingLen(char *msg, int len) {
    int i = 0;
    while (i < len)
    {
        printf("%.2x\n", msg[i]);
        i++;
    }
}

void md5(char *init_msg, int init_len)
{
    //Part of STEP 2. Ensure the message is not longer than 2^64
    if (init_len >= (0x2^64))
    {
        char *new_msg = NULL;
        for (int i = 0; i < 64; i++){
            new_msg[i] = init_msg[i];
        }
        
        init_msg = new_msg;
    }

    //STEP 1. Fill message wiht its text + N bits to complete 64 bits.
    int len = 64;
    char *msg = calloc(len, 1); //Add as much 0 as needed.
    memcpy(msg, init_msg, init_len); 
    msg[init_len] = 0x80; // Add 1 at the final text's position.
    
    //Code for STEP 1 testing.
    //TestingLen(msg, len);

    //STEP 2. Complete 512 bits.
    int bits = len * 8; 
    memcpy(msg + len, &bits, 4);
    
    //Code for STEP 2 testing.
    //TestingLen(msg, bits);

    free(msg); //This must be the last line.
}

int main(int argc, char *args[])
{
    char *msg = "Hello World";
    int len = strlen(msg);

    md5(msg, len);

    return 0;
}