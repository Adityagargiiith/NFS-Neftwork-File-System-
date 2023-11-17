#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

// int client_fd;
int copy_file(char file_path, int client_fd)
{
    write(client_fd, "file", sizeof("file"));
    write(client_fd, file_path, sizeof(file_path));
    int file_fd = open(file_path, O_RDONLY | O_WRONLY | O_CREAT, 0666);
    
}
void copy_dir(char dir_path[], int client_fd)
{
    write(client_fd, "dir", sizeof("dir"));
    write(client_fd, dir_path, sizeof(dir_path));
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
            stat(path2, &path_stat);
            if (S_ISDIR(path_stat.st_mode))
            {
                copy_dir(path2, client_fd);
            }
            else
            {
                copy_file(path2, client_fd);
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
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
    {
        printf("Accepting failed\n");
        exit(0);
    }
    printf("Enter the path to the directory that you want to send\n");
    char dir_path[100];
    scanf("%s", dir_path);

    return 0;
}