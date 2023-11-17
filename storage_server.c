#include "storage_server.h"


int get_random_port_number()
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
    server_address.sin_port = htons(0);
    server_address.sin_addr.s_addr = inet_addr(SS_IP);

    int bind_success = bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_success == -1)
    {
        perror("Error in bind() function call: ");
        exit(1);
    }

    struct sockaddr_in temp;
    int len = sizeof(temp);
    getsockname(sock, (struct sockaddr *)&temp, (socklen_t *)&len);
    int port_number = ntohs(temp.sin_port);
    close(sock);
    return port_number;
}

int nm_conn_port;
int s2s_conn_port;
void *naming_server_init(void *)
{
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Error in socket() function call: ");
        exit(1);
    }
    int server_port_number = NM_INIT_PORT;
    char *ip_address = NM_CONN_IP;
    char buffer[4096];
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port_number);
    server_address.sin_addr.s_addr = inet_addr(ip_address);

    int connect_success = connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connect_success == -1)
    {
        perror("Error in connect() function call: ");
        exit(1);
    }

    struct data_of_ss initial_data_of_ss;
    initial_data_of_ss.port_number = get_random_port_number();
    nm_conn_port = initial_data_of_ss.port_number;
    printf("Enter number of paths: ");
    scanf("%d", &initial_data_of_ss.number_of_paths);
    initial_data_of_ss.paths[initial_data_of_ss.number_of_paths];
    for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
    {
        initial_data_of_ss.paths[i].permissions = 1; // hard coded for time being . Will change this later
        printf("Enter path %d: ", (i + 1));
        scanf("%s", initial_data_of_ss.paths[i].path);
        initial_data_of_ss.paths[i].path[strlen(initial_data_of_ss.paths[i].path)] = '\0';
        // printf("%s\n", initial_data_of_ss.paths[i].path);
        // check if it is a file or directory
    }

    int current_index = initial_data_of_ss.number_of_paths;
    // printf("Here151\n");
    for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
    {
        struct stat path_stat;
        char path2[100];
        path2[0] = '.';
        strcpy(path2 + 1, initial_data_of_ss.paths[i].path);
        path2[strlen(path2)] = '\0';
        stat(path2, &path_stat);
        if (S_ISDIR(path_stat.st_mode))
        {
            DIR *dir;
            struct dirent *curr_elem_of_dir;
            if ((dir = opendir(path2)) != NULL)
            {
                curr_elem_of_dir = readdir(dir);
                while (curr_elem_of_dir != NULL)
                {
                    if (strcmp(curr_elem_of_dir->d_name, ".") != 0 && strcmp(curr_elem_of_dir->d_name, "..") != 0)
                    {
                        strcpy(initial_data_of_ss.paths[current_index].path, initial_data_of_ss.paths[i].path);
                        strcat(initial_data_of_ss.paths[current_index].path, "/");
                        strcat(initial_data_of_ss.paths[current_index].path, curr_elem_of_dir->d_name);
                        initial_data_of_ss.paths[current_index].permissions = 1;
                        current_index++;
                        initial_data_of_ss.number_of_paths++;
                    }
                    curr_elem_of_dir = readdir(dir);
                }
            }
        }
    }

    int err_check = send(sock, &initial_data_of_ss, sizeof(initial_data_of_ss), 0);
    if (err_check == -1)
    {
        perror("Error in send() function call: ");
        return NULL;
    }
    printf("Data sent to naming server\n");
    return NULL;
}

void *naming_server_communication(void *)
{
    int socket_nm = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_nm == -1)
    {
        perror("Error in socket() function call: ");
        exit(1);
    }

    struct sockaddr_in server_address_nm;

    memset(&server_address_nm, 0, sizeof(server_address_nm));
    server_address_nm.sin_family = AF_INET;
    server_address_nm.sin_port = htons(nm_conn_port);
    server_address_nm.sin_addr.s_addr = inet_addr(SS_IP);

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
        memset(buffer1, 0, 4096);
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
            makedirss(dirname, path, client_socket_nm);
        }
        else if (strcmp(token, "deletedir") == 0)
        {
            char *path = strtok(NULL, " ");
            deletedirss(path, client_socket_nm);
        }
        else if (strcmp(token, "makefile") == 0)
        {
            char *filename = strtok(NULL, " ");
            char *path = strtok(NULL, " ");
            makefiless(filename, path, client_socket_nm);
        }
        else if (strcmp(token, "deletefile") == 0)
        {
            char *path = strtok(NULL, " ");
            deletefiless(path, client_socket_nm);
        }
        else if(strcmp(token, "copyfile")==0)
        {
            char *temp= strtok(NULL, " ");
            if(strcmp(temp,"dest")==0)
            {
                char *dest = strtok(NULL, " ");
                ss_info *ss_to_receive = (ss_info *)malloc(sizeof(ss_info));
                int bytes_received = recv(client_socket_nm, ss_to_receive, sizeof(ss_info), 0);
                if (bytes_received == -1)
                {
                    perror("Error in recv() function call: ");
                    exit(1);
                }
                copyfilereceive(dest,ss_to_receive,client_socket_nm);
            }
            else if(strcmp(temp,"src")==0)
            {
                char *src = strtok(NULL, " ");
                ss_info *ss_to_send = (ss_info *)malloc(sizeof(ss_info));
                int bytes_received = recv(client_socket_nm, ss_to_send, sizeof(ss_info), 0);
                if (bytes_received == -1)
                {
                    perror("Error in recv() function call: ");
                    exit(1);
                }
                copyfiless(src,ss_to_send,client_socket_nm);
                
            }
        }

        close(client_socket_nm);
    }
}

void *client_thread(void *)
{
    return NULL;
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, naming_server_init, NULL);

    pthread_join(thread_id, NULL);

    pthread_t thread_id2;
    pthread_create(&thread_id2, NULL, naming_server_communication, NULL);

    pthread_t thread_id3;
    pthread_create(&thread_id3, NULL, client_thread, NULL);

    pthread_join(thread_id2, NULL);
    pthread_join(thread_id3, NULL);
    return 0;
}