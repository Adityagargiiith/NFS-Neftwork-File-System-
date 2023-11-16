#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

int main()
{
    char *path = "/command";
    char *name = "./command/abc";

    int p = mkdir(name, 0777);
    printf("%d\n", p);
    if (p == -1)
    {
        printf("Error\n");
    }
    else
    {
        printf("Success\n");
    }
    return 0;
}