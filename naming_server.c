#include "naming_server.h"
#include "storage_server.h"

// 1 for dir and 2 for file
struct tree_node *root;

void insert_into_tree_new(char *path, int access_permission, char *ip, int port)
{
    int ct_of_slash = 0;
    // printf("%s\n", path);
    // char *token = strtok(path, "/");
    for (int i = 0; i < strlen(path); i++)
    {
        if (path[i] == '/')
        {
            ct_of_slash++;
        }
    }
    char *path_arr[ct_of_slash];
    char *cp_of_path = (char *)malloc(sizeof(char) * strlen(path));
    strcpy(cp_of_path, path);
    char *token = strtok(cp_of_path, "/");
    int i = 0;
    while (token != NULL)
    {
        path_arr[i] = token;
        token = strtok(NULL, "/");
        i++;
    }
    tree_node_ptr temp = root;
    for (int i = 0; i < (ct_of_slash - 1); i++)
    {
        if (temp->first_child == NULL)
        {
            tree_node_ptr new_node = (tree_node_ptr)malloc(sizeof(struct tree_node));
            new_node->name = path_arr[i];
            new_node->access_permissiom = 0;
            new_node->first_child = NULL;
            new_node->next = NULL;
            new_node->dir_or_file = 1;
            temp->first_child = new_node;
            temp = new_node;
        }
        else
        {
            temp = temp->first_child;
            while (temp->next != NULL)
            {
                if (strcmp(temp->name, path_arr[i]) == 0)
                {
                    break;
                }
                temp = temp->next;
            }
            if (strcmp(temp->name, path_arr[i]) == 0)
            {
                continue;
            }
            else
            {
                tree_node_ptr new_node = (tree_node_ptr)malloc(sizeof(struct tree_node));
                new_node->name = path_arr[i];
                new_node->access_permissiom = 0;
                new_node->first_child = NULL;
                new_node->next = NULL;
                new_node->dir_or_file = 1;
                temp->next = new_node;
                temp = new_node;
            }
        }
    }
    if (temp->first_child == NULL)
    {
        tree_node_ptr new_node = (tree_node_ptr)malloc(sizeof(struct tree_node));
        new_node->name = path_arr[ct_of_slash - 1];
        new_node->access_permissiom = access_permission;
        new_node->first_child = NULL;
        new_node->next = NULL;
        new_node->dir_or_file = 2;
        new_node->ss_port = port;
        new_node->ss_ip = (char *)malloc(sizeof(char) * (strlen(ip) + 1));
        strcpy(new_node->ss_ip, ip);
        temp->first_child = new_node;
        temp = new_node;
    }
    else
    {
        temp = temp->first_child;
        while (temp->next != NULL)
        {
            if (strcmp(temp->name, path_arr[ct_of_slash - 1]) == 0)
            {
                break;
            }
            temp = temp->next;
        }
        if (strcmp(temp->name, path_arr[ct_of_slash - 1]) == 0)
        {
            temp->access_permissiom = access_permission;
            temp->ss_port = port;
            temp->ss_ip = (char *)malloc(sizeof(char) * (strlen(ip) + 1));
            strcpy(temp->ss_ip, ip);
        }
        else
        {
            tree_node_ptr new_node = (tree_node_ptr)malloc(sizeof(struct tree_node));
            new_node->name = path_arr[ct_of_slash - 1];
            new_node->access_permissiom = access_permission;
            new_node->first_child = NULL;
            new_node->next = NULL;
            new_node->dir_or_file = 2;
            new_node->ss_port = port;
            new_node->ss_ip = (char *)malloc(sizeof(char) * (strlen(ip) + 1));
            strcpy(new_node->ss_ip, ip);
            temp->next = new_node;
            temp = new_node;
        }
    }
}
void init_root()
{
    root = (struct tree_node *)malloc(sizeof(struct tree_node));
    root->access_permissiom = 0;
    root->dir_or_file = 1;
    root->first_child = NULL;
    root->name = (char *)malloc(sizeof(char) * (strlen("/") + 1));
    strcpy(root->name, "/");
    root->name[strlen("/")] = '\0';
    root->next = NULL;
    root->ss_ip = NULL;
    root->ss_port = 0;
}

