#include "storage_sesrver.h"

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

    int number_of_accessible_paths;
    printf("Enter number of accessible paths: ");
    scanf("%d", &number_of_accessible_paths);

    send(sock, &number_of_accessible_paths, sizeof(number_of_accessible_paths), 0);
    for (int i = 0; i < number_of_accessible_paths; i++)
    {
        char path[4096];
        printf("Enter path %d: ", i + 1);
        scanf("%s", path);
        printf("%s\n", path);
        send(sock, path, strlen(path), 0);
        // send(sock, path, strlen(path),);
    }

    

    return 0;
}