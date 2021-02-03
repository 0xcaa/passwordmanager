#include <stdio.h>
#include <unistd.h>

#include "encrypt.h"

#define PASS_FILE "test.txt"
#define TEMP "temp.txt"

#ifndef ENCRYPT
#define ENCRYPT 1
#endif

#ifndef DECRYPT
#define DECRYPT 0
#endif

int main(int argc, char **argv)
{
    int opt;
    FILE *FIN, *FOUT;

    
    /*check if there is a existing password file
      if there is not a password file is make a new one*/
   /* 
    if((access(PASS_FILE, F_OK))== -1){
        printf("Password file not found\nMaking new file\n");
        fp = fopen(PASS_FILE, "w");
        // prompt user for password to use for encryption key

        char password[30];
        printf("Enter new password: ");
        fgets(password, 30, stdin);
        printf("%s\n", password);
        f_crypt(ENCRYPT, password, fp);
           
    }
    else{
        fp = fopen(PASS_FILE, "r");
        //decrypt file
    }
    */

    unsigned char ckey[] = "thiskeyisverybad";
    unsigned char ivec[] = "dontusethisinput";


    while((opt = getopt(argc, argv, "hsnde")) != -1)
        switch(opt)
        {
            case 's':
                printf("show pass\n");
                break;
            case 'n':
                printf("new password entry\n");
                break;
            case 'h':
                printf("help\n");
                break;
         /* case 'd':
                printf("delete entry\n");
                break; */
            case 'd':
                FIN = fopen(PASS_FILE, "rb");
                FOUT = fopen(TEMP, "wb");
                f_crypt(DECRYPT, FIN, FOUT, ckey, ivec);
                fclose(FIN);
                fclose(FOUT);
                rename(TEMP, PASS_FILE);
                break;
            case 'e':
                FIN = fopen(PASS_FILE, "rb");
                FOUT = fopen(TEMP, "wb");
                f_crypt(ENCRYPT, FIN, FOUT, ckey, ivec);
                fclose(FIN);
                fclose(FOUT);
                rename(TEMP, PASS_FILE);
               break;
        default:
            printf("default\n");
            return 1;
        }

    return 0;
}