ss_info search_path_in_trie(char *path)
{
    ss_info ret_answer;
    ret_answer.ss_ip = NULL;
    ret_answer.ss_port = -1;
    int ct_of_slash = 0;
    for (int i = 0; i < strlen(path); i++)
    {
        if (path[i] == '/')
        {
            ct_of_slash++;
        }
    }
    char *path_levels_arr[ct_of_slash];
    char *copy_of_path = (char *)malloc(sizeof(char) * strlen(path));
    strcpy(copy_of_path, path);
    char *token = strtok(copy_of_path, "/");
    int i = 0;
    while (token != NULL)
    {
        path_levels_arr[i] = token;
        token = strtok(NULL, "/");
        i++;
    }
    tree_node_ptr temp = root;
    for (int i = 0; i < (ct_of_slash - 1); i++)
    {
        if (temp->first_child == NULL)
        {
            // return 0;
            // here we have to return the error code of path not found
            return ret_answer;
        }
        else
        {
            temp = temp->first_child;
            while (temp->next != NULL)
            {
                if (strcmp(temp->name, path_levels_arr[i]) == 0)
                {
                    break;
                }
                temp = temp->next;
            }
            if (strcmp(temp->name, path_levels_arr[i]) == 0)
            {
                continue;
            }
            else
            {
                // return 0;
                // here we have to return the error code of path not found
                return ret_answer;
            }
        }
    }
    if (temp->first_child == NULL)
    {
        // return 0;
        // here we have to return the error code of path not found
        return ret_answer;
    }
    else
    {
        temp = temp->first_child;
        while (temp->next != NULL)
        {
            if (strcmp(temp->name, path_levels_arr[ct_of_slash - 1]) == 0)
            {
                if (temp->access_permissiom)
                {
                    // return 1;
                    ret_answer.ss_ip = (char *)malloc(sizeof(char) * (strlen(temp->ss_ip) + 1));
                    strcpy(ret_answer.ss_ip, temp->ss_ip);
                    ret_answer.ss_port = temp->ss_port;
                    return ret_answer;
                }
                else
                {
                    // return -1;
                    // here we have to return the error code of missing permission
                    return ret_answer;
                }
            }
            temp = temp->next;
        }
        if (strcmp(temp->name, path_levels_arr[ct_of_slash - 1]) == 0)
        {
            if (temp->access_permissiom)
            {
                // return 1;
                ret_answer.ss_ip = (char *)malloc(sizeof(char) * (strlen(temp->ss_ip) + 1));
                strcpy(ret_answer.ss_ip, temp->ss_ip);
                ret_answer.ss_port = temp->ss_port;
                return ret_answer;
            }
            else
            {
                // return -1;
                // here we have to return the error code of missing permission
                return ret_answer;
            }
        }
        else
        {
            // return 0;
            // here we have to return the error code of path not found
            return ret_answer;
        }
    }
}
void print_tree(struct tree_node *curr)
{
    // printf("Yaha\n");
    if (curr == NULL)
    {
        return;
    }
    else
    {
        printf("%s ", curr->name);
        printf("Port %d ip %s\n", curr->ss_port, curr->ss_ip);
        print_tree(curr->first_child);
        print_tree(curr->next);
    }
}

void *ss_init_thread(void *)
{
    // initialize_all_ss();
    init_root();
    int port_number = 5572;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Error in socket() function call: ");
        exit(1);
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    server_address.sin_port = port_number;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int bind_success = bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_success == -1)
    {
        perror("Error in bind() function call: ");
        exit(1);
    }
    while (1)
    {

        int listen_success = listen(sock, 10);
        if (listen_success == -1)
        {
            perror("Error in listen() function call: ");
            exit(1);
        }

        int client_socket;
        struct sockaddr_in client_address;
        int client_address_length = sizeof(client_address);
        client_socket = accept(sock, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1)
        {
            perror("Error in accept() function call: ");
            exit(1);
        }
        printf("Storage server connected\n");
        struct data_of_ss initial_data_of_ss;
        memset(&initial_data_of_ss, 0, sizeof(initial_data_of_ss));
        recv(client_socket, &initial_data_of_ss, sizeof(initial_data_of_ss), 0);
        // printf("Here1\n");
        for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
        {
            insert_into_tree_new(initial_data_of_ss.paths[i].path, initial_data_of_ss.paths[i].permissions, "127.0.0.1", initial_data_of_ss.port_number);
        }

        printf("Here2\n");
        // print_tree(root);
        // ss_info ans=search_path_in_trie("/a/b");
        // if (ans.ss_port==-1)
        // {
        //     /* code */
        //     printf("Not found\n");
        // }
        // else
        // {
        //     /* code */
        //     printf("Found\n");
        //     printf("%d %s\n",ans.ss_port,ans.ss_ip);

        // }
    }
}

