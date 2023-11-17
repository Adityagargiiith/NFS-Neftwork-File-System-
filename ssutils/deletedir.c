#include "deletedir.h"

void deletedirss(char* path,int socket)
{
    int status;
    char *new_path = (char *)malloc(strlen(path) + 4);
    char *tmp = ".";
    strcpy(new_path, tmp);
    strcat(new_path, path);

    if (access(new_path, F_OK) == -1)
    {
        status = DIR_NOT_FOUND;
        send(socket, &status, sizeof(status), 0);
        perror("Error in access() function call: ");
        return;
    }
    // create directory
    int err_check = rmdir(new_path);
    if (err_check == -1)
    {
        status = DELETE_ERROR;
        send(socket, &status, sizeof(status), 0);
        perror("Error in rmdir() function call: ");
        return;
    }
    printf("Directory %s deleted\n",path);

    status = SUCCESS;
    if (send(socket, &status, sizeof(status), 0) < 0)
    {
        printf("Error in sending data to naming server\n");
        return;
    }
}