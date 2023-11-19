#include "write.h"

void writess(char *path, char *data, int client_soket)
{
    char *cpy_path = (char *)malloc(sizeof(char) * strlen(path));
    strcpy(cpy_path, ".");
    strcat(cpy_path, path);
    cpy_path[strlen(path) + 1] = '\0';
    path = cpy_path;


    struct stat path_stat;
    int path_exists = stat(path, &path_stat);
    if (path_exists == -1)
    {
        int to_send = FILE_NOT_FOUND;
        if (send(client_soket, &to_send, sizeof(int), 0) < 0)
        {
            perror("Error in send() function call: ");
            return;
        }
        return;
    }
    else
    {
        int fd = open(path, O_WRONLY | O_TRUNC);
        if (fd == -1)
        {
            perror("Error in open() function call: ");
            int to_return = 125;
            if (send(client_soket, &to_return, sizeof(int), 0) < 0)
            {
                perror("Error in send() function call: ");
                return;
            }
            return;
        }
        int number_of_bytes_written = write(fd, data, strlen(data));
        if (number_of_bytes_written == -1)
        {
            perror("Error in write() function call: ");
            int to_return = 125;
            if (send(client_soket, &to_return, sizeof(int), 0) < 0)
            {
                perror("Error in send() function call: ");
                return;
            }
            return;
        }
        int to_return = SUCCESS;
        if (send(client_soket, &to_return, sizeof(int), 0) < 0)
        {
            perror("Error in send() function call: ");
            return;
        }
        return;
    }
    
}