#include "copydir.h"

void rec_file(int server_fd)
{
    char file_name[100];
    memset(file_name, 0, sizeof(file_name));
    read(server_fd, file_name, sizeof(file_name));
    printf("File name recieved is %s\n", file_name);
    // int file_fd = open(file_name, O_RDONLY);
    // create
    int file_fd = open(file_name, O_RDONLY | O_WRONLY | O_CREAT, 0666);
    int number_of_bytes_to_recieve;
    read(server_fd, &number_of_bytes_to_recieve, sizeof(number_of_bytes_to_recieve));
    printf("Number of bytes to recieve is %d\n", number_of_bytes_to_recieve);
    // char buffer[100];
    // memset(buffer, 0, sizeof(buffer));
    int number_of_bytes_recieved = 0;
    while (1)
    {
        if (number_of_bytes_recieved >= number_of_bytes_to_recieve)
        {
            break;
        }
        // memset(buffer, 0, sizeof(buffer));
        int left = number_of_bytes_to_recieve - number_of_bytes_recieved;
        int size_of_buffer;
        if (left > 100)
        {
            size_of_buffer = 100;
        }
        else
        {
            size_of_buffer = left;
        }
        char new_buffer[size_of_buffer];
        int number_of_bytes_recieved_now = read(server_fd, new_buffer, sizeof(new_buffer));
        if (number_of_bytes_recieved_now == 0)
        {
            /* code */
            break;
        }
        write(file_fd, new_buffer, number_of_bytes_recieved_now);
        number_of_bytes_recieved += number_of_bytes_recieved_now;
    }
}

void rec_dir(int server_fd)
{
    char dir_name[100];
    memset(dir_name, 0, sizeof(dir_name));
    read(server_fd, dir_name, sizeof(dir_name));
    mkdir(dir_name, 0777);
}

void copydirreceive(char *dest, ss_info * ss_to_receive, int client_socket_nm)
{

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("Error in socket() function call: ");
        exit(1);
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(ss_to_receive->s2s_port);
    server_address.sin_addr.s_addr = inet_addr(ss_to_receive->ss_ip);
    int connect_success = connect(server_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connect_success == -1)
    {
        perror("Error in connect() function call: ");
        exit(1);
    }

    int number_of_paths;
    read(server_fd, &number_of_paths, sizeof(number_of_paths));
    while (number_of_paths--)
    {
        int dir_or_file;
        dir_or_file = 0;
        read(server_fd, &dir_or_file, sizeof(dir_or_file));
        if (dir_or_file == 0)
        {
            rec_file(server_fd);
        }
        else if (dir_or_file == 1)
        {
            rec_dir(server_fd);
        }
    }
    close(server_fd);

    int status = SUCCESS;
    if (send(client_socket_nm, &status, sizeof(status), 0) == -1)
    {
        perror("Error in send() function call: ");
        return;
    }

    return;
}

int count_no_of_paths(char *src)
{
    char *path = (char *)malloc(sizeof(char) * 100);
    strcpy(path, src);
    path[strlen(path)] = '\0';
    int dir_or_file = 1;
    char *dir_name = (char *)malloc(sizeof(char) * 100);
    strcpy(dir_name, path);
    dir_name[strlen(dir_name)] = '\0';
    DIR *dir = opendir(dir_name);
    struct dirent *curr_elem_of_dir;
    curr_elem_of_dir = readdir(dir);
    int count = 1;
    while (curr_elem_of_dir != NULL)
    {
        if (strcmp(curr_elem_of_dir->d_name, ".") != 0 && strcmp(curr_elem_of_dir->d_name, "..") != 0)
        {
            memset(path, 0, sizeof(path));
            strcpy(path, dir_name);
            strcat(path, "/");
            strcat(path, curr_elem_of_dir->d_name);
            path[strlen(path)] = '\0';
            struct stat path_stat;
            stat(path, &path_stat);
            if (S_ISDIR(path_stat.st_mode))
            {
                dir_or_file = 1;
            }
            else
            {
                dir_or_file = 0;
            }
            if (dir_or_file == 1)
            {
                count += count_no_of_paths(path);
            }
            else
            {
                count++;
            }
        }
        curr_elem_of_dir = readdir(dir);
    }
    return count;
}

