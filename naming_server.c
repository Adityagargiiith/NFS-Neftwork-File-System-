#include "naming_server.h"
#include "storage_server.h"

// 1 for dir and 2 for file
struct tree_node *root;
FILE *log_file;

int failure[100000];
struct data_of_ss initial_data[100];
int backup_pending[100000];
int curr_number_of_ss;
struct replica_info
{
    int original_ss_index;
    char original_ss_ip[20];
    int original_ss_port;
    int replica1_ss_index;
    int replica2_ss_index;
    int replica1_ss_port;
    int replica2_ss_port;
    char replica1_ss_ip[20];
    char replica2_ss_ip[20];
    int number_of_references;
};

struct replica_info backup_arr[100];

void *ss_backup_update(void *arg)
{
    printf("Here\n");
    pthread_detach(pthread_self());
    int ss_number = *((int *)arg);
    while (1)
    {
        if (backup_pending[ss_number] == 1)
        {
            // printf("Here\n");
            if (backup_arr[ss_number].replica1_ss_index == -1)
            {
                continue;
            }
            for (int i = 0; i < initial_data[ss_number].number_of_paths; i++)
            {
                if (initial_data[ss_number].paths[i].dir_or_file == 1)
                {
                    printf("Here1\n");
                    int sock_ss_backup = socket(AF_INET, SOCK_STREAM, 0);
                    if (sock_ss_backup == -1)
                    {
                        perror("Error in socket() function call: ");
                        return NULL;
                    }
                    struct sockaddr_in server_address_ss_backup;
                    memset(&server_address_ss_backup, 0, sizeof(server_address_ss_backup));
                    server_address_ss_backup.sin_family = AF_INET;
                    server_address_ss_backup.sin_port = htons(initial_data[ss_number].port_number);
                    server_address_ss_backup.sin_addr.s_addr = inet_addr(initial_data[ss_number].ip);
                    int connect_success = connect(sock_ss_backup, (struct sockaddr *)&server_address_ss_backup, sizeof(server_address_ss_backup));
                    if (connect_success == -1)
                    {
                        perror("Error in connect() function call: ");
                        return NULL;
                    }
                    char *msg = (char *)malloc(sizeof(char) * 100);
                    memset(msg, 0, 100);
                    strcpy(msg, "copydir src ");
                    strcat(msg, initial_data[ss_number].paths[i].path);

                    msg[strlen(msg)] = '\0';
                    if (send(sock_ss_backup, msg, strlen(msg), 0) == -1)
                    {
                        perror("Error in send() function call: ");
                        return NULL;
                    }
                    usleep(1000);
                    ss_info to_send;
                    to_send.s2s_port = initial_data[backup_arr[ss_number].replica1_ss_index].s2s_port;
                    strcpy(to_send.ss_ip, initial_data[backup_arr[ss_number].replica1_ss_index].ip);
                    if (send(sock_ss_backup, &to_send, sizeof(to_send), 0) == -1)
                    {
                        perror("Error in send() function call: ");
                        return NULL;
                    }
                    int sock_ss_backup1 = socket(AF_INET, SOCK_STREAM, 0);
                    if (sock_ss_backup1 == -1)
                    {
                        perror("Error in socket() function call: ");
                        return NULL;
                    }
                    struct sockaddr_in server_address_ss_backup1;
                    memset(&server_address_ss_backup1, 0, sizeof(server_address_ss_backup1));
                    server_address_ss_backup1.sin_family = AF_INET;
                    server_address_ss_backup1.sin_port = htons(initial_data[backup_arr[ss_number].replica1_ss_index].port_number);
                    server_address_ss_backup1.sin_addr.s_addr = inet_addr(initial_data[backup_arr[ss_number].replica1_ss_index].ip);
                    int connect_success1 = connect(sock_ss_backup1, (struct sockaddr *)&server_address_ss_backup1, sizeof(server_address_ss_backup1));
                    if (connect_success1 == -1)
                    {
                        perror("Error in connect() function call: ");
                        return NULL;
                    }
                    char *msg1 = (char *)malloc(sizeof(char) * 100);
                    memset(msg1, 0, 100);
                    strcpy(msg1, "copydir dest ");
                    strcat(msg1, "/");
                    msg1[strlen(msg1)] = '\0';
                    if (send(sock_ss_backup1, msg1, strlen(msg1), 0) == -1)
                    {
                        perror("Error in send() function call: ");
                        return NULL;
                    }
                    usleep(1000);
                    ss_info to_send1;
                    to_send1.s2s_port = initial_data[ss_number].s2s_port;
                    strcpy(to_send1.ss_ip, initial_data[ss_number].ip);
                    if (send(sock_ss_backup1, &to_send1, sizeof(to_send1), 0) == -1)
                    {
                        perror("Error in send() function call: ");
                        return NULL;
                    }
                    int status;
                    if (recv(sock_ss_backup, &status, sizeof(status), 0) == -1)
                    {
                        perror("Error in recv() function call: ");
                        return NULL;
                    }
                    int status1;
                    if (recv(sock_ss_backup1, &status1, sizeof(status1), 0) == -1)
                    {
                        perror("Error in recv() function call: ");
                        return NULL;
                    }
                    close(sock_ss_backup);
                    close(sock_ss_backup1);
                    if (backup_arr[ss_number].replica2_ss_index == -1)
                    {
                        continue;
                    }
                    int sock_ss_backup2 = socket(AF_INET, SOCK_STREAM, 0);
                    if (sock_ss_backup2 == -1)
                    {
                        perror("Error in socket() function call: ");
                        return NULL;
                    }
                    struct sockaddr_in server_address_ss_backup2;
                    memset(&server_address_ss_backup2, 0, sizeof(server_address_ss_backup2));
                    server_address_ss_backup2.sin_family = AF_INET;
                    server_address_ss_backup2.sin_port = htons(initial_data[ss_number].port_number);
                    server_address_ss_backup2.sin_addr.s_addr = inet_addr(initial_data[ss_number].ip);
                    int connect_success2 = connect(sock_ss_backup2, (struct sockaddr *)&server_address_ss_backup2, sizeof(server_address_ss_backup2));
                    if (connect_success2 == -1)
                    {
                        perror("Error in connect() function call: ");
                        return NULL;
                    }
                    char *msg2 = (char *)malloc(sizeof(char) * 100);
                    memset(msg2, 0, 100);
                    strcpy(msg2, "copydir src ");
                    strcat(msg2, initial_data[ss_number].paths[i].path);

                    msg2[strlen(msg2)] = '\0';
                    if (send(sock_ss_backup2, msg2, strlen(msg2), 0) == -1)
                    {
                        perror("Error in send() function call: ");
                        return NULL;
                    }
                    usleep(1000);
                    ss_info to_send2;
                    to_send2.s2s_port = initial_data[backup_arr[ss_number].replica2_ss_index].s2s_port;
                    strcpy(to_send2.ss_ip, initial_data[backup_arr[ss_number].replica2_ss_index].ip);
                    if (send(sock_ss_backup2, &to_send2, sizeof(to_send2), 0) == -1)
                    {
                        perror("Error in send() function call: ");
                        return NULL;
                    }
                    int sock_ss_backup3 = socket(AF_INET, SOCK_STREAM, 0);
                    if (sock_ss_backup3 == -1)
                    {
                        perror("Error in socket() function call: ");
                        return NULL;
                    }
                    struct sockaddr_in server_address_ss_backup3;
                    memset(&server_address_ss_backup3, 0, sizeof(server_address_ss_backup3));
                    server_address_ss_backup3.sin_family = AF_INET;
                    server_address_ss_backup3.sin_port = htons(initial_data[backup_arr[ss_number].replica2_ss_index].port_number);
                    server_address_ss_backup3.sin_addr.s_addr = inet_addr(initial_data[backup_arr[ss_number].replica2_ss_index].ip);
                    int connect_success3 = connect(sock_ss_backup3, (struct sockaddr *)&server_address_ss_backup3, sizeof(server_address_ss_backup3));
                    if (connect_success3 == -1)
                    {
                        perror("Error in connect() function call: ");
                        return NULL;
                    }
                    char *msg3 = (char *)malloc(sizeof(char) * 100);
                    memset(msg3, 0, 100);
                    strcpy(msg3, "copydir dest ");
                    strcat(msg3, "/");
                    msg3[strlen(msg3)] = '\0';
                    if (send(sock_ss_backup3, msg3, strlen(msg3), 0) == -1)
                    {
                        perror("Error in send() function call: ");
                        return NULL;
                    }
                    usleep(1000);
                    ss_info to_send3;
                    to_send3.s2s_port = initial_data[ss_number].s2s_port;
                    strcpy(to_send3.ss_ip, initial_data[ss_number].ip);
                    if (send(sock_ss_backup3, &to_send3, sizeof(to_send3), 0) == -1)
                    {
                        perror("Error in send() function call: ");
                        return NULL;
                    }
                    int status2;
                    if (recv(sock_ss_backup2, &status2, sizeof(status2), 0) == -1)
                    {
                        perror("Error in recv() function call: ");
                        return NULL;
                    }
                    int status3;
                    if (recv(sock_ss_backup3, &status3, sizeof(status3), 0) == -1)
                    {
                        perror("Error in recv() function call: ");
                        return NULL;
                    }
                    close(sock_ss_backup2);
                    close(sock_ss_backup3);
                }
            }
        }
    }
}
void *ss_init_thread(void *)
{
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
    int listen_success = listen(sock, 10);
    if (listen_success == -1)
    {
        perror("Error in listen() function call: ");
        exit(1);
    }
    while (1)
    {
        printf("Current number of ss is %d\n", curr_number_of_ss);
        for (int i = 0; i < curr_number_of_ss; i++)
        {
            printf("Original ss index is %d\n", backup_arr[i].original_ss_index);
            printf("Replica1 ss index is %d\n", backup_arr[i].replica1_ss_index);
            printf("Replica2 ss index is %d\n", backup_arr[i].replica2_ss_index);
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
        initial_data[curr_number_of_ss] = initial_data_of_ss;
        backup_arr[curr_number_of_ss].original_ss_index = curr_number_of_ss;
        strcpy(backup_arr[curr_number_of_ss].original_ss_ip, ip_address_of_ss);
        strcpy(initial_data[curr_number_of_ss].ip, ip_address_of_ss);
        backup_arr[curr_number_of_ss].original_ss_port = initial_data_of_ss.port_number;
        if (curr_number_of_ss == 0)
        {
            curr_number_of_ss++;
            pthread_t ss_backup_update_thread_id;
            int arg1 = curr_number_of_ss;
            pthread_create(&ss_backup_update_thread_id, NULL, ss_backup_update, (void *)(&arg1));
            continue;
        }
        else if (curr_number_of_ss == 1)
        {
            backup_arr[0].replica1_ss_index = curr_number_of_ss;
            strcpy(backup_arr[0].replica1_ss_ip, ip_address_of_ss);
            backup_arr[0].replica1_ss_port = initial_data_of_ss.port_number;
            backup_arr[curr_number_of_ss].replica1_ss_index = 0;
            strcpy(backup_arr[curr_number_of_ss].replica1_ss_ip, backup_arr[0].original_ss_ip);
            backup_arr[curr_number_of_ss].replica1_ss_port = backup_arr[0].original_ss_port;
            backup_arr[curr_number_of_ss].number_of_references = 1;
            backup_arr[0].number_of_references++;
            pthread_t ss_backup_update_thread_id;
            int arg1 = curr_number_of_ss;
            pthread_create(&ss_backup_update_thread_id, NULL, ss_backup_update, (void *)(&arg1));
            curr_number_of_ss++;
        }
        else if (curr_number_of_ss == 2)
        {
            backup_arr[0].replica2_ss_index = curr_number_of_ss;
            strcpy(backup_arr[0].replica2_ss_ip, ip_address_of_ss);
            backup_arr[0].replica2_ss_port = initial_data_of_ss.port_number;
            backup_arr[curr_number_of_ss].replica2_ss_index = 0;
            strcpy(backup_arr[curr_number_of_ss].replica2_ss_ip, backup_arr[0].original_ss_ip);
            backup_arr[curr_number_of_ss].replica2_ss_port = backup_arr[0].original_ss_port;
            backup_arr[curr_number_of_ss].number_of_references++;
            backup_arr[0].number_of_references++;
            backup_arr[1].replica2_ss_index = curr_number_of_ss;
            strcpy(backup_arr[1].replica2_ss_ip, ip_address_of_ss);
            backup_arr[1].replica2_ss_port = initial_data_of_ss.port_number;
            backup_arr[curr_number_of_ss].replica1_ss_index = 1;
            strcpy(backup_arr[curr_number_of_ss].replica1_ss_ip, backup_arr[1].original_ss_ip);
            backup_arr[curr_number_of_ss].replica1_ss_port = backup_arr[1].original_ss_port;
            backup_arr[curr_number_of_ss].number_of_references++;
            backup_arr[1].number_of_references++;
            pthread_t ss_backup_update_thread_id;
            int arg1 = curr_number_of_ss;
            pthread_create(&ss_backup_update_thread_id, NULL, ss_backup_update, (void *)(&arg1));
            curr_number_of_ss++;
        }
        else
        {
            int min_index = 0;
            int min = backup_arr[0].number_of_references;
            for (int i = 1; i < curr_number_of_ss; i++)
            {
                if (backup_arr[i].number_of_references < min)
                {
                    min = backup_arr[i].number_of_references;
                    min_index = i;
                }
            }
            backup_arr[curr_number_of_ss].replica1_ss_index = min_index;
            strcpy(backup_arr[curr_number_of_ss].replica1_ss_ip, backup_arr[min_index].original_ss_ip);
            backup_arr[curr_number_of_ss].replica1_ss_port = backup_arr[min_index].original_ss_port;
            backup_arr[min_index].number_of_references++;
            int min2_index = 0;
            int min2 = backup_arr[0].number_of_references;
            for (int i = 1; i < curr_number_of_ss; i++)
            {
                if (backup_arr[i].number_of_references < min2)
                {
                    if (i == min_index)
                    {
                        continue;
                    }
                    min2 = backup_arr[i].number_of_references;
                    min2_index = i;
                }
            }
            backup_arr[curr_number_of_ss].replica2_ss_index = min2_index;
            strcpy(backup_arr[curr_number_of_ss].replica2_ss_ip, backup_arr[min2_index].original_ss_ip);
            backup_arr[curr_number_of_ss].replica2_ss_port = backup_arr[min2_index].original_ss_port;
            backup_arr[min2_index].number_of_references++;
            pthread_t ss_backup_update_thread_id;
            int arg1 = curr_number_of_ss;
            pthread_create(&ss_backup_update_thread_id, NULL, ss_backup_update, (void *)(&arg1));
            curr_number_of_ss++;
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
    else if (strcmp(token, "read") == 0)
    {
        /* code */
        // printf("Here for read\n");
        char *filename = strtok(NULL, " ");
        printf("Filename is %s\n", filename);
        readnm(filename, client_socket);
    }
    else if (strcmp(token, "write") == 0)
    {
        /* code */
        char *filename = strtok(NULL, " ");
        printf("Filename is %s\n", filename);
        writenm(filename, client_socket);
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
    curr_number_of_ss = 0;
    memset(failure, 0, sizeof(failure));
    // memset(backup_arr, 0, sizeof(backup_arr));
    for (int i = 0; i < 100; i++)
    {
        backup_pending[i] = 1;
        backup_arr[i].original_ss_index = -1;
        backup_arr[i].replica1_ss_index = -1;
        backup_arr[i].replica2_ss_index = -1;
        backup_arr[i].number_of_references = 0;
    }

    log_file = fopen("naming_server.log", "a");
    pthread_t ss_init_thread_id;
    pthread_create(&ss_init_thread_id, NULL, ss_init_thread, NULL);
    pthread_t client_thread_id;
    pthread_create(&client_thread_id, NULL, client_thread, NULL);
    pthread_join(client_thread_id, NULL);
    pthread_join(ss_init_thread_id, NULL);
    fclose(log_file);
    return 0;
}