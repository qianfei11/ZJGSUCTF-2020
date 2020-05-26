/**********************************************************************\
* To commemorate the 1996 RSA Data Security Conference, the following  *
* code is released into the public domain by its author.  Prost!       *
*                                                                      *
* This cipher uses 16-bit words and little-endian byte ordering.       *
* I wonder which processor it was optimized for?                       *
*                                                                      *
* Thanks to CodeView, SoftIce, and D86 for helping bring this code to  *
* the public.                                                          *
\**********************************************************************/
#include <stdio.h>
#include <assert.h>
#include <string.h>

/**********************************************************************\
* Expand a variable-length user key (between 1 and 128 bytes) to a     *
* 64-short working rc2 key, of at most "bits" effective key bits.      *
* The effective key bits parameter looks like an export control hack.  *
* For normal use, it should always be set to 1024.  For convenience,   *
* zero is accepted as an alias for 1024.                               *
\**********************************************************************/

void keygen(unsigned short xkey[64],
            unsigned char *key,
            unsigned len,
            unsigned bits)
{
    unsigned char x;
    unsigned i;
    /* 256-entry permutation table, probably derived somehow from pi */
    static unsigned char PITABLE[256] = {0xD9, 0x78, 0xF9, 0xC4, 0x19, 0xDD, 0xB5, 0xED, 0x28, 0xE9, 0xFD, 0x79, 0x4A, 0xA0, 0xD8, 0x9D, 0xC6, 0x7E, 0x37, 0x83, 0x2B, 0x76, 0x53, 0x8E, 0x62, 0x4C, 0x64, 0x88, 0x44, 0x8B, 0xFB, 0xA2, 0x17, 0x9A, 0x59, 0xF5, 0x87, 0xB3, 0x4F, 0x13, 0x61, 0x45, 0x6D, 0x8D, 0x09, 0x81, 0x7D, 0x32, 0xBD, 0x8F, 0x40, 0xEB, 0x86, 0xB7, 0x7B, 0x0B, 0xF0, 0x95, 0x21, 0x22, 0x5C, 0x6B, 0x4E, 0x82, 0x54, 0xD6, 0x65, 0x93, 0xCE, 0x60, 0xB2, 0x1C, 0x73, 0x56, 0xC0, 0x14, 0xA7, 0x8C, 0xF1, 0xDC, 0x12, 0x75, 0xCA, 0x1F, 0x3B, 0xBE, 0xE4, 0xD1, 0x42, 0x3D, 0xD4, 0x30, 0xA3, 0x3C, 0xB6, 0x26, 0x6F, 0xBF, 0x0E, 0xDA, 0x46, 0x69, 0x07, 0x57, 0x27, 0xF2, 0x1D, 0x9B, 0xBC, 0x94, 0x43, 0x03, 0xF8, 0x11, 0xC7, 0xF6, 0x90, 0xEF, 0x3E, 0xE7, 0x06, 0xC3, 0xD5, 0x2F, 0xC8, 0x66, 0x1E, 0xD7, 0x08, 0xE8, 0xEA, 0xDE, 0x80, 0x52, 0xEE, 0xF7, 0x84, 0xAA, 0x72, 0xAC, 0x35, 0x4D, 0x6A, 0x2A, 0x96, 0x1A, 0xD2, 0x71, 0x5A, 0x15, 0x49, 0x74, 0x4B, 0x9F, 0xD0, 0x5E, 0x04, 0x18, 0xA4, 0xEC, 0xC2, 0xE0, 0x41, 0x6E, 0x0F, 0x51, 0xCB, 0xCC, 0x24, 0x91, 0xAF, 0x50, 0xA1, 0xF4, 0x70, 0x39, 0x99, 0x7C, 0x3A, 0x85, 0x23, 0xB8, 0xB4, 0x7A, 0xFC, 0x02, 0x36, 0x5B, 0x25, 0x55, 0x97, 0x31, 0x2D, 0x5D, 0xFA, 0x98, 0xE3, 0x8A, 0x92, 0xAE, 0x05, 0xDF, 0x29, 0x10, 0x67, 0x6C, 0xBA, 0xC9, 0xD3, 0x00, 0xE6, 0xCF, 0xE1, 0x9E, 0xA8, 0x2C, 0x63, 0x16, 0x01, 0x3F, 0x58, 0xE2, 0x89, 0xA9, 0x0D, 0x38, 0x34, 0x1B, 0xAB, 0x33, 0xFF, 0xB0, 0xBB, 0x48, 0x0C, 0x5F, 0xB9, 0xB1, 0xCD, 0x2E, 0xC5, 0xF3, 0xDB, 0x47, 0xE5, 0xA5, 0x9C, 0x77, 0x0A, 0xA6, 0x20, 0x68, 0xFE, 0x7F, 0xC1, 0xAD};

    assert(len > 0 && len <= 128);
    assert(bits <= 1024);
    if (!bits)
        bits = 1024;

    memcpy(xkey, key, len);

    /* Phase 1: Expand input key to 128 bytes */
    // for i = T, T+1, ..., 127 do
    //     L[i] = PITABLE[L[i-1] + L[i-T]];
    if (len < 128)
    {
        i = 0;
        x = ((unsigned char *)xkey)[len - 1];
        do
        {
            x = PITABLE[(x + ((unsigned char *)xkey)[i++]) & 255];
            ((unsigned char *)xkey)[len++] = x;
        } while (len < 128);
    }

    /* Phase 2 - reduce effective key size to "bits" */
    // L[128-T8] = PITABLE[L[128-T8] & TM];
    len = (bits + 7) >> 3; // bits = T1, len = T8, T8 = (T1+7)/8;
    i = 128 - len;
    x = PITABLE[((unsigned char *)xkey)[i] & (255 >> (7 & -bits))]; // (255 >> (7 & -bits) = TM, TM = 255 MOD 2^(8 + T1 - 8*T8);
    ((unsigned char *)xkey)[i] = x;

    // for i = 127-T8, ..., 0 do
    //     L[i] = PITABLE[L[i+1] XOR L[i+T8]];
    while (i--)
    {
        x = PITABLE[x ^ ((unsigned char *)xkey)[i + len]];
        ((unsigned char *)xkey)[i] = x;
    }

    /* Phase 3 - copy to xkey in little-endian order */
    i = 63;
    do
    {
        xkey[i] = ((unsigned char *)xkey)[2 * i] +
                  (((unsigned char *)xkey)[2 * i + 1] << 8);
    } while (i--);
}