int send_file(char *path, int client_fd)
{
    char *file_path = (char *)malloc(sizeof(char) * 100);
    strcpy(file_path, path);
    file_path[strlen(file_path)] = '\0';

    struct stat path_stat;
    int status = stat(file_path, &path_stat);
    if (status == -1)
    {
        perror("Error in stat() function call: ");
        return -1;
    }

    if (!(path_stat.st_mode & S_IRUSR))
    {
        perror("Error in stat() function call: ");
        return -1;
    }
    int size_of_file = path_stat.st_size;

    char *file_name = (char *)malloc(sizeof(char) * 100);
    strcpy(file_name, file_path);
    file_name[strlen(file_name)] = '\0';
    int dir_or_file = 0;
    write(client_fd, &dir_or_file, sizeof(dir_or_file));
    write(client_fd, file_name, sizeof(file_name));
    write(client_fd, &size_of_file, sizeof(size_of_file));
    int file_fd = open(file_path, O_RDONLY);
    int number_of_bytes_to_send = size_of_file;
    int number_of_bytes_sent = 0;
    while (1)
    {
        if (number_of_bytes_sent >= number_of_bytes_to_send)
        {
            break;
        }
        int left = number_of_bytes_to_send - number_of_bytes_sent;
        int size_of_buffer;
        if (left > 100)
        {
            size_of_buffer = 100;
        }
        else
        {
            size_of_buffer = left;
        }
        char buffer[size_of_buffer];
        memset(buffer, 0, sizeof(buffer));
        int number_of_bytes_read_now = read(file_fd, buffer, sizeof(buffer));
        if (number_of_bytes_read_now == 0)
        {
            break;
        }
        if (number_of_bytes_read_now < 0)
        {
            perror("Error in read() function call: ");
            return -1;
        }
        write(client_fd, buffer, number_of_bytes_read_now);
        number_of_bytes_sent += number_of_bytes_read_now;
    }
}

int send_dir(char *src, int client_fd)
{
    char *path = (char *)malloc(sizeof(char) * 100);
    strcpy(path, src);
    path[strlen(path)] = '\0';
    int dir_or_file = 1;
    char *dir_name = (char *)malloc(sizeof(char) * 100);
    strcpy(dir_name, path);
    dir_name[strlen(dir_name)] = '\0';
    write(client_fd, &dir_or_file, sizeof(dir_or_file));
    write(client_fd, dir_name, sizeof(dir_name));
    DIR *dir = opendir(dir_name);
    struct dirent *curr_elem_of_dir;
    curr_elem_of_dir = readdir(dir);
    while (curr_elem_of_dir != NULL)
    {
        if (strcmp(curr_elem_of_dir->d_name, ".") != 0 && strcmp(curr_elem_of_dir->d_name, "..") != 0)
        {
            memset(path, 0, sizeof(path));
            strcpy(path, dir_name);
            strcat(path, "/");
            strcat(path, curr_elem_of_dir->d_name);
            path[strlen(path)] = '\0';
            struct stat path_stat;
            stat(path, &path_stat);
            if (S_ISDIR(path_stat.st_mode))
            {
                dir_or_file = 1;
            }
            else
            {
                dir_or_file = 0;
            }
            if (dir_or_file == 1)
            {
                send_dir(path, client_fd);
            }
            else
            {
                send_file(path, client_fd);
            }
        }
        curr_elem_of_dir = readdir(dir);
    }

    return 0;
}

void copydirss(char *src, ss_info *ss_to_send, int client_socket_nm, int s2s_conn_port)
{
    int sock_ss = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ss == -1)
    {
        perror("Error in socket() function call: ");
        return;
    }
    struct sockaddr_in server_address_ss;
    memset(&server_address_ss, 0, sizeof(server_address_ss));
    server_address_ss.sin_family = AF_INET;
    server_address_ss.sin_port = htons(s2s_conn_port);
    server_address_ss.sin_addr.s_addr = inet_addr(SS_IP);

    int bind_success = bind(sock_ss, (struct sockaddr *)&server_address_ss, sizeof(server_address_ss));
    if (bind_success == -1)
    {
        perror("Error in bind() function call: ");
        return;
    }

    int listen_success = listen(sock_ss, 10);
    if (listen_success == -1)
    {
        perror("Error in listen() function call: ");
        return;
    }

    int client_socket_ss;
    struct sockaddr_in client_address_ss;
    int client_address_length_ss = sizeof(client_address_ss);
    client_socket_ss = accept(sock_ss, (struct sockaddr *)&client_address_ss, &client_address_length_ss);
    if (client_socket_ss < 0)
    {
        perror("Error in accept() function call: ");
        return;
    }

    char *dir_name = (char *)malloc(sizeof(char) * 100);
    memset(dir_name, 0, sizeof(dir_name));
    strcpy(dir_name, src);
    dir_name[strlen(dir_name)] = '\0';
    int ct = count_no_of_paths(dir_name);
    write(client_socket_ss, &ct, sizeof(ct));
    send_dir(dir_name, client_socket_ss);

    close(client_socket_ss);
    close(sock_ss);

    int status = SUCCESS;
    if (send(client_socket_nm, &status, sizeof(status), 0) == -1)
    {
        perror("Error in send() function call: ");
        return;
    }
    return;
}