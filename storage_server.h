#ifndef STORAGE_SERVER_H
#define STORAGE_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "errorcode.h"
#include <semaphore.h>
#include "ssutils/makedir.h"
#include "ssutils/deletedir.h"
#include "ssutils/makefile.h"
#include "ssutils/deletefile.h"
#include "nmutils/trie.h"
#include "ssutils/copyfile.h"
#include "nmutils/copydir.h"

#define MAX_PATHS 1000
#define NM_INIT_PORT 5572
#define NM_CONN_IP "127.0.0.1"
#define SS_IP "127.0.0.1"

struct details_of_path
{
    char path[400];
    int permissions;
    int dir_or_file;
};

struct data_of_ss
{
    int port_number;
    int number_of_paths;
    int s2s_port;
    int client_port;
    struct details_of_path paths[MAX_PATHS];
};

#endif