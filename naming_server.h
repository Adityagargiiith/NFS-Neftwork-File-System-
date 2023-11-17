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
#include "errorcode.h"
#include "nmutils/makedir.h"
#include "nmutils/deletedir.h"
#include "nmutils/makefile.h"
#include "nmutils/deletefile.h"
#include "nmutils/trie.h"

#define CLIENT_PORT 51234
#define SS_PORT 5572





// tree_node_ptr insert_into_tree(root_ptr root,char *path,int access_permission);

// // void delete_from_tree(root_ptr root,char *path);

// // void remove_permission(root_ptr root,char *path);

// void print_tree(struct tree_node* curr);

// ss_info search_path_in_trie(char *path);



// void insert_into_tree_new(char *path, int access_permission, char *ip, int port);
// extern struct tree_node *root;
// void init_root();

#define IP "127.0.0.1"

#endif