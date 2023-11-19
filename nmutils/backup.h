#ifndef __BACKUPNMUTILS_H
#define __BACKUPNMUTILS_H

// void *backup_dir(char *dir_path,)
#include "../naming_server.h"
struct ss_new{
    int ss_port;
    char ss_ip[20];
    int s2s_port;
    int dir_or_file;
};
typedef struct ss_new ss_new;
int backupdir(ss_new sender_info1, ss_new receiver_info1, char *dir_path);
int backup_file(ss_new sender_info1, ss_new reciever_info1, char *file_path);

#endif