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
    char *ss_ip;
    int ss_port;
};

typedef struct tree_node tree_node;
typedef struct tree_node *tree_node_ptr;
typedef struct tree_node *root_ptr;

struct ss_info{
    char *ss_ip;
    int ss_port;
};


typedef struct ss_info ss_info;


tree_node_ptr insert_into_tree(root_ptr root,char *path,int access_permission);

void delete_from_tree(root_ptr root,char *path);

void remove_permission(root_ptr root,char *path);

void print_tree(struct tree_node* curr);

ss_info search_path_in_trie(char *path);



void insert_into_tree_new(char *path, int access_permission, char *ip, int port);
extern struct tree_node *root;
void init_root();

#endif