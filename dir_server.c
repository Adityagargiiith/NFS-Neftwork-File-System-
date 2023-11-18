// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <pthread.h>
// #include <fcntl.h>
// #include <sys/stat.h>
// #include <dirent.h>

// // int client_fd;
// int copy_file(char file_path[], int client_fd)
// {
//     write(client_fd, "file", sizeof("file"));
//     char file_name[100];
//     int i = strlen(file_path) - 1;
//     while (i >= 0 && file_path[i] != '/')
//     {
//         i--;
//     }
//     i++;
//     int j = 0;
//     while (i < strlen(file_path))
//     {
//         file_name[j] = file_path[i];
//         i++;
//         j++;
//     }
//     file_name[j] = '\0';
//     write(client_fd, file_name, sizeof(file_name));
//     int file_fd = open(file_path, O_RDONLY, 0666);
//     struct stat file_stat;
//     int file_status = stat(file_path, &file_stat);
//     if (file_status < 0)
//     {
//         printf("File does not exist\n");
//         exit(0);
//     }
//     int number_of_bytes_to_send = file_stat.st_size;
//     write(client_fd, &number_of_bytes_to_send, sizeof(number_of_bytes_to_send));
//     int number_of_bytes_sent = 0;
//     char buffer[100];
//     while (number_of_bytes_sent < number_of_bytes_to_send)
//     {
//         int number_of_bytes_read = read(file_fd, buffer, sizeof(buffer));
//         if (number_of_bytes_read < 0)
//         {
//             printf("Error in reading file\n");
//             exit(0);
//         }
//         if (number_of_bytes_read == 0)
//         {
//             printf("File reading done\n");
//             break;
//         }
//         int sent_bytes = write(client_fd, buffer, number_of_bytes_read);
//         if (sent_bytes < 0)
//         {
//             printf("Error in sending file\n");
//             exit(0);
//         }
//         number_of_bytes_sent += sent_bytes;
//     }
//     return 0;

// }
// void copy_dir(char dir_path[], int client_fd)
// {
//     write(client_fd, "dir", sizeof("dir"));
//     char dir_name[100];
//     int i = strlen(dir_path) - 1;
//     while (i >= 0 && dir_path[i] != '/')
//     {
//         i--;
//     }
//     i++;
//     int j = 0;
//     while (i < strlen(dir_path))
//     {
//         dir_name[j] = dir_path[i];
//         i++;
//         j++;
//     }
//     dir_name[j] = '\0';
//     write(client_fd, dir_name, sizeof(dir_name));
//     printf("%s\n",dir_name);
//     DIR *dir = opendir(dir_path);
//     struct dirent *dir_entry;
//     dir_entry = readdir(dir);
//     while (dir_entry != NULL)
//     {
//         if (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0)
//         {
//             // check if it is a file or a directory
//             struct stat path_stat;
//             char path2[100];
//             strcpy(path2, dir_path);
//             strcat(path2, "/");
//             strcat(path2, dir_entry->d_name);
//             path2[strlen(path2)] = '\0';
//             printf("%s\n", path2);
//             stat(path2, &path_stat);
//             if (S_ISDIR(path_stat.st_mode))
//             {
//                 copy_dir(path2, client_fd);
//             }
//             else
//             {
//                 copy_file(path2, client_fd);
//             }
//         }
//         dir_entry = readdir(dir);
//     }
//     write(client_fd,"end",sizeof("end"));
//     chdir("..");
// }

// int main()
// {
//     int server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd < 0)
//     {
//         printf("Socket creation failed\n");
//         exit(0);
//     }
//     struct sockaddr_in server_addr;
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(54678);
//     server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     int bind_status = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
//     if (bind_status < 0)
//     {
//         printf("Binding failed\n");
//         exit(0);
//     }
//     int listen_status = listen(server_fd, 10);
//     if (listen_status < 0)
//     {
//         printf("Listening failed\n");
//         exit(0);
//     }
//     struct sockaddr_in client_addr;
//     socklen_t client_addr_len = sizeof(client_addr);
//     int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
//     if (client_fd < 0)
//     {
//         printf("Accepting failed\n");
//         exit(0);
//     }
//     printf("Enter the path to the directory that you want to send\n");
//     char dir_path[100];
//     scanf("%s", dir_path);
//     copy_dir(dir_path, client_fd);
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

