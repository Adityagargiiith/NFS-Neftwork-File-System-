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
#include "nmutils/copyfile.h"
#include "nmutils/copydir.h"
#include "nmutils/read.h"
#include "nmutils/write.h"

#define CLIENT_PORT 51234
#define SS_PORT 5572


#define IP "127.0.0.1"

#endif