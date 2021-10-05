#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void md5(char *init_msg, int msg_len)
{
    //STEP 1. Fill message wiht its text + necessary bits to complete 64 bytes.
    int len = 64;
    char *msg = calloc(len, 1); //Add as much 0 as needed.
    memcpy(msg, init_msg, msg_len); 
    msg[msg_len] = 0x80; // Add 1 at the final text's position.

    //Code for STEP 1 testing.
    // int i = 0;
    // while (i < len)
    // {
    //     printf("%.2x", msg[i]);
    //     i++;
    // }

    free(msg); //This must be the last line.
}

int main(int argc, char *args[])
{
    char *msg = "Hello";
    int len = strlen(msg);

    md5(msg, len);

    return 0;
}