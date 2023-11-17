#include "deletedir.h"

void deletedirnm(char* path, int client_socket_nm)
{
    ss_info ans =search_path_in_trie(path);

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
    strcpy(msg_to_ss, "deletedir ");
    strcat(msg_to_ss, path);

    if (send(sock_ss, msg_to_ss, strlen(msg_to_ss), 0) == -1)
    {
        perror("Error in send() function call: ");
        return;
    }

    int status;

    if (recv(sock_ss, &status, sizeof(status), 0) == -1)
    {
        perror("Error in recv() function call: ");
        return;
    }

    close(sock_ss);
    printf("HERE12345\n");
    if (status == SUCCESS)
    {
        printf("Yaha\n");
        delete_from_trie(path);
        printf("Here11\n");
    }
    else
    {
        printf("Error in deleting directory\n");
    }

    if (send(client_socket_nm, &status, sizeof(status), 0) == -1)
    {
        perror("Error in send() function call: ");
        return;
    }

    return;
}