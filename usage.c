#include <stdio.h>

void usage(char *name)
{
    printf("Usage:\n\t-n\tAdd new password entry\n");
    printf("\t-s\tShow password entry\n");
    printf("\t-d\tDelete password entry\n");

    printf("Example:\n%s -n phone 1234\n", name);
    printf("%s -s phone\n", name);
}
