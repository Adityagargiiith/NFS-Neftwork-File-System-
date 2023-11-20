#include "write.h"
int writenm(char *path, int client_soket)
{
    ss_info res=search_path_in_trie(path);
    if (res.ss_port==-1)
    {
        int to_send=FILE_NOT_FOUND;
        if(send(client_soket,&to_send,sizeof(int),0)<0)
        {
            perror("Error in send() function call: ");
            return -1;
        }
        return -1;
    }
    else
    {
        int to_send=SUCCESS;
        if(send(client_soket,&to_send,sizeof(int),0)<0)
        {
            perror("Error in send() function call: ");
            return -1;
        }
        usleep(1000);
        if(send(client_soket,&res,sizeof(ss_info),0)<0)
        {
            perror("Error in send() function call: ");
            return -1;
        }
        int status;
        if(recv(client_soket,&status,sizeof(int),0)<0)
        {
            perror("Error in recv() function call: ");
            return -1;
        }
        if(status==SUCCESS)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
}