int send_file(char file_path2[], int client_fd)
{
    // printf("Enter the relative path of hte file to send\n");
    char file_path[100];
    // scanf("%s", file_path);
    strcpy(file_path, file_path2);

    file_path[strlen(file_path)] = '\0';
    printf("%s\n", file_path);
    struct stat file_stat;

    int file_status = stat(file_path, &file_stat);
    if (file_status < 0)
    {
        printf("File does not exist\n");
        exit(0);
    }

    if (!(file_stat.st_mode & S_IRUSR))
    {
        printf("File does not have read permission\n");
        exit(0);
    }
    int send_size = file_stat.st_size;

    char file_name[100];
    strcpy(file_name, file_path);
    file_name[strlen(file_name)] = '\0';
    int dir_or_file = 0;
    write(client_fd, &dir_or_file, sizeof(dir_or_file));
    write(client_fd, file_name, sizeof(file_name));
    write(client_fd, &send_size, sizeof(send_size));
    int number_of_bytes_sent = 0;
    int number_of_bytes_to_send = send_size;
    int file_fd = open(file_path, O_RDONLY);
    char buffer[100];
    while (number_of_bytes_sent < send_size)
    {
        int number_of_bytes_read = read(file_fd, buffer, sizeof(buffer));
        if (number_of_bytes_read < 0)
        {
            printf("Error in reading file\n");
            exit(0);
        }
        if (number_of_bytes_read == 0)
        {
            printf("File reading done\n");
            break;
        }
        int sent_bytes = write(client_fd, buffer, number_of_bytes_read);
        if (sent_bytes < 0)
        {
            printf("Error in sending file\n");
            exit(0);
        }
        else
        {
            number_of_bytes_sent += sent_bytes;
        }
    }
    printf("sending done\n");
}
int count_number_of_paths_to_send(char dir_path2[])
{
    char dir_path[100];
    strcpy(dir_path, dir_path2);
    dir_path[strlen(dir_path)] = '\0';
    int dir_or_file = 1;
    char dir_name[100];
    strcpy(dir_name, dir_path);
    dir_name[strlen(dir_name)] = '\0';
    DIR *dir = opendir(dir_path);
    struct dirent *dir_entry;
    dir_entry = readdir(dir);
    int count=1;
    while (dir_entry != NULL)
    {
        if (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0)
        {
            // check if it is a file or a directory
            struct stat path_stat;
            char path2[100];
            strcpy(path2, dir_path);
            strcat(path2, "/");
            strcat(path2, dir_entry->d_name);
            path2[strlen(path2)] = '\0';
            printf("%s\n", path2);
            stat(path2, &path_stat);
            if (S_ISDIR(path_stat.st_mode))
            {
                count+=count_number_of_paths_to_send(path2);
            }
            else
            {
                count++;
            }
        }
        dir_entry = readdir(dir);
    }
    return count;

}
int send_dir(char dir_path2[], int client_fd)
{
    char dir_path[100];
    strcpy(dir_path, dir_path2);
    dir_path[strlen(dir_path)] = '\0';
    int dir_or_file = 1;
    char dir_name[100];
    strcpy(dir_name, dir_path);
    dir_name[strlen(dir_name)] = '\0';
    write(client_fd, &dir_or_file, sizeof(dir_or_file));
    write(client_fd, dir_name, sizeof(dir_name));
    DIR *dir = opendir(dir_path);
    struct dirent *dir_entry;
    dir_entry = readdir(dir);
    while (dir_entry != NULL)
    {
        if (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0)
        {
            // check if it is a file or a directory
            struct stat path_stat;
            char path2[100];
            strcpy(path2, dir_path);
            strcat(path2, "/");
            strcat(path2, dir_entry->d_name);
            path2[strlen(path2)] = '\0';
            printf("%s\n", path2);
            stat(path2, &path_stat);
            printf("%s\n", path2);
            if (S_ISDIR(path_stat.st_mode))
            {
                send_dir(path2, client_fd);
            }
            else
            {
                send_file(path2, client_fd);
            }
        }
        dir_entry = readdir(dir);
    }
}

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        printf("Socket creation failed\n");
        exit(0);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54678);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int bind_status = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_status < 0)
    {
        printf("Binding failed\n");
        exit(0);
    }
    int listen_status = listen(server_fd, 10);
    if (listen_status < 0)
    {
        printf("Listening failed\n");
        exit(0);
    }
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
    {
        printf("Accepting failed\n");
        exit(0);
    }
    char dir_to_send[100];
    memset(dir_to_send, 0, sizeof(dir_to_send));
    printf("Enter the path of the directory to send\n");
    scanf("%s", dir_to_send);
    dir_to_send[strlen(dir_to_send)] = '\0';
    int ct=count_number_of_paths_to_send(dir_to_send);
    write(client_fd,&ct,sizeof(ct));
    send_dir(dir_to_send, client_fd);
    printf("done\n");
    

    return 0;
}