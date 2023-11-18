#include "naming_server.h"
#include "storage_server.h"

// 1 for dir and 2 for file
struct tree_node *root;
FILE *log_file;

void *ss_init_thread(void *)
{
    // initialize_all_ss();
    init_root();
    int port_number = SS_PORT;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Error in socket() function call: ");
        exit(1);
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = inet_addr(IP);

    int bind_success = bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_success == -1)
    {
        perror("Error in bind() function call: ");
        exit(1);
    }
    while (1)
    {

        int listen_success = listen(sock, 10);
        if (listen_success == -1)
        {
            perror("Error in listen() function call: ");
            exit(1);
        }

        int client_socket;
        struct sockaddr_in client_address;
        int client_address_length = sizeof(client_address);
        client_socket = accept(sock, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1)
        {
            perror("Error in accept() function call: ");
            exit(1);
        }
        printf("Storage server connected\n");
        struct data_of_ss initial_data_of_ss;
        memset(&initial_data_of_ss, 0, sizeof(initial_data_of_ss));
        // recv(client_socket, &initial_data_of_ss, sizeof(initial_data_of_ss), 0);
        // printf("Here1\n");
        if (recv(client_socket, &initial_data_of_ss, sizeof(initial_data_of_ss), 0) == -1)
        {
            perror("Error in recv() function call: ");
            exit(1);
        }
        // get ip address of storage server from struct sockaddr_in client_address
        char *ip_address_of_ss = inet_ntoa(client_address.sin_addr);

        for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
        {
            insert_into_tree_new(initial_data_of_ss.paths[i].path, initial_data_of_ss.paths[i].permissions, ip_address_of_ss, initial_data_of_ss.port_number, initial_data_of_ss.client_port, initial_data_of_ss.s2s_port, initial_data_of_ss.paths[i].dir_or_file);
        }
    }
}

void *client_req_handler(void *arg)
{
    pthread_detach(pthread_self());
    int client_socket = *((int *)arg);
    char *msg = (char *)malloc(sizeof(char) * 100);
    memset(msg, 0, 100);
    if (recv(client_socket, msg, 100, 0) == -1)
    {
        perror("Error in recv() function call: ");
        exit(1);
    }
    char *token = strtok(msg, " ");
    fprintf(log_file, "Received a request from client: %s\n", msg);
    fflush(log_file); // Make sure the log message is immediately written to the file

    if (strcmp(token, "makedir") == 0)
    {
        char *name_of_dir = strtok(NULL, " ");
        char *path = strtok(NULL, " ");
        makedirnm(name_of_dir, path, client_socket);
    }
    else if (strcmp(token, "deletedir") == 0)
    {
        char *path = strtok(NULL, " ");
        deletedirnm(path, client_socket);
    }
    else if (strcmp(token, "makefile") == 0)
    {
        char *filename = strtok(NULL, " ");
        char *path = strtok(NULL, " ");
        makefilenm(filename, path, client_socket);
    }
    else if (strcmp(token, "deletefile") == 0)
    {
        char *path = strtok(NULL, " ");
        deletefilenm(path, client_socket);
    }
    else if (strcmp(token, "copyfile") == 0)
    {
        char *src = strtok(NULL, " ");
        char *dest = strtok(NULL, " ");
        copyfilenm(src, dest, client_socket);
    }
    else if (strcmp(token, "copydir") == 0)
    {
        char *src = strtok(NULL, " ");
        char *dest = strtok(NULL, " ");
        copydirnm(src, dest, client_socket);
    }

    close(client_socket);
    return NULL;
}

void *client_thread(void *)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Error in socket() function call: ");
        exit(1);
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    server_address.sin_port = htons(CLIENT_PORT);
    server_address.sin_addr.s_addr = inet_addr(IP);

    int bind_success = bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_success == -1)
    {
        perror("Error in bind() function call: ");
        exit(1);
    }

    int listen_success = listen(sock, 15);
    if (listen_success == -1)
    {
        perror("Error in listen() function call: ");
        exit(1);
    }

    while (1)
    {
        int client_socket;
        struct sockaddr_in client_address;
        int client_address_length = sizeof(client_address);
        client_socket = accept(sock, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1)
        {
            perror("Error in accept() function call: ");
            exit(1);
        }
        pthread_t client_req_handler_thread_id;
        pthread_create(&client_req_handler_thread_id, NULL, client_req_handler, (void *)&client_socket);
    }
}

int main()
{
    log_file = fopen("naming_server.log", "a");
    pthread_t ss_init_thread_id;
    pthread_create(&ss_init_thread_id, NULL, ss_init_thread, NULL);
    pthread_t client_thread_id;
    pthread_create(&client_thread_id, NULL, client_thread, NULL);
    pthread_join(ss_init_thread_id, NULL);
    fclose(log_file);
    return 0;
}