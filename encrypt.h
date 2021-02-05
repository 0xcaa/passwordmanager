int f_crypt(int crypt_status, FILE *ifp, FILE *ofp, unsigned char *ckey, unsigned char *ivec);
int sha_pass(const char *password, unsigned char *hashed, unsigned int *shalen);
