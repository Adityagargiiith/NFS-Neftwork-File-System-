#include "naming_server.h"
#include "client.h"
#include "errocode.h"

void help(char *command)
{
    char* cpy_command = (char*)malloc(strlen(command) + 1);
    strcpy(cpy_command, command);
    cpy_command[strlen(command)] = '\0';
    int ctr = 0;
    char* token = strtok(cpy_command, " ");
    while (token != NULL)
    {
        ctr++;
        token = strtok(NULL, " ");
    }
    if (ctr > 1)
    {
        printf("Usage: help\n");
        return;
    }
    printf("Commands:\n");
    printf("1. makedir <dirname> <path> : Create a directory with name <dirname> at <path>\n");
    
    printf("2. read <filename(with path)> : Read the file <filename> from <path>\n");
    printf("3. write <filename(with path)> <data> : Write <data> to the file <filename> at <path>\n");
    printf("4. deletefile <filename(with path)> : Delete the file <filename> from <path>\n");

}

int main()
{
    printf("------------------Welcome to the NFS------------------\n");
    printf("Use help to get a list of commands and their usage\n");

    int pid = fork();
    if (pid == -1)
    {
        printf("Error in forking\n");
        exit(1);
    }
    else if (pid == 0)
    {
        char input[100];
        memset(input, 0, 100);
        while (1)
        {
            printf(">> ");
            fgets(input, 100, stdin);
            input[strlen(input) - 1] = '\0';

            char *cpy_command = (char *)malloc(strlen(input) + 1);
            strcpy(cpy_command, input);
            cpy_command[strlen(input)] = '\0';

            char *command = strtok(cpy_command, " ");
            if (strcmp(command, "help") == 0)
            {
                // printf("Commands:\n");
                // printf("1. create <filename> <size>\n");
                // printf("2. read <filename> <offset> <size>\n");
                // printf("3. write <filename> <offset> <size> <data>\n");
                // printf("4. delete <filename>\n");
                // printf("5. exit\n");
                help(input);
            }
            else if (strcmp(command, "create") == 0)
            {
                char *filename = strtok(NULL, " ");
                char *size = strtok(NULL, " ");
                if (filename == NULL || size == NULL)
                {
                    printf("Usage: create <filename> <size>\n");
                    continue;
                }
                create(filename, atoi(size));
            }
            else if (strcmp(command, "read") == 0)
            {
                char *filename = strtok(NULL, " ");
                char *offset = strtok(NULL, " ");
                char *size = strtok(NULL, " ");
                if (filename == NULL || offset == NULL || size == NULL)
                {
                    printf("Usage: read <filename> <offset> <size>\n");
                    continue;
                }
                read_file(filename, atoi(offset), atoi(size));
            }
            else if (strcmp(command, "write") == 0)
            {
                char *filename = strtok(NULL, " ");
                char *offset = strtok(NULL, " ");
                char *size = strtok(NULL, " ");
                char *data = strtok(NULL, " ");
                if (filename == NULL || offset == NULL || size == NULL || data == NULL)
                {
                    printf("Usage: write <filename> <offset> <size> <data>\n");
                    continue;
                }
                write_file(filename, atoi(offset), atoi(size), data);
            }
            else if (strcmp(command, "delete") == 0)
            {
                char *filename = strtok(NULL, " ");
                if (filename == NULL)
                {
                    printf("Usage: delete <filename>\n");
                    continue;
                }
                delete_file(filename);
            }
            else if (strcmp(command, "exit") == 0)
            {
                exit(0);
            }
            else
            {
                printf("Invalid command\n");
            }
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);

        if (status == 0)
        {
            printf("Client exited successfully\n");
        }
        else
        {
            printf("Client exited with error\n");
        }
    }
}