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
// #include <sys/stat.h>
// // int server_fd;
// int recieve_file(char file_name[],int server_fd)
// {
//     printf("Here\n");
//     int file_fd = open(file_name, O_RDONLY | O_WRONLY | O_CREAT, 0666);
//     int number_of_bytes_to_recieve;
//     read(server_fd, &number_of_bytes_to_recieve, sizeof(number_of_bytes_to_recieve));
//     // printf("Number of bytes to recieve is %d\n", number_of_bytes_to_recieve);
//     char buffer[100];
//     memset(buffer, 0, sizeof(buffer));
//     int number_of_bytes_recieved = 0;
//     while (1)
//     {
//         if (number_of_bytes_recieved >= number_of_bytes_to_recieve)
//         {
//             break;
//         }
//         int number_of_bytes_recieved_now = read(server_fd, buffer, sizeof(buffer));
//         if (number_of_bytes_recieved_now == 0)
//         {
//             /* code */
//             break;
//         }

//         write(file_fd, buffer, number_of_bytes_recieved_now);
//         number_of_bytes_recieved += number_of_bytes_recieved_now;
//     }

//     printf("File recieved\n");
//     return 0;
// }
// int receive_dir(char dir_name[], int server_fd)
// {
//     printf("%s\n",dir_name);
//     int err_check=mkdir(dir_name, 0777);
//     if (err_check<0)
//     {
//         printf("Directory not created\n");
//         return -1;
//     }

//     char oldpath[512];
//     getcwd(oldpath, sizeof(oldpath));
//     chdir(dir_name);
//     while (1)
//     {
//         char buffer[100];
//         read(server_fd, buffer, sizeof(buffer));
//         if (strcmp(buffer, "dir") == 0)
//         {
//             read(server_fd, buffer, sizeof(buffer));
//             receive_dir(buffer, server_fd);
//             chdir("..");
//             continue;
//         }
//         else if(strcmp(buffer, "file") == 0)
//         {
//             read(server_fd, buffer, sizeof(buffer));
//             recieve_file(buffer, server_fd);
//         }
//         else
//         {
//             printf("End of this directory\n");
//             break;
//         }
//     }
//     chdir(oldpath);
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
//     int connect_status = connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
//     if (connect_status < 0)
//     {
//         printf("Connection failed\n");
//         exit(0);
//     }
//     printf("Connection done\n");
//     // read(server_fd, "dir", sizeof("dir"));
//     char buffer[1000];
//     // memset(buffer,)
//     memset(buffer, 0, sizeof(buffer));
//     read(server_fd, buffer, sizeof(buffer));
//     //    printf("%s\n",buffer);
//     if (strcmp("dir", buffer) == 0)
//     {
//         printf("Dir Comming\n");
//     }
//     char buff2[1000];
//     memset(buff2,0,sizeof(buff2));
//     read(server_fd, buff2, sizeof(buffer));
//     printf("%s\n",buff2);
//     receive_dir(buff2, server_fd);
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
#include <fcntl.h>
#include <sys/stat.h>

int rec_file(int server_fd)
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
        int left=number_of_bytes_to_recieve-number_of_bytes_recieved;
        int size_of_buffer;
        if (left>100)
        {
            size_of_buffer=100;
        }
        else
        {
            size_of_buffer=left;
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
    return 0;
}

int rec_dir(int server_fd)
{
    char dir_name[100];
    memset(dir_name, 0, sizeof(dir_name));
    read(server_fd, dir_name, sizeof(dir_name));
    mkdir(dir_name, 0777);
    return 0;
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
    int connect_status = connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_status < 0)
    {
        printf("Connection failed\n");
        exit(0);
    }
    printf("Connection done\n");
    int number_of_paths;
    read(server_fd, &number_of_paths, sizeof(number_of_paths));
    // while (1)
    // {
    //     int dir_or_file;
    //     dir_or_file = 0;
    //     read(server_fd, &dir_or_file, sizeof(dir_or_file));
    //     if (dir_or_file == 0)
    //     {
    //         rec_file(server_fd);
    //     }
    //     else if (dir_or_file == 1)
    //     {
    //         rec_dir(server_fd);
    //     }
    //     read(server_fd, &dir_or_file, sizeof(dir_or_file));
    //     if (dir_or_file == 0)
    //     {
    //         rec_file(server_fd);
    //     }
    //     else if (dir_or_file == 1)
    //     {
    //         rec_dir(server_fd);
    //     }
    //     // dir_or_file = 0;
    //     memset(&dir_or_file, 0, sizeof(dir_or_file));
    //     read(server_fd, &dir_or_file, sizeof(dir_or_file));
    //     if (dir_or_file == 0)
    //     {
    //         printf("Last one is a file\n");
    //         rec_file(server_fd);
    //     }
    //     else if (dir_or_file == 1)
    //     {
    //         printf("Here\n");
    //         rec_dir(server_fd);
    //     }
    //     read(server_fd, &dir_or_file, sizeof(dir_or_file));
    //     if (dir_or_file == 0)
    //     {
    //         printf("Last one is a file\n");
    //         rec_file(server_fd);
    //     }
    //     else if (dir_or_file == 1)
    //     {
    //         printf("Here\n");
    //         rec_dir(server_fd);
    //     }
    // }
    printf("Number of paths is %d\n", number_of_paths);
    for (int i = 0; i < number_of_paths; i++)
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
    

    printf("File recieved\n");
    return 0;
}