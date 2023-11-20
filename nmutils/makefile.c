#include "makefile.h"

int makefilenm(char *filename, char* path , int client_soket)
{
    ss_info ans =search_path_in_trie(path);

    if(ans.ss_port==-1)
    {
        int status = INVALID_PATH;
        if (send(client_soket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return -1;
        }
        return -1;
    }

    int sock_ss = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_ss == -1)
    {
        perror("Error in socket() function call: ");
        return -1;
    }

    struct sockaddr_in server_address_ss;
    memset(&server_address_ss, 0, sizeof(server_address_ss));
    server_address_ss.sin_family = AF_INET;
    server_address_ss.sin_port = htons(ans.ss_port);
    server_address_ss.sin_addr.s_addr = inet_addr(ans.ss_ip);

    int connect_success = connect(sock_ss, (struct sockaddr *)&server_address_ss, sizeof(server_address_ss));
    if(connect_success == -1)
    {
        perror("Error in connect() function call: ");
        return -1;
    }

    char *msg_to_ss = (char *)malloc(sizeof(char) * 100);
    strcpy(msg_to_ss, "makefile ");
    strcat(msg_to_ss, filename);
    strcat(msg_to_ss, " ");
    strcat(msg_to_ss, path);

    if(send(sock_ss, msg_to_ss, strlen(msg_to_ss), 0) == -1)
    {
        perror("Error in send() function call: ");
        return -1;
    }

    int status;
    if(recv(sock_ss, &status, sizeof(status), 0) == -1)
    {
        perror("Error in recv() function call: ");
        return -1;
    }

    close(sock_ss);

    char *new_path = (char *)malloc(sizeof(char) * (strlen(path) + strlen(filename) + 2));
    strcpy(new_path, path);
    strcat(new_path, "/");
    strcat(new_path, filename);

    if(status == SUCCESS)
    {
        insert_into_tree_new(new_path, 1, ans.ss_ip, ans.ss_port, ans.client_port, ans.s2s_port,0);
    }
    else
    {
        printf("Error in making file\n");
    }

    if(send(client_soket, &status, sizeof(status), 0) == -1)
    {
        perror("Error in send() function call: ");
        return -1;
    }
    

    return status;
}