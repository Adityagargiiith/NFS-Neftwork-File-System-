#include "write.h"

void write_file(char *input)
{
    char *cpy_command = (char *)malloc(sizeof(char) * strlen(input));
    strcpy(cpy_command, input);
    cpy_command[strlen(input)] = '\0';

    char *command = strtok(cpy_command, " ");

    char *filename = strtok(NULL, " ");
    if (filename == NULL)
    {
        printf("Usage: write <filename(with path)> \n");
        return;
    }

    char *token = strtok(NULL, " ");
    if (token != NULL)
    {
        printf("Usage: write <filename(with path)> \n");
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
    strcpy(msg, "write ");
    strcat(msg, filename);

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
        ss_info *ss = (ss_info *)malloc(sizeof(ss_info));
        if (recv(sockfd, ss, sizeof(ss_info), 0) < 0)
        {
            printf("Error in receiving data from naming server\n");
            return;
        }

        int ss_sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (ss_sockfd < 0)
        {
            printf("Error in creating socket\n");
            return;
        }

        struct sockaddr_in ss_serv_addr;
        memset(&ss_serv_addr, 0, sizeof(ss_serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(ss->ss_port);
        serv_addr.sin_addr.s_addr = inet_addr(ss->ss_ip);

        int ss_ret = connect(ss_sockfd, (struct sockaddr *)&ss_serv_addr, sizeof(ss_serv_addr));

        if (ss_ret < 0)
        {
            printf("Error in connecting to storage server\n");
            return;
        }

        char *msg2 = (char *)malloc(sizeof(char) * 100);
        strcpy(msg2, "write ");
        strcat(msg2, filename);

        int len2 = strlen(msg2);
        msg2[len2] = '\0';

        if (send(ss_sockfd, msg2, strlen(msg2), 0) < 0)
        {
            printf("Error in sending data to storage server\n");
            return;
        }

    }
    else if (status == FILE_NOT_FOUND)
    {
        printf("File not found\n");
        return;
    }
    else
    {
        printf("Error in writing file\n");
        return;
    }
}