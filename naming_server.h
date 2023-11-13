#ifndef NAMING_SERVER_H
#define NAMING_SERVER_H




#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<pthread.h>



struct tree_node{
    char *name;
    int number_of_children;
    struct tree_node *first_child;
    struct tree_node *next;
    int dir_or_file;
    int access_permissiom;
};

typedef struct tree_node tree_node;
typedef struct tree_node *tree_node_ptr;
typedef struct tree_node *root_ptr;

struct ss_info{
    char *ss_ip;
    int ss_port;
    root_ptr root_for_acess;
};



typedef struct ss_info ss_info;

struct all_ss{
    int number_of_ss;
    ss_info *arr_of_all_ss;
};

tree_node_ptr insert_into_tree(root_ptr root,char *path,int access_permission);

void delete_from_tree(root_ptr root,char *path);

void remove_permission(root_ptr root,char *path);

void add_ss(char *ss_ip,int ss_port);

void print_tree(root_ptr root);

struct all_ss all_ss_info;

int search_path_in_trie(root_ptr root, char *path);


ss_info search_for_path_in_all_ss(char *path);

// #define CLIENT_PORT 51234

#endif