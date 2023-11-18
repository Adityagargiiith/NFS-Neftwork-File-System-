#include "copyfile.h"

void copyfilenm(char *src, char *dest, int client_socket)
{
    printf("Src: %s\n", src);
    printf("Dest: %s\n", dest);
    ss_info ans = search_path_in_trie(src);
    if (ans.ss_port == -1)
    {
        int status = SRC_NOT_FOUND;
        if (send(client_socket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
        return;
    }

    if(ans.dir_or_file == IS_DIR)
    {
        int status = SRC_IS_DIR;
        if (send(client_socket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
        return;
    }

    ss_info ans1 = search_path_in_trie(dest);
    if (ans1.ss_port == -1)
    {
        int status = DEST_NOT_FOUND;
        if (send(client_socket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
        return;
    }

    if(ans1.dir_or_file == IS_FILE)
    {
        int status = DEST_IS_FILE;
        if (send(client_socket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
        return;
    }

    printf("Port 1: %d\n", ans.ss_port);
    printf("Port 2: %d\n", ans1.ss_port);

    int sock_ss = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ss == -1)
    {
        perror("Error in socket() function call: ");
        return;
    }
    struct sockaddr_in server_address_ss;
    memset(&server_address_ss, 0, sizeof(server_address_ss));
    server_address_ss.sin_family = AF_INET;
    server_address_ss.sin_port = htons(ans.ss_port);
    server_address_ss.sin_addr.s_addr = inet_addr(ans.ss_ip);

    int connect_success = connect(sock_ss, (struct sockaddr *)&server_address_ss, sizeof(server_address_ss));
    if (connect_success == -1)
    {
        perror("Error in connect() function call: ");
        return;
    }

    char *msg_to_ss = (char *)malloc(sizeof(char) * 100);
    strcpy(msg_to_ss, "copyfile src ");
    strcat(msg_to_ss, src);

    msg_to_ss[strlen(msg_to_ss)] = '\0';
    printf("Msg to ss: %s\n", msg_to_ss);
    if (send(sock_ss, msg_to_ss, strlen(msg_to_ss), 0) == -1)
    {
        perror("Error in send() function call: ");
        return;
    }

    usleep(1000);

    if (send(sock_ss, &ans1, sizeof(ans1), 0) == -1)
    {
        perror("Error in send() function call: ");
        return;
    }

    int sock_ss1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ss1 == -1)
    {
        perror("Error in socket() function call: ");
        return;
    }
    struct sockaddr_in server_address_ss1;
    memset(&server_address_ss1, 0, sizeof(server_address_ss1));
    server_address_ss1.sin_family = AF_INET;
    server_address_ss1.sin_port = htons(ans1.ss_port);
    server_address_ss1.sin_addr.s_addr = inet_addr(ans1.ss_ip);

    int connect_success1 = connect(sock_ss1, (struct sockaddr *)&server_address_ss1, sizeof(server_address_ss1));
    if (connect_success1 == -1)
    {
        perror("Error in connect() function call: ");
        return;
    }

    char *msg_to_ss1 = (char *)malloc(sizeof(char) * 100);
    strcpy(msg_to_ss1, "copyfile dest ");
    strcat(msg_to_ss1, dest);

    msg_to_ss1[strlen(msg_to_ss1)] = '\0';
    printf("Msg to ss1: %s\n", msg_to_ss1);

    if (send(sock_ss1, msg_to_ss1, strlen(msg_to_ss1), 0) == -1)
    {
        perror("Error in send() function call: ");
        return;
    }

    usleep(1000);

    if (send(sock_ss1, &ans, sizeof(ans), 0) == -1)
    {
        perror("Error in send() function call: ");
        return;
    }

    printf("Sent\n");

    int status;
    if (recv(sock_ss, &status, sizeof(status), 0) == -1)
    {
        perror("Error in recv() function call: ");
        return;
    }

    int status1;
    if (recv(sock_ss1, &status1, sizeof(status1), 0) == -1)
    {
        perror("Error in recv() function call: ");
        return;
    }

    if (status == SUCCESS && status1 == SUCCESS)
    {
        int status = SUCCESS;
        if (send(client_socket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
    }
    else
    {
        int status = COPY_ERROR;
        if (send(client_socket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
    }

    close(sock_ss);
    close(sock_ss1);
    return;
}