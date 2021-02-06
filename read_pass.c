#include <stdio.h>


#define PASS_FILE "test.txt"

int show_pass(char *name)
{
    //look for name keyword in whole file and print everything untill 3 #
    printf("%s\n", name);



    return 0;
}


int add_pass(char *desc, char *creds)
{
    //check for duplicat descriptions befor apending
    //3 # to identify a new password entry

    FILE *fp;

    fp = fopen(PASS_FILE, "a");

    fprintf(fp, "###%s\n%s\n", desc, creds);

    return 1;
}
