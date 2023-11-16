#include "storage_server.h"

int main()
{
    // tcp client
    // int sock = 0;
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        /* code */
        perror("Error in socket() function call: ");
        exit(1);
    }
    int server_port_number = 5572;
    char *ip_address = "127.0.0.1";
    char buffer[4096];
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = server_port_number;
    server_address.sin_addr.s_addr = inet_addr(ip_address);

    int connect_success = connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connect_success == -1)
    {
        /* code */
        perror("Error in connect() function call: ");
        exit(1);
    }

    // int number_of_accessible_paths;
    // printf("Enter number of accessible paths: ");
    // scanf("%d", &number_of_accessible_paths);

    // send(sock, &number_of_accessible_paths, sizeof(number_of_accessible_paths), 0);
    // for (int i = 0; i < number_of_accessible_paths; i++)
    // {
    //     char path[4096];
    //     printf("Enter path %d: ", i + 1);
    //     scanf("%s", path);
    //     printf("%s\n", path);
    //     send(sock, path, strlen(path), 0);
    // }

    struct data_of_ss initial_data_of_ss;
    scanf("%d", &initial_data_of_ss.number_of_paths);
    initial_data_of_ss.port_number = 6677; // hard coded for time being . Will change this later
    initial_data_of_ss.paths[initial_data_of_ss.number_of_paths];
    for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
    {
        initial_data_of_ss.paths[i].permissions = 1; // hard coded for time being . Will change this later
        printf("Enter path %d: ", i + 1);
        scanf("%s", initial_data_of_ss.paths[i].path);
        initial_data_of_ss.paths[i].path[strlen(initial_data_of_ss.paths[i].path)] = '\0';
        printf("%s\n", initial_data_of_ss.paths[i].path);
    }
    for (int i = initial_data_of_ss.number_of_paths; i < MAX_PATHS; i++)
    {
        initial_data_of_ss.paths[i].permissions = 0;
        initial_data_of_ss.paths[i].path[0] = '\0';
    }

    int err_check = send(sock, &initial_data_of_ss, sizeof(initial_data_of_ss), 0);
    printf("Data sent to naming server\n");

    int socket_nm = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_nm == -1)
    {
        perror("Error in socket() function call: ");
        exit(1);
    }

    struct sockaddr_in server_address_nm;

    memset(&server_address_nm, 0, sizeof(server_address_nm));
    server_address_nm.sin_family = AF_INET;
    server_address_nm.sin_port = htons(6677);
    server_address_nm.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(socket_nm, (struct sockaddr *)&server_address_nm, sizeof(server_address_nm)) == -1)
    {
        perror("Error in bind() function call: ");
        exit(1);
    }

    if (listen(socket_nm, 10) == -1)
    {
        perror("Error in listen() function call: ");
        exit(1);
    }

    while (1)
    {
        int client_socket_nm;
        struct sockaddr_in client_address_nm;
        int client_address_length_nm = sizeof(client_address_nm);
        client_socket_nm = accept(socket_nm, (struct sockaddr *)&client_address_nm, &client_address_length_nm);
        if (client_socket_nm == -1)
        {
            perror("Error in accept() function call: ");
            exit(1);
        }

        char *buffer1 = (char *)malloc(4096);
        int bytes_received = recv(client_socket_nm, buffer1, 4096, 0);
        if (bytes_received == -1)
        {
            perror("Error in recv() function call: ");
            exit(1);
        }
        char *token = strtok(buffer1, " ");

        if (strcmp(token, "makedir") == 0)
        {
            char *dirname = strtok(NULL, " ");
            char *path = strtok(NULL, " ");
            printf("dirname: %s\n", dirname);
            printf("path: %s\n", path);
            int status;

            // check if path is accessible
            // int path_accessible = 0;
            // for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
            // {
            //     if (strcmp(initial_data_of_ss.paths[i].path, path) == 0)
            //     {
            //         path_accessible = 1;
            //         break;
            //     }
            // }

            // if (path_accessible == 0)
            // {
            //     printf("Path not accessible\n");
            //     continue;
            // }

            // check if directory already exists
            char *path_to_check = (char *)malloc(strlen(path) + strlen(dirname) + 2);
            strcpy(path_to_check, path);
            strcat(path_to_check, "/");
            strcat(path_to_check, dirname);

            if (access(path_to_check, F_OK) == 0)
            {
                printf("Directory already exists\n");
                status = DIR_ALREADY_EXISTS;
                send(client_socket_nm, &status, sizeof(status), 0);
                continue;
            }

            char *new_path = (char *)malloc(strlen(path) + strlen(dirname) + 4);
            char *tmp = ".";
            strcpy(new_path, tmp);
            strcat(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, dirname);
            printf("new_path: %s\n", new_path);

            // create directory
            int err_check = mkdir(new_path, 0777);
            if (err_check == -1)
            {
                status = CREATE_ERROR;
                send(client_socket_nm, &status, sizeof(status), 0);
                perror("Error in mkdir() function call: ");
                continue;
            }
            printf("Directory created\n");

            status = SUCCESS;
            send(client_socket_nm, &status, sizeof(status), 0);
        }
        // send success message to naming server
    }

    // close(sock);

    return 0;
}