/**********************************************************************\
* Encrypt an 8-byte block of plaintext using the given key.            *
\**********************************************************************/

void encrypt(unsigned short xkey[64],
             unsigned char *plain,
             unsigned char *cipher)
{
    // xkey = K, plain = R
    unsigned x76, x54, x32, x10, i;

    x76 = (plain[7] << 8) + plain[6];
    x54 = (plain[5] << 8) + plain[4];
    x32 = (plain[3] << 8) + plain[2];
    x10 = (plain[1] << 8) + plain[0];

    for (i = 0; i < 16; i++)
    {
        // R[i] = R[i] + K[j] + (R[i-1] & R[i-2]) + ((~R[i-1]) & R[i-3]);
        // j = j + 1;
        // R[i] = R[i] rol s[i];
        x10 += (x32 & ~x76) + (x54 & x76) + xkey[4 * i + 0];
        x10 = (x10 << 1) + (x10 >> 15 & 1);

        x32 += (x54 & ~x10) + (x76 & x10) + xkey[4 * i + 1];
        x32 = (x32 << 2) + (x32 >> 14 & 3);

        x54 += (x76 & ~x32) + (x10 & x32) + xkey[4 * i + 2];
        x54 = (x54 << 3) + (x54 >> 13 & 7);

        x76 += (x10 & ~x54) + (x32 & x54) + xkey[4 * i + 3];
        x76 = (x76 << 5) + (x76 >> 11 & 31);

        // R[i] = R[i] + K[R[i-1] & 63];
        if (i == 4 || i == 10)
        {
            x10 += xkey[x76 & 63];
            x32 += xkey[x10 & 63];
            x54 += xkey[x32 & 63];
            x76 += xkey[x54 & 63];
        }
    }

    cipher[0] = (unsigned char)x10;
    cipher[1] = (unsigned char)(x10 >> 8);
    cipher[2] = (unsigned char)x32;
    cipher[3] = (unsigned char)(x32 >> 8);
    cipher[4] = (unsigned char)x54;
    cipher[5] = (unsigned char)(x54 >> 8);
    cipher[6] = (unsigned char)x76;
    cipher[7] = (unsigned char)(x76 >> 8);
}

char *logo = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[1m";

void welcome() {
    puts(logo);
}

int main()
{
    unsigned bits = 64;
    unsigned short xkey[64];
    unsigned char key[8] = {104, 97, 108, 105, 108, 117, 121, 97}; // "haliluya";
    // unsigned char *flag = "zjgsuctf{1t_533m5_th4t_y0u_h4v3_41r34dY_m3t_rC2}";
    unsigned char flag[48];
    welcome();
    printf("Please input the flag:\n");
    scanf("%s", flag);
    int length = strlen(flag);
    int remain = length % 8;
    if (remain != 0)
    {
        printf("Error!\n");
        return -1;
    }
    keygen(xkey, key, 8, bits);
    unsigned char enc[length + 1];
    for (int i = 0; i < length; i += 8)
    {
        encrypt(xkey, &flag[i], &enc[i]);
    }
    enc[length] = '\x00';
    unsigned char data[48] = {20, 180, 57, 50, 17, 164, 240, 38, 110, 45, 59, 122, 186, 229, 114, 109, 246, 93, 201, 140, 108, 36, 16, 150, 114, 86, 247, 126, 89, 69, 210, 56, 231, 106, 211, 66, 71, 128, 38, 26, 144, 190, 253, 20, 191, 28, 52, 221};
    for (int i = 0; i < length; i++)
    {
        if (enc[i] != data[i])
        {
            printf("Error!\n");
            return -1;
        }
    }
    printf("Congratulations!\n");
    return 0;
}
