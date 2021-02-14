#include <stdio.h>
#include <string.h>

#include "main.h"

int show_pass(FILE *fp, char *name)
{
    char id[] = "###";
    char line[MAX_LEN], line2[MAX_LEN];
    int found = 0;

    while(fgets(line, sizeof line, fp)!= NULL)
        if(NULL != strstr(line, name) && NULL != strstr(line, id)){
            fprintf(stdout,"%s%s", line, fgets(line2, sizeof line2, fp));
            found = 1;
        }

    return found;
}


int add_pass(FILE *fp, char *desc, char *creds)
{
    char temp[sizeof desc+4] = "###";
    char line[MAX_LEN];
    int i, x=0;
    char *pos;
    for(i=3;i<sizeof desc+3;i++){
        temp[i] = desc[x];
        x++;
    }

    //check for duplicat descriptions befor appending
    while(fgets(line, sizeof line, fp)!= NULL){
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

int delete_pass(FILE *fp, char *desc)
{
    char temp[MAX_LEN] = "###", line[MAX_LEN];
    int i, x=0, found = 0;
    char *pos;
    FILE *tfp;

    tfp = fopen(TEMP_FILE, "wb");

    for(i=3;i<sizeof temp;i++){
        temp[i] = desc[x];
        x++;
    }

    while(fgets(line, sizeof line, fp)!= NULL){
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';
        if('\0' ==(strcmp(line, temp))){
            found = 1;
            fgets(line, sizeof line, fp);
        }
        else
            fprintf(tfp, "%s\n", line);

    }
    fclose(tfp);
    fclose(fp);
    remove(PASS_FILE);
    rename(TEMP_FILE, PASS_FILE);

    return found;
}

int show_all(FILE *fp)
{

    //function to dump all contents of file
    printf("all\n");
    return 1;



}
