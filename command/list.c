#include "list.h"

void list(char *input)
{
        char *array[2];
 array[0] = strtok(input, " ");
        array[1] = strtok(NULL, " ");
        // array[2] = strtok(NULL, " ");
 DIR *dir;
    struct dirent *entry;
    char path[50];
    // char dot[10];
    strcpy(path, ".");
    // strcat(path, array[2]);
    // strcat(path, "/");
    strcat(path, array[1]);
    dir = opendir(path);

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);

}