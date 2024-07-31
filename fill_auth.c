#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    FILE *fptr;
    char username[50];
    char password[50];
    fptr = fopen("auth.txt", "wb");
    for(int i = 0; i < 3; i++)
    {
        scanf("%s", username);
        scanf("%s", password);
        fprintf(fptr, "%s %s\n", username, password);
    }
}