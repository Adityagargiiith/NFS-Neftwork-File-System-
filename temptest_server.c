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
#include <errno.h>

#define SERVER_PORT 52369
#define second_server_port 52370
#define MAXLINE 1024

int client_socket2;

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

    int connect_success = bind(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connect_success == -1)
    {
        perror("Error in bind() function call: ");
        exit(EXIT_FAILURE);
    }

    int listen_success = listen(client_socket, 5);
    if (listen_success == -1)
    {
        perror("Error in listen() function call: ");
        exit(EXIT_FAILURE);
    }

    int client_socket2 = accept(client_socket, NULL, NULL);
    if (client_socket2 == -1)
    {
        perror("Error in accept() function call: ");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // receive heart beat signal from client to check if it is alive with timeout
        char buffer[MAXLINE];
        memset(buffer, 0, sizeof(buffer));
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        setsockopt(client_socket2, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
        int recv_success = recv(client_socket2, buffer, MAXLINE, 0);
        if (recv_success == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                printf("Server is dead\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                perror("Error in recv() function call: ");
                exit(EXIT_FAILURE);
            }
        }
        else if (recv_success == 0)
        {
            printf("Server is dead\n");
            exit(EXIT_FAILURE);
        }
        
        printf("Server: %s\n", buffer);
        sleep(1);

    }
}

int main()
{
    pthread_t tid;
    int client_socket;
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
    
    pthread_create(&tid, NULL, handler, NULL);

    // Step 2: Creating sockaddr_in structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Step 3: Connect the client socket to server socket
    int bind_success = bind(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_success == -1)
    {
        perror("Error in bind() function call: ");
        exit(EXIT_FAILURE);
    }

    // Step 4: Accept the connection request from client
    int listen_success = listen(client_socket, 5);
    if (listen_success == -1)
    {
        perror("Error in listen() function call: ");
        exit(EXIT_FAILURE);
    }

    // Step 5: Read data from client
    client_socket2 = accept(client_socket, NULL, NULL);
    if (client_socket2 == -1)
    {
        perror("Error in accept() function call: ");
        exit(EXIT_FAILURE);
    }


    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        usleep(1000);
        int recv_success = recv(client_socket2, buffer, MAXLINE, 0);
        if (recv_success == -1)
        {
            perror("Error in recv() function call: ");
            exit(EXIT_FAILURE);
        }
        printf("Client: %s\n", buffer);

        // Step 6: Send data to client
        int send_success = send(client_socket2, message, strlen(message), 0);
        if (send_success == -1)
        {
            perror("Error in send() function call: ");
            exit(EXIT_FAILURE);
        }
        sleep(5);
    }

    // Step 7: Close socket
    close(client_socket);
}
