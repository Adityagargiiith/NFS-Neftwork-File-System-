#include "copyfile.h"

void copyfile(char *input)
{
    char *cpy_command = (char *)malloc(strlen(input) + 1);
    strcpy(cpy_command, input);
    cpy_command[strlen(input)] = '\0';

    char *command = strtok(cpy_command, " ");
    char *src = strtok(NULL, " ");
    if (src == NULL)
    {
        printf("Usage: copyfile <src> <dest>\n");
        return;
    }

    char *dest = strtok(NULL, " ");
    if (dest == NULL)
    {
        printf("Usage: copyfile <src> <dest>\n");
        return;
    }

    char *tmp = strtok(NULL, " ");
    if (tmp != NULL)
    {
        printf("Usage: copyfile <src> <dest>\n");
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
    strcpy(msg, "copyfile ");
    strcat(msg, src);
    strcat(msg, " ");
    strcat(msg, dest);

    int len = strlen(msg);
    msg[len] = '\0';

    printf("Message sent: %s\n", msg);
    if (send(sockfd, msg, strlen(msg), 0) < 0)
    {
        printf("Error in sending message to naming server\n");
        return;
    }

    int status;
    if (recv(sockfd, &status, sizeof(status), 0) < 0)
    {
        printf("Error in recieving status from naming server\n");
        return;
    }

    if (status == SUCCESS)
    {
        printf("File copied successfully\n");
    }
    else if (status == SRC_NOT_FOUND)
    {
        printf("Source file not found\n");
    }
    else if (status == DEST_NOT_FOUND)
    {
        printf("Destination file not found\n");
    }
    else if (status == SRC_IS_DIR)
    {
        printf("Source is a directory\n");
    }
    else if (status == DEST_IS_DIR)
    {
        printf("Destination is a directory\n");
    }
    else if (status == SRC_IS_FILE)
    {
        printf("Source is a file\n");
    }
    else if (status == DEST_IS_FILE)
    {
        printf("Destination is a file\n");
    }
    else if (status == COPY_ERROR)
    {
        printf("Error in copying file\n");
    }
    else
    {
        printf("Unknown error occured while copying file\n");
    }

    return;
    
}