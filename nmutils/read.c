#include "read.h"

void readnm(char *path, int client_soket)
{
    ss_info ans =search_path_in_trie(path);
    if (ans.ss_port==-1)
    {
        int status = FILE_NOT_FOUND;
        if (send(client_soket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
        return;
    }
    else
    {
        int status=SUCCESS;
        if (send(client_soket, &status, sizeof(status), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
        if (send(client_soket, &ans, sizeof(ans), 0) == -1)
        {
            perror("Error in send() function call: ");
            return;
        }
    }
}