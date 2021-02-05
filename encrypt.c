#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

#ifndef ENCRYPT
#define ENCRYPT 1
#endif
#ifndef DECRYPT
#define DECRYPT 0
#endif

#define PASS_FILE "test.txt"
#define TEMP_FILE "temp.txt"


/**
 * Encrypt or decrypt, depending on flag 'crypt_status'
 */
int f_crypt(int crypt_status, FILE *ifp, FILE *ofp, unsigned char *ckey, unsigned char *ivec)
{
    //get size of file
	fseek(ifp, 0L, SEEK_END);
	const unsigned BUFSIZE = ftell(ifp);
	fseek(ifp, 0L, SEEK_SET);

    unsigned char *read_buf = malloc(BUFSIZE);
    unsigned char *cipher_buf;
    unsigned blocksize;
    int outlen, inlen;

    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){
        fprintf(stderr, "Error: could not create EVP_CIPHER_CTX_new\n");
        return 0;
    }
    if(1 != EVP_CipherInit_ex(ctx, EVP_aes_256_cbc(), NULL, ckey, ivec, crypt_status)){
        fprintf(stderr, "Error: EVP_CipherInit_ex\n");
        return 0;
    }

    blocksize = EVP_CIPHER_CTX_block_size(ctx);
    cipher_buf = malloc(BUFSIZE + blocksize);

    while (1) {
        // Read in data in blocks until EOF. Update the ciphering with each read.

        inlen = fread(read_buf, sizeof(unsigned char), BUFSIZE, ifp);
        if(!EVP_CipherUpdate(ctx, cipher_buf, &outlen, read_buf, inlen)){
            EVP_CIPHER_CTX_free(ctx);
            free(cipher_buf);
            free(read_buf);
            fprintf(stderr, "Error: EVP_CipherUpdate\n");
            return 0;
        }
        fwrite(cipher_buf, sizeof(unsigned char), outlen, ofp);
        if (inlen < BUFSIZE) { // EOF
            break;
        }
    }
    // Now cipher the final block and write it out.
    if(!EVP_CipherFinal_ex(ctx, cipher_buf, &outlen)){
        EVP_CIPHER_CTX_free(ctx);
        free(cipher_buf);
        free(read_buf);
        fprintf(stderr, "Error: EVP_CipherFinal\n");
        return 0;
    }

    fwrite(cipher_buf, sizeof(unsigned char), outlen, ofp);

    // Free memory
    EVP_CIPHER_CTX_free(ctx);
    free(cipher_buf);
    free(read_buf);
    return 1;
}
int sha_pass(const char *password, unsigned char *hashed, unsigned int *shalen)
{
    EVP_MD_CTX *context;


	if((context = EVP_MD_CTX_new())==NULL){
        fprintf(stderr, "Error: EVP_MD_CTX_new\n");
        return 0;
    }
	if(1 != EVP_DigestInit_ex(context, EVP_sha256(), NULL)){
        fprintf(stderr, "Error: EVP_DigestInti_ex\n");
        return 0;
    }
	if(1 != EVP_DigestUpdate(context, password, strlen(password))){
        fprintf(stderr, "Error: EVP_DigestUpdate\n");
        return 0;
    }
	if(1 != EVP_DigestFinal_ex(context, hashed, shalen)){
        fprintf(stderr, "Error: EVP_DigestFinal_ex\n");
        return 0;
    }

	EVP_MD_CTX_free(context);
    return 1;
}
