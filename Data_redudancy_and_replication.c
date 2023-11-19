
//storage_server.c

void send_file_to_ss(char* filename, char* ss_ip, int ss_port) {
    // Open the file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Connect to the other SS
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ss_addr;
    ss_addr.sin_family = AF_INET;
    ss_addr.sin_port = htons(ss_port);
    ss_addr.sin_addr.s_addr = inet_addr(ss_ip);
    if (connect(sock, (struct sockaddr*)&ss_addr, sizeof(ss_addr)) < 0) {
        perror("Failed to connect to other SS");
        return;
    }

    // Send the file
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    // Close the file and the socket
    fclose(file);
    close(sock);
}

//function call wherever needed
end_file_to_ss(filename, "other_ss_ip", other_ss_port);
send_file_to_ss(filename, "another_ss_ip", another_ss_port);

//naming_server.c

void retrieve_file_from_ss(char* filename, char* ss_ip, int ss_port) {
    // Connect to the SS
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ss_addr;
    ss_addr.sin_family = AF_INET;
    ss_addr.sin_port = htons(ss_port);
    ss_addr.sin_addr.s_addr = inet_addr(ss_ip);
    if (connect(sock, (struct sockaddr*)&ss_addr, sizeof(ss_addr)) < 0) {
        perror("Failed to connect to SS");
        return;
    }

    // Send a request for the file
    send(sock, filename, strlen(filename), 0);

    // Receive the file and write it to a local file
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    char buffer[1024];
    ssize_t bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }

    // Close the file and the socket
    fclose(file);
    close(sock);
}

// When a file is requested and the primary SS is down...
//call this function
retrieve_file_from_ss(filename, "replicated_ss_ip", replicated_ss_port);