#include "trie.h"

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