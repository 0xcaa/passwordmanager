#include <stdio.h>
#include <string.h>

#include "main.h"

int show_pass(FILE *fp, char *name)
{
    char id[] = "###";
    char line[250];
    int found = 0;

    while(fgets(line, sizeof line, fp)!= NULL)
        if(NULL != strstr(line, name) && NULL != strstr(line, id)){
            fprintf(stdout, "%s", line);
            fgets(line, sizeof line, fp);
            fprintf(stdout, "%s", line);
            found = 1;
        }

    return found;
}


int add_pass(FILE *fp, char *desc, char *creds)
{
    char temp[sizeof desc+4] = "###";
    char line[250];
    int i, x=0;
    for(i=3;i<sizeof desc+3;i++){
        temp[i] = desc[x];
        x++;
    }

    //check for duplicat descriptions befor apending
    //3 # to identify a new password entry
    while(fgets(line, sizeof line, fp)!= NULL){
        char *pos;
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';
        if('\0' ==(strcmp(line, temp)))
            return 0;
    }
    fclose(fp);

    fp = fopen(PASS_FILE, "ab");
    fprintf(fp, "###%s\n%s\n", desc, creds);

    return 1;
}
