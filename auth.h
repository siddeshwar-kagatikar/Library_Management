# include <stdio.h>
#ifndef AUTH_H
#define AUTH_H
#define AUTH_LEN 20;


typedef struct auth
{
    char name[100];
    char password[100];
}auth;

typedef struct BookDat
{
    char *book[10];
    int sum;
}BookDat;

#endif