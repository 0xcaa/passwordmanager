#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#define PASS_FILE "test.txt"
#define TEMP_FILE "temp.txt"


/**
 * Encrypt or decrypt, depending on flag 'should_encrypt'
 */
void f_crypt(int crypt_status, FILE *ifp, FILE *ofp, unsigned char *ckey, unsigned char *ivec) {
/*
    unsigned char ckey[] = "thiskeyisverybad";
    unsigned char ivec[] = "dontusethisinput";
    FILE *ofp;
    ofp = fopen(TEMP_FILE, "wb");
    */





    const unsigned BUFSIZE=4096;
    unsigned char *read_buf = malloc(BUFSIZE);
    unsigned char *cipher_buf;
    unsigned blocksize;
    int out_len;
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    EVP_CipherInit(ctx, EVP_aes_256_cbc(), ckey, ivec, crypt_status);
    blocksize = EVP_CIPHER_CTX_block_size(ctx);
    cipher_buf = malloc(BUFSIZE + blocksize);

    while (1) {

        // Read in data in blocks until EOF. Update the ciphering with each read.

        int numRead = fread(read_buf, sizeof(unsigned char), BUFSIZE, ifp);
        EVP_CipherUpdate(ctx, cipher_buf, &out_len, read_buf, numRead);
        fwrite(cipher_buf, sizeof(unsigned char), out_len, ofp); //ofp
        if (numRead < BUFSIZE) { // EOF
            break;
        }
    }

    // Now cipher the final block and write it out.

    EVP_CipherFinal(ctx, cipher_buf, &out_len);
    fwrite(cipher_buf, sizeof(unsigned char), out_len, ofp); //ofp

    // Free memory

    free(cipher_buf);
    free(read_buf);


//    rename(TEMP_FILE, PASS_FILE);
}

