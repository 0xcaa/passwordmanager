#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>

#include "main.h"
#include "encrypt.h"
#include "read_pass.h"


/*
   run programm like this:
   passwordmanager -n phone 1234    to add phone password entry
   passwordmanager -s phone         to show phone password

   then it asks for password to ensure you are allowed to run the program
   the programm needs the password everytime to decrypt the file
   */

int main(int argc, char **argv)
{
    int opt, i;
    FILE *FIN, *FOUT;
    unsigned int shalen;
    unsigned char ckey[HASH_LEN];
    unsigned char ivec[HASH_LEN/4];
    //char pass[31];
    //char pass_verifier[31];
    char p[PASS_SIZE];

    const char pass[] = "thiskeyisverybad";

    if(!(get_password(p))){
        fprintf(stderr, "could not read password\n");
        return EXIT_FAILURE;
    }
    printf("%s\n", p);
    
    /*check if there is a existing password file
      if there is not a password file is make a new one*/

    /*
    if((access(PASS_FILE, F_OK))== -1){
        printf("Password file not found\nMaking new file\n");
        FIN = fopen(PASS_FILE, "wb");
        fclose(FIN);

        // prompt user for password to use for encryption key
        printf("Enter new password: ");
        if(fgets(pass, 30, stdin)==NULL){
            fprintf(stderr, "Error: Password cannot be 0 and cant exeed 30 characters\n");
            return -1;
        }
        if(fgets(pass_verifier, 30, stdin)==NULL && strcmp(pass, pass_verifier)!='\0'){
            fprintf(stderr, "Error: Passwords does not match\n");
            return -1;
        }
        sha_pass(pass, ckey, &shalen);
        //fill ivec with first 16 bytes of password hash
        for(i=0;i<shalen/2;i++)
            ivec[i] = ckey[i];

        FIN = fopen(PASS_FILE, "rb");
        FOUT = fopen(TEMP, "wb");
        f_crypt(ENCRYPT, FIN, FOUT, ckey, ivec);
        fclose(FIN);
        fclose(FOUT);
        rename(TEMP, PASS_FILE);
    }
    else{
        printf("Enter password: ");
        if(fgets(pass, 30, stdin)==NULL){
            fprintf(stderr, "Error: Password cannot be 0 and cant exeed 30 characters\n");
            return -1;
        }
    }

    FIN = fopen(PASS_FILE, "rb");
    FOUT = fopen(TEMP, "wb");
    f_crypt(DECRYPT, FIN, FOUT, ckey, ivec);
    fclose(FIN);
    fclose(FOUT);
    rename(TEMP, PASS_FILE);

    return 0;
    */

    sha_pass(pass, ckey, &shalen);
    //fill ivec with first 16 bytes of password hash
    for(i=0;i<shalen/2;i++)
        ivec[i] = ckey[i];

    while((opt = getopt(argc, argv, "hsndqe")) != -1)
        switch(opt)
        {
            case 's':
                FIN = fopen(PASS_FILE, "rb");
                if(! (show_pass(FIN, argv[2])))
                    fprintf(stderr, "Password entry not found\n");
                fclose(FIN);
                break;
            case 'n':
                FIN = fopen(PASS_FILE, "rb");
                if(! (add_pass(FIN, argv[2], argv[3]))){
                    fprintf(stderr, "Entry %s already exists\n", argv[2]);
                    fclose(FIN);
                    exit(EXIT_FAILURE);
                }

                fclose(FIN);
                break;
            case 'h':
                printf("help\n");
                break;
            case 'q':
                FIN = fopen(PASS_FILE, "rb");
                if(! (delete_pass(FIN, argv[2]))){
                    fprintf(stderr, "Error: entry %s does not exist\n", argv[2]);
                    //fclose(FIN);
                    exit(EXIT_FAILURE);
                }
                else
                    fprintf(stdout, "Deleted %s\n", argv[2]);
               break;
            case 'd':
                FIN = fopen(PASS_FILE, "rb");
                FOUT = fopen(TEMP_FILE, "wb");
                f_crypt(DECRYPT, FIN, FOUT, ckey, ivec);
                fclose(FIN);
                fclose(FOUT);
                rename(TEMP_FILE, PASS_FILE);
                break;
            case 'e':
                FIN = fopen(PASS_FILE, "rb");
                FOUT = fopen(TEMP_FILE, "wb");
                f_crypt(ENCRYPT, FIN, FOUT, ckey, ivec);
                fclose(FIN);
                fclose(FOUT);
                rename(TEMP_FILE, PASS_FILE);
               break;
        default:
            printf("default\n");
            return 1;
        }

    return 0;
}


int get_password(char *password)
{
    struct termios oflags, nflags;

    // disable echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if(tcsetattr(fileno(stdin), TCSANOW, &nflags) !=0){
        perror("tcsetattr");
        return 0;
    }

    printf("Password: ");
    fgets(password, PASS_SIZE, stdin);
    password[strlen(password)-1] = 0;

    // restore terminal
    if(tcsetattr(fileno(stdin), TCSANOW, &oflags) !=0){
        perror("tcsetattr");
        return 0;
    }

    return 1;
}
