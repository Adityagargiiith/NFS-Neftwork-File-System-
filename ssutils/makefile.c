#include "makefile.h"

void makefiless(char *filename, char* path , int client_soket)
{
    char *new_path = (char *)malloc(strlen(path) + strlen(filename) + 4);
    char *tmp = ".";
    strcpy(new_path, tmp);
    strcat(new_path, path);
    strcat(new_path, "/");
    strcat(new_path, filename);
    printf("new_path: %s\n", new_path);

    int status;

    if(access(new_path,F_OK)==0)
    {
        status = FILE_ALREADY_EXISTS;
        send(client_soket,&status,sizeof(status),0);
        perror("Error in access() function call: ");
        return;
    }

    int fd = open(new_path,O_CREAT|O_RDWR,0777);
    if(fd==-1)
    {
        status = CREATE_ERROR;
        send(client_soket,&status,sizeof(status),0);
        perror("Error in open() function call: ");
        return;
    }
    printf("File %s created\n",filename);

    status = SUCCESS;
    if(send(client_soket,&status,sizeof(status),0)<0)
    {
        printf("Error in sending data to naming server\n");
        return;
    }

    close(fd);

}