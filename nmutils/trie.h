#ifndef TRIE_H
#define TRIE_H

#include "../naming_server.h"
void insert_into_tree_new(char *path, int access_permission, char *ip, int port);
void init_root();
ss_info search_path_in_trie(char *path);
void print_tree(struct tree_node *curr);

extern struct tree_node *root;
#endif