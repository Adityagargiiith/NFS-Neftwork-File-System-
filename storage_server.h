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
#include "ssutils/makedir.h"
#include "ssutils/deletedir.h"
#include "ssutils/makefile.h"
#include "ssutils/deletefile.h"

#define MAX_PATHS 1000

struct details_of_path
{
    char path[400];
    int permissions; // read ,write , exxecute
};

struct data_of_ss
{
    int port_number;
    int number_of_paths;
    struct details_of_path paths[MAX_PATHS];
};

#endif