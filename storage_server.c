#include "storage_sesrver.h"

int main()
{
    int number_of_accessible_paths;
    scanf("%d", &number_of_accessible_paths);
    char *accessible_paths[number_of_accessible_paths];
    for (int i = 0; i < number_of_accessible_paths; i++)
    {
        accessible_paths[i] = (char *)malloc(sizeof(char) * 4096);
        scanf("%s", accessible_paths[i]);
    }
    char *ip="127.0.0.1";
    int port_for_access=8089;


    



    return 0;
}