void *client_req_handler(void *arg)
{
    pthread_detach(pthread_self());
    int client_socket = *((int *)arg);
    char *msg = (char *)malloc(sizeof(char) * 100);
    if (recv(client_socket, msg, 100, 0) == -1)
    {
        perror("Error in recv() function call: ");
        exit(1);
    }
    char *token = strtok(msg, " ");
    if (strcmp(token, "makedir") == 0)
    {
        char *name_of_dir = strtok(NULL, " ");
        char *path = strtok(NULL, " ");

        ss_info ans = search_path_in_trie(path);

        int sock_ss = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_ss == -1)
        {
            perror("Error in socket() function call: ");
            exit(1);
        }
        struct sockaddr_in server_address_ss;
        memset(&server_address_ss, 0, sizeof(server_address_ss));
        server_address_ss.sin_family = AF_INET;
        server_address_ss.sin_port = htons(ans.ss_port);
        server_address_ss.sin_addr.s_addr = inet_addr(ans.ss_ip);

        int connect_success = connect(sock_ss, (struct sockaddr *)&server_address_ss, sizeof(server_address_ss));
        if (connect_success == -1)
        {
            perror("Error in connect() function call: ");
            exit(1);
        }

        char *msg_to_ss = (char *)malloc(sizeof(char) * 100);
        strcpy(msg_to_ss, "makedir ");
        strcat(msg_to_ss, name_of_dir);
        strcat(msg_to_ss, " ");
        strcat(msg_to_ss, path);

        if (send(sock_ss, msg_to_ss, strlen(msg_to_ss), 0) == -1)
        {
            perror("Error in send() function call: ");
            exit(1);
        }

        int status;
        if (recv(sock_ss, &status, sizeof(status), 0) == -1)
        {
            perror("Error in recv() function call: ");
            exit(1);
        }

        close(sock_ss);

        char *new_path = (char *)malloc(sizeof(char) * (strlen(path) + strlen(name_of_dir) + 2));
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, name_of_dir);

        if (status == SUCCESS)
        {
            insert_into_tree_new(new_path, 1, ans.ss_ip, ans.ss_port);
        }
        else
        {
            printf("Error in making directory\n");
        }

        if (send(client_socket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            exit(1);
        }

        print_tree(root);
    }

    close(client_socket);
    return NULL;
}

void *client_thread(void *)
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

    server_address.sin_port = htons(CLIENT_PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int bind_success = bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_success == -1)
    {
        perror("Error in bind() function call: ");
        exit(1);
    }

    int listen_success = listen(sock, 15);
    if (listen_success == -1)
    {
        perror("Error in listen() function call: ");
        exit(1);
    }

    while (1)
    {
        int client_socket;
        struct sockaddr_in client_address;
        int client_address_length = sizeof(client_address);
        client_socket = accept(sock, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1)
        {
            perror("Error in accept() function call: ");
            exit(1);
        }
        pthread_t client_req_handler_thread_id;
        pthread_create(&client_req_handler_thread_id, NULL, client_req_handler, (void *)&client_socket);
    }
}

int main()
{
    pthread_t ss_init_thread_id;
    pthread_create(&ss_init_thread_id, NULL, ss_init_thread, NULL);
    pthread_t client_thread_id;
    pthread_create(&client_thread_id, NULL, client_thread, NULL);
    pthread_join(ss_init_thread_id, NULL);
    return 0;
}