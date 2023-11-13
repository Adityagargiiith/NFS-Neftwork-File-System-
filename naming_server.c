#include "naming_server.h"
#include "storage_server.h"

// 1 for dir and 2 for file

tree_node_ptr insert_into_tree(root_ptr root, char *path, int access_permission)
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
    char *cp_of_path=(char *)malloc(sizeof(char)*strlen(path));
    strcpy(cp_of_path,path);
    char *token = strtok(cp_of_path, "/");
    int i = 0;
    while (token != NULL)
    {
        path_arr[i] = token;
        token = strtok(NULL, "/");
        i++;
    }
    tree_node_ptr temp = root;
    for (int i = 0; i < (ct_of_slash-1); i++)
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
        new_node->name = path_arr[ct_of_slash-1];
        new_node->access_permissiom = access_permission;
        new_node->first_child = NULL;
        new_node->next = NULL;
        new_node->dir_or_file = 2;
        temp->first_child = new_node;
        temp = new_node;
    }
    else
    {
        temp = temp->first_child;
        while (temp->next != NULL)
        {
            if (strcmp(temp->name, path_arr[ct_of_slash-1]) == 0)
            {
                break;
            }
            temp = temp->next;
        }
        if (strcmp(temp->name, path_arr[ct_of_slash-1]) == 0)
        {
            temp->access_permissiom = access_permission;
        }
        else
        {
            tree_node_ptr new_node = (tree_node_ptr)malloc(sizeof(struct tree_node));
            new_node->name = path_arr[ct_of_slash-1];
            new_node->access_permissiom = access_permission;
            new_node->first_child = NULL;
            new_node->next = NULL;
            new_node->dir_or_file = 2;
            temp->next = new_node;
            temp = new_node;
        }
    }
    // print_tree(root);      
}


void print_tree(root_ptr root)
{
    if (root == NULL)
    {
        // printf("NULL\n");
        return;
    }
    printf("%s\n", root->name);
    print_tree(root->first_child);
    print_tree(root->next);
}

void add_ss(char *ss_ip, int ss_port)
{

    all_ss_info.number_of_ss++;

    all_ss_info.arr_of_all_ss = (ss_info *)realloc(all_ss_info.arr_of_all_ss, sizeof(ss_info) * all_ss_info.number_of_ss);

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].ss_ip = (char *)malloc(sizeof(char) * strlen(ss_ip));
    strcpy(all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].ss_ip, ss_ip);

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].ss_port = ss_port;

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess = (root_ptr)malloc(sizeof(struct tree_node));

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->name = "/";

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->access_permissiom = 0;

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->first_child = NULL;

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->next = NULL;
 
    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->dir_or_file = 1;
   
}


void add_ss_a(struct data_of_ss initial_data_of_ss)
{
    all_ss_info.number_of_ss++;

    all_ss_info.arr_of_all_ss = (ss_info *)realloc(all_ss_info.arr_of_all_ss, sizeof(ss_info) * all_ss_info.number_of_ss);

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].ss_ip = (char *)malloc(sizeof(char) * strlen("127.0.0.1"));

    strcpy(all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].ss_ip, "127.0.0.1");

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].ss_port = initial_data_of_ss.port_number;

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess = (root_ptr)malloc(sizeof(struct tree_node));

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->name = "/";

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->access_permissiom = 0;

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->first_child = NULL;

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->next = NULL;

    all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess->dir_or_file = 1;
    // printf("Hi Hello\n");
    for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
    {
        // printf("%s\n", initial_data_of_ss.paths[i].path);
        initial_data_of_ss.paths[i].path[strlen(initial_data_of_ss.paths[i].path)] = '\0';
        insert_into_tree(all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess, initial_data_of_ss.paths[i].path, initial_data_of_ss.paths[i].permissions);
    }
    print_tree(all_ss_info.arr_of_all_ss[all_ss_info.number_of_ss - 1].root_for_acess);

}

void initialize_all_ss()
{
    all_ss_info.number_of_ss=0;
    all_ss_info.arr_of_all_ss=NULL;
}


void add_addr_to_ss(char * path,int ss_number)
{
    insert_into_tree(all_ss_info.arr_of_all_ss[ss_number].root_for_acess,path,1);

}

void *ss_init_thread(void *)
{
    initialize_all_ss();
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
        struct data_of_ss initial_data_of_ss;
        memset(&initial_data_of_ss,0,sizeof(initial_data_of_ss));
        recv(client_socket, &initial_data_of_ss, sizeof(initial_data_of_ss), 0);
        printf("Here1\n");
        // add_ss("127.0.0.1",initial_data_of_ss.port_number);
        // printf("Here2\n");
        // for(int i=0;i<initial_data_of_ss.number_of_paths;i++)
        // {
        //     add_addr_to_ss(initial_data_of_ss.paths[i].path,all_ss_info.number_of_ss-1);
        // }
        // printf("%d\n", initial_data_of_ss.number_of_paths);
        for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
        {
            
            printf("%s\n", initial_data_of_ss.paths[i].path);
        }
        
        add_ss_a(initial_data_of_ss);
        printf("Here3\n");
    }
}


int main()
{
    // initialize_all_ss();
    // int port_number=5572;
    // int sock=socket(AF_INET,SOCK_STREAM,0);
    // if(sock==-1)
    // {
    //     perror("Error in socket() function call: ");
    //     exit(1);
    // }
    // struct sockaddr_in server_address;
    // memset(&server_address,0,sizeof(server_address));
    // server_address.sin_family=AF_INET;

    // server_address.sin_port=port_number;
    // server_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    // int bind_success=bind(sock,(struct sockaddr *)&server_address,sizeof(server_address));
    // if(bind_success==-1)
    // {
    //     perror("Error in bind() function call: ");
    //     exit(1);
    // }

    // int listen_success=listen(sock,10);


    // if(listen_success==-1)
    // {
    //     perror("Error in listen() function call: ");
    //     exit(1);
    // }


    // int client_socket;
    // struct sockaddr_in client_address;
    // int client_address_length=sizeof(client_address);
    // client_socket=accept(sock,(struct sockaddr *)&client_address,&client_address_length);
    // if(client_socket==-1)
    // {
    //     perror("Error in accept() function call: ");
    //     exit(1);
    // }
    // int number_of_accessible_paths;
    // recv(client_socket,&number_of_accessible_paths,sizeof(number_of_accessible_paths),0);
    // printf("%d\n",number_of_accessible_paths);

    // add_ss("127.0.0.1",85);
    // // printf("Here\n");
    // // print_tree(all_ss_info.arr_of_all_ss[0].root_for_acess);
    // for(int i=0;i<number_of_accessible_paths;i++)
    // {
    //     char path[4096];
    //     memset(path,0,sizeof(path));
    //     recv(client_socket,path,sizeof(path),0);
    //     path[strlen(path)]='\0';
    //     printf("%s\n",path);
    //     // print_tree(all_ss_info.arr_of_all_ss[0].root_for_acess);
    //     add_addr_to_ss(path,all_ss_info.number_of_ss-1);
    // }
    // print_tree(all_ss_info.arr_of_all_ss[0].root_for_acess);

    pthread_t ss_init_thread_id;
    pthread_create(&ss_init_thread_id,NULL,ss_init_thread,NULL);
    pthread_join(ss_init_thread_id,NULL);
    return 0;
}