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
    initial_data_of_ss.port_number=6677;   // hard coded for time being . Will change this later
    initial_data_of_ss.paths[initial_data_of_ss.number_of_paths];
    for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
    {
        initial_data_of_ss.paths[i].permissions=1;  // hard coded for time being . Will change this later
        printf("Enter path %d: ", i + 1);
        scanf("%s", initial_data_of_ss.paths[i].path);
        initial_data_of_ss.paths[i].path[strlen(initial_data_of_ss.paths[i].path)]='\0';
        printf("%s\n", initial_data_of_ss.paths[i].path);
    }
    for (int i = initial_data_of_ss.number_of_paths; i < MAX_PATHS; i++)
    {
        initial_data_of_ss.paths[i].permissions=0;
        initial_data_of_ss.paths[i].path[0]='\0';
    }
    
    int err_check=send(sock, &initial_data_of_ss, sizeof(initial_data_of_ss), 0);
    printf("Data sent to naming server\n");
    // close(sock);

    return 0;
}