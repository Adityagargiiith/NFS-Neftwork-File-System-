#ifndef TRIE_H
#define TRIE_H

#include "../naming_server.h"

struct tree_node
{
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

struct ss_info
{
    char *ss_ip;
    int ss_port;
};

typedef struct ss_info ss_info;

void insert_into_tree_new(char *path, int access_permission, char *ip, int port);
void init_root();
ss_info search_path_in_trie(char *path);
void print_tree(struct tree_node *curr);
int delete_from_trie(char *path);
void delete_subtree(struct tree_node *curr);
extern struct tree_node *root;
#endif