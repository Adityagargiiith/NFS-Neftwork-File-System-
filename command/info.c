#include "info.h"

void info(char *input)
{
    char *array[2];
    char *token;

    // char *array[2];
 array[0] = strtok(input, " ");
array[1] = strtok(NULL, " ");
array[2] = strtok(NULL, " ");
    // array[0] = strtok(input, " ");
// array[1] = strtok(NULL, " ");
    char path[50];
    strcpy(path, ".");
    // strcat(path, array[2]);
    strcat(path, array[2]);
    strcat(path, "/");
    strcat(path, array[1]);
    // printf("%s\n", path);
// printf("%s\n", path);
     struct stat fileStat;

    // Use stat function to get file information
    if (stat(path, &fileStat) == 0) {
        // Print file size
        printf("File Size: %ld bytes\n", fileStat.st_size);

        // Print access rights
        printf("Access Rights: ");
        printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf((fileStat.st_mode & S_IXOTH) ? "x\n" : "-\n");

        // Print timestamps
        printf("Last Access Time: %s", ctime(&fileStat.st_atime));
        printf("Last Modification Time: %s", ctime(&fileStat.st_mtime));
        printf("Last Status Change Time: %s", ctime(&fileStat.st_ctime));
    } else {
        // Handle error if stat function fails
        perror("Error");
    }


}