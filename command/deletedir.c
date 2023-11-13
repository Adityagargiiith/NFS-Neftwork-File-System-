#include "deletedir.h"

void deletedir(char *input)
{
    char *cpy_command = (char *)malloc(strlen(input) + 1);
    strcpy(cpy_command, input);
    cpy_command[strlen(input)] = '\0';

    char *command = strtok(cpy_command, " ");

    char *dirname = strtok(NULL, " ");
    if (dirname == NULL)
    {
        printf("Usage: deletedir <dirname(with path)> \n");
        return;
    }

    char *token = strtok(NULL, " ");
    if (token != NULL)
    {
        printf("Usage: deletedir <dirname(with path)> \n");
        return;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        printf("Error in creating socket\n");
        return;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(NM_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(ip);

    int ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (ret < 0)
    {
        printf("Error in connecting to naming server\n");
        return;
    }

    char *msg = (char *)malloc(sizeof(char) * 100);
    strcpy(msg, "deletedir ");
    strcat(msg, dirname);

    int len = strlen(msg);
    msg[len] = '\0';

    if (send(sockfd, msg, strlen(msg), 0) < 0)
    {
        printf("Error in sending data to naming server\n");
        return;
    }

    int status;
    if (recv(sockfd, &status, sizeof(status), 0) < 0)
    {
        printf("Error in receiving data from naming server\n");
        return;
    }

    if (status == SUCCESS)
    {
        printf("Directory deleted successfully\n");
    }
    else if (status == DIR_NOT_FOUND)
    {
        printf("Directory not found\n");
    }
    else if (status == PERMISSION_DENIED)
    {
        printf("Permission Denied\n");
    }
    else if (status == INVALID_PATH)
    {
        printf("Invalid Path\n");
    }
    else if (status == INVALID_DIRNAME)
    {
        printf("Invalid Directory Name\n");
    }
    else
    {
        printf("Unknown Error occured while deleting directory\n");
    }

    close(sockfd);

    return;
}