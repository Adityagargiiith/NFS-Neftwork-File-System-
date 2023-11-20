#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <poll.h>

#define SERVER_PORT 52369
#define second_server_port 52370
#define MAXLINE 1024
int client_socket;

void *handler(void *)
{
    int client_socket= socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error in socket() function call: ");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(second_server_port);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connect_success = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connect_success == -1)
    {
        perror("Error in connect() function call: ");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        //send heart beat signal to server to check if it is alive
        char *message = "ping";
        int send_success = send(client_socket, message, strlen(message), 0);
        if (send_success == -1)
        {
            perror("Error in send() function call: ");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

}

int main()
{
    pthread_t tid;
    struct sockaddr_in server_address;
    char buffer[MAXLINE];
    char *message = "Hello Server";

    // Step 1: Creating socket file descriptor
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error in socket() function call: ");
        exit(EXIT_FAILURE);
    }

    // Step 2: Creating sockaddr_in structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Step 3: Connect the client socket to server socket
    int connect_success = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connect_success == -1)
    {
        perror("Error in connect() function call: ");
        exit(EXIT_FAILURE);
    }

    pthread_create(&tid, NULL, handler, NULL);

    // Step 4: Send data to server
    while (1)
    {
        int send_success = send(client_socket, message, strlen(message), 0);
        if (send_success == -1)
        {
            perror("Error in send() function call: ");
            exit(EXIT_FAILURE);
        }

        // Step 5: Read data from server
        memset(buffer, 0, sizeof(buffer));
        int read_success = recv(client_socket, buffer, MAXLINE, 0);
        if (read_success == -1)
        {
            perror("Error in read() function call: ");
            exit(EXIT_FAILURE);
        }
        printf("Server: %s\n", buffer);
        sleep(5);
    }

    // Step 6: Close socket
    close(client_socket);
    return 0;
}