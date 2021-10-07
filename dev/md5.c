#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

//Methods declaration
char ValidateFileSize(char *file_name); //Validate if the file < 256 bytes
void ReadFile(char *file_name);         //Read txt file.
void md5(char *init_msg, int init_len); //Main function.
void TestingLen(char *msg, int len);    //Only for testing purposes.

//Global definitions
#define uint unsigned int
#define F(x, y, z) ((x & y) | (~x & z))
#define G(x, y, z) ((x & z) | (y & ~z))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))

#define FF(a, b, c, d, m, s, t)  \
    {                            \
        a += F(b, c, d) + m + t; \
        a = b + ROTLEFT(a, s);   \
    }
#define GG(a, b, c, d, m, s, t)  \
    {                            \
        a += G(b, c, d) + m + t; \
        a = b + ROTLEFT(a, s);   \
    }
#define HH(a, b, c, d, m, s, t)  \
    {                            \
        a += H(b, c, d) + m + t; \
        a = b + ROTLEFT(a, s);   \
    }
#define II(a, b, c, d, m, s, t)  \
    {                            \
        a += I(b, c, d) + m + t; \
        a = b + ROTLEFT(a, s);   \
    }

//Global variables declaration.
char text[256];
int flag = 0;
//STEP 3.
uint A = 0x67452301;
uint B = 0xefcdab89;
uint C = 0x98badcfe;
uint D = 0x10325476;

int main(int argc, char **args)
{
    char *msg = "Hello";
    int len = 0;
    
    ReadFile(args[1]);
    msg = text;
    len = strlen(msg);

    if (msg != 0)
    {
       md5(msg, len);
    }

    return 0;
}

void ReadFile(char *file_name)
{

    if (ValidateFileSize(file_name) == 1)
    {
        int ch;
        FILE *file = fopen(file_name, "r");

        int i = 0;
        while ((ch = fgetc(file)) != EOF)
        {
            text[i] = ch;
            i++;
        }

        fclose(file);
        flag = 1;
    }
    else
    {
        flag = 0;
        printf("\nError al intentar leer el archivo.");
    }
}

char ValidateFileSize(char *file_name)
{
    struct stat st;
    int size = 0;
    stat(file_name, &st);
    size = st.st_size;

    if (size < 256)
    {
        return 1;
    }
    else
    {
        printf("The file size can't be less than 256");
        return 0;
    }
}

void md5(char *init_msg, int init_len)
{
    //Part of STEP 2. Ensure the message is not longer than 2^64
    if (init_len >= (0x2 ^ 64))
    {
        char *new_msg = NULL;
        for (int i = 0; i < 64; i++)
        {
            new_msg[i] = init_msg[i];
        }

        init_msg = new_msg;
    }

    //STEP 1-2. Fill message wiht its text + N bits to complete 64 bits.
    int len = 64;
    char *msg = calloc(len, 1); //Add as much 0 as needed.
    memcpy(msg, init_msg, init_len);
    msg[init_len] = 0x80; // Add 1 at the final text's position.

    //Code for STEP 1-2 testing.
    //TestingLen(msg, len);

    //STEP 4. Process message.
    for (int x = 0; x < 3; x++)
    {
        int a, b, c, d, m[16], i, j;

        //COMMENTS FROM RFC1321: MD5 specifies big endian byte order, but this implementation assumes a little
        // endian byte order CPU. Reverse all the bytes upon input, and re-reverse them
        // on output (in md5_final()).
        for (i = 0, j = 0; i < 16; ++i, j += 4)
        {
            m[i] = (msg[j]) + (msg[j + 1] << 8) + (msg[j + 2] << 16) + (msg[j + 3] << 24);
        }

        //Buffer initialization. STEP 3.
        a = A;
        b = B;
        c = C;
        d = D;

        //Create 16 bits blocks.
        FF(a, b, c, d, m[0], 7, 0xd76aa478);
        FF(d, a, b, c, m[1], 12, 0xe8c7b756);
        FF(c, d, a, b, m[2], 17, 0x242070db);
        FF(b, c, d, a, m[3], 22, 0xc1bdceee);
        FF(a, b, c, d, m[4], 7, 0xf57c0faf);
        FF(d, a, b, c, m[5], 12, 0x4787c62a);
        FF(c, d, a, b, m[6], 17, 0xa8304613);
        FF(b, c, d, a, m[7], 22, 0xfd469501);
        FF(a, b, c, d, m[8], 7, 0x698098d8);
        FF(d, a, b, c, m[9], 12, 0x8b44f7af);
        FF(c, d, a, b, m[10], 17, 0xffff5bb1);
        FF(b, c, d, a, m[11], 22, 0x895cd7be);
        FF(a, b, c, d, m[12], 7, 0x6b901122);
        FF(d, a, b, c, m[13], 12, 0xfd987193);
        FF(c, d, a, b, m[14], 17, 0xa679438e);
        FF(b, c, d, a, m[15], 22, 0x49b40821);

        GG(a, b, c, d, m[1], 5, 0xf61e2562);
        GG(d, a, b, c, m[6], 9, 0xc040b340);
        GG(c, d, a, b, m[11], 14, 0x265e5a51);
        GG(b, c, d, a, m[0], 20, 0xe9b6c7aa);
        GG(a, b, c, d, m[5], 5, 0xd62f105d);
        GG(d, a, b, c, m[10], 9, 0x02441453);
        GG(c, d, a, b, m[15], 14, 0xd8a1e681);
        GG(b, c, d, a, m[4], 20, 0xe7d3fbc8);
        GG(a, b, c, d, m[9], 5, 0x21e1cde6);
        GG(d, a, b, c, m[14], 9, 0xc33707d6);
        GG(c, d, a, b, m[3], 14, 0xf4d50d87);
        GG(b, c, d, a, m[8], 20, 0x455a14ed);
        GG(a, b, c, d, m[13], 5, 0xa9e3e905);
        GG(d, a, b, c, m[2], 9, 0xfcefa3f8);
        GG(c, d, a, b, m[7], 14, 0x676f02d9);
        GG(b, c, d, a, m[12], 20, 0x8d2a4c8a);

        HH(a, b, c, d, m[5], 4, 0xfffa3942);
        HH(d, a, b, c, m[8], 11, 0x8771f681);
        HH(c, d, a, b, m[11], 16, 0x6d9d6122);
        HH(b, c, d, a, m[14], 23, 0xfde5380c);
        HH(a, b, c, d, m[1], 4, 0xa4beea44);
        HH(d, a, b, c, m[4], 11, 0x4bdecfa9);
        HH(c, d, a, b, m[7], 16, 0xf6bb4b60);
        HH(b, c, d, a, m[10], 23, 0xbebfbc70);
        HH(a, b, c, d, m[13], 4, 0x289b7ec6);
        HH(d, a, b, c, m[0], 11, 0xeaa127fa);
        HH(c, d, a, b, m[3], 16, 0xd4ef3085);
        HH(b, c, d, a, m[6], 23, 0x04881d05);
        HH(a, b, c, d, m[9], 4, 0xd9d4d039);
        HH(d, a, b, c, m[12], 11, 0xe6db99e5);
        HH(c, d, a, b, m[15], 16, 0x1fa27cf8);
        HH(b, c, d, a, m[2], 23, 0xc4ac5665);

        II(a, b, c, d, m[0], 6, 0xf4292244);
        II(d, a, b, c, m[7], 10, 0x432aff97);
        II(c, d, a, b, m[14], 15, 0xab9423a7);
        II(b, c, d, a, m[5], 21, 0xfc93a039);
        II(a, b, c, d, m[12], 6, 0x655b59c3);
        II(d, a, b, c, m[3], 10, 0x8f0ccc92);
        II(c, d, a, b, m[10], 15, 0xffeff47d);
        II(b, c, d, a, m[1], 21, 0x85845dd1);
        II(a, b, c, d, m[8], 6, 0x6fa87e4f);
        II(d, a, b, c, m[15], 10, 0xfe2ce6e0);
        II(c, d, a, b, m[6], 15, 0xa3014314);
        II(b, c, d, a, m[13], 21, 0x4e0811a1);
        II(a, b, c, d, m[4], 6, 0xf7537e82);
        II(d, a, b, c, m[11], 10, 0xbd3af235);
        II(c, d, a, b, m[2], 15, 0x2ad7d2bb);
        II(b, c, d, a, m[9], 21, 0xeb86d391);

        //Saving init value + new value.
        A += a;
        B += b;
        C += c;
        D += d;
    }

    free(msg); //Free memory.

    //STEP 5. Print result
    uint8_t *p;

    p = (uint8_t *)&A;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], A);

    p = (uint8_t *)&B;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], B);

    p = (uint8_t *)&C;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], C);

    p = (uint8_t *)&D;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], D);
    printf("\n");
}

void TestingLen(char *msg, int len)
{
    int i = 0;
    while (i < len)
    {
        printf("%2.2x\n", msg[i]);
        i++;
    }
}