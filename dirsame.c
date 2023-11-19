#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

void copyfcn(char *src, char *dest)
{
    DIR *dir = opendir(src);
    if (dir == NULL)
    {
        perror("Error in opendir() function call: ");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char *new_src = (char *)malloc(100 * sizeof(char));
        memset(new_src, 0, 100);
        char *new_dest = (char *)malloc(100 * sizeof(char));
        memset(new_dest, 0, 100);
        strcpy(new_src, src);
        strcat(new_src, "/");
        strcat(new_src, entry->d_name);
        new_src[strlen(new_src)] = '\0';
        strcpy(new_dest, dest);
        strcat(new_dest, "/");
        strcat(new_dest, entry->d_name);
        new_dest[strlen(new_dest)] = '\0';

        if (entry->d_type == DT_DIR)
        {
            int status = mkdir(new_dest, 0777);
            if (status == -1)
            {
                perror("Error in mkdir() function call: ");
                return;
            }
            copyfcn(new_src, new_dest);
        }
        else
        {
            FILE *fp1 = fopen(new_src, "r");
            if (fp1 == NULL)
            {
                perror("Error in fopen() function call: ");
                return;
            }

            FILE *fp2 = fopen(new_dest, "w");
            if (fp2 == NULL)
            {
                perror("Error in fopen() function call: ");
                return;
            }

            char ch;
            while ((ch = fgetc(fp1)) != EOF)
                fputc(ch, fp2);

            fclose(fp1);
            fclose(fp2);
        }
    }
}

int main()
{
    char *src = (char *)malloc(100 * sizeof(char));
    char *dest = (char *)malloc(100 * sizeof(char));
    printf("Enter source directory: ");
    scanf("%s", src);
    printf("Enter destination directory: ");
    scanf("%s", dest);

    char *original = (char *)malloc(100 * sizeof(char));
    getcwd(original, 100);
    printf("Original: %s\n", original);

    char *new_src = (char *)malloc(100 * sizeof(char));
    memset(new_src, 0, 100);
    char *new_dest = (char *)malloc(100 * sizeof(char));
    memset(new_dest, 0, 100);
    strcpy(new_src, original);
    strcat(new_src, src);
    new_src[strlen(new_src)] = '\0';
    strcpy(new_dest, original);
    strcat(new_dest, dest);
    new_dest[strlen(new_dest)] = '\0';

    printf("New source: %s\n", new_src);
    printf("New destination: %s\n", new_dest);

    int status = chdir(new_dest);
    if (status == -1)
    {
        perror("Error in chdir() function call: ");
        return 0;
    }

    char *dirname = (char *)malloc(100 * sizeof(char));
    memset(dirname, 0, 100);
    
    char *temp = (char *)malloc(100 * sizeof(char));
    memset(temp, 0, 100);
    strcpy(temp, src);

    char *token = strtok(temp, "/");
    while (token != NULL)
    {
        memset(dirname, 0, 100);
        strcpy(dirname, token);
        dirname[strlen(dirname)] = '\0';
        token = strtok(NULL, "/");
    }

    printf("Dirname: %s\n", dirname);

    int status1 = mkdir(dirname, 0777);
    if (status1 == -1)
    {
        perror("Error in mkdir() function call: ");
        return 0;
    }

    strcat(new_dest, "/");
    strcat(new_dest, dirname);
    new_dest[strlen(new_dest)] = '\0';


    copyfcn(new_src, new_dest);


}