#include "naming_server.h"

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

void add_ss(char *ss_ip, int ss_port, char *path_arr[], int number_of_paths)
{
    all_ss_info.number_of_ss++;
    ss_info *ss_arr=all_ss_info.arr_of_all_ss;
    ss_arr=(ss_info *)realloc(ss_arr,sizeof(ss_info)*all_ss_info.number_of_ss);
    ss_info ss_new=ss_arr[all_ss_info.number_of_ss-1];
    ss_new.ss_ip=ss_ip;
    ss_new.ss_port=ss_port;
    struct tree_node root_for_ss_new;
    root_for_ss_new.name = "/";
    root_for_ss_new.access_permissiom=0;
    root_for_ss_new.first_child=NULL;
    root_for_ss_new.next=NULL;
    ss_new.root_for_acess=&root_for_ss_new;
    for (int i = 0; i < number_of_paths; i++)
    {
        insert_into_tree(&root_for_ss_new, path_arr[i], 1);
    }
    ss_new.root_for_acess=&root_for_ss_new;
    ss_arr[all_ss_info.number_of_ss-1]=ss_new;
    print_tree(ss_arr[all_ss_info.number_of_ss-1].root_for_acess);

}

int main()
{
    
    
    return 0;
}