#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>
#include "auth.h"

int user()
{
    printf("--------------Login Menu--------------\n");
    printf("1.Enter as Admin\n");
    printf("2.Enter as User\n");
    int choice;
    scanf("%d", &choice);
    return choice;
}

auth *login()
{
    printf("--------------Login Menu--------------\n");
    printf("1.Enter user name: \n");
    char str[100];
    scanf("%s", str);
    printf("2.Enter password: \n");
    char str2[100];
    scanf("%s", str2);
    auth *a = (auth *)malloc(sizeof(auth));
    strcpy(a->name, str);
    strcpy(a->password, str2);
    return a;
}

    
char *Add_book(int id, int copies, char book[100]) {
    // Add a new book
    FILE *fp = fopen("book.txt", "r");
    if (fp == NULL) {
        return "Error opening the file.\n";
    }
    int fpno = fileno(fp);
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_type = F_RDLCK;
    
    char tmp_id[100], tmp_copies[100];
    char tmp_books[100];
    int flag = 0;

    int res = fcntl(fpno, F_SETLKW, &lock);
    if (res < 0){
        return "Lock unsuccessful\n";
    }
    while (fscanf(fp, "%s %s %s\n", tmp_id, tmp_books, tmp_copies) == 3) {
        if (id == atoi(tmp_id)) {
            lock.l_type = F_UNLCK;
            fcntl(fpno, F_SETLKW, &lock);
            flag = 1;
            return "Book already exists!!!\n";
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);

    struct flock lock2;
    lock2.l_whence = SEEK_SET;
    lock2.l_len = 0;
    lock2.l_start = 0;
    lock2.l_type = F_WRLCK;


    if (flag == 0) {
        fclose(fp);
        fp = fopen("book.txt", "a");
        fpno = fileno(fp);
        int res = fcntl(fpno, F_SETLKW, &lock2);
        if (res < 0){
           return "Lock unsuccessful\n";
        }
        if (fp == NULL) {
            return "Error opening the file.\n";
        }

        fprintf(fp, "%d %s %d\n", id, book, copies);
    }
    lock2.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock2);  
    fclose(fp);
    return "Book added successfully!!!\n";
}


char * Del_book(int del_id) {
    int id, copies;
    char book[100];
    
    // Delete a book
    FILE *fp1 = fopen("book.txt", "r");
    FILE *fp2 = fopen("temp.txt", "w");
    
    if (fp1 == NULL || fp2 == NULL) {
        return "Error opening the file.\n";
    }

    char id1[100];
    char copy1[100];
    int flag = 0;

    int fpno = fileno(fp1);
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_type = F_RDLCK;

    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);
    
    while (fscanf(fp1, "%s %s %s\n", id1, book, copy1) == 3) {
        id = atoi(id1);
        copies = atoi(copy1);
        if (id != del_id) {
            fprintf(fp2, "%d %s %d\n", id, book, copies);
        }
        else{
            printf("entered\n");
            flag = 1;
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove("book.txt");
    rename("temp.txt", "book.txt");

    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);
    if(flag == 0) {
        return "Book not found!!!\n";
        }
    return "Book deleted successfully!!!\n";
}


char * Update_book(int mod_id, char new_book[100], int add_copies) {
    int id, copies;
    char book[100];
    
    // Update a book
    FILE *fp1 = fopen("book.txt", "r");
    FILE *fp2 = fopen("temp.txt", "w");
    
    if (fp1 == NULL || fp2 == NULL) {
        return "Error opening the file.\n";
    }

    char id1[100];
    char copy1[100];
    int flag = 0;

    int fpno = fileno(fp1);
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_type = F_RDLCK;

    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);
    
    while (fscanf(fp1, "%s %s %s\n", id1, book, copy1) == 3) {
        id = atoi(id1);
        copies = atoi(copy1);
        if (id != mod_id) {
            fprintf(fp2, "%d %s %d\n", id, book, copies);
        } else {
            fprintf(fp2, "%d %s %d\n", mod_id, new_book, add_copies);
            flag = 1;
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove("book.txt");
    rename("temp.txt", "book.txt");

    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);
    if(flag == 0)
    {
        return "Book not found!!!\n";
    }
    return "Book updated successfully!!!\n";
}


char * Search_book_admin(int search_id) {
    int id, copies;
    char *book = (char *)malloc(100*sizeof(char));
    
    // Search a book
    FILE *fp1 = fopen("book.txt", "r");
    
    if (fp1 == NULL) {
        return "Error opening the file.\n";
    }

    char id1[100];
    char copy1[100];
    int found = 0;

    int fpno = fileno(fp1);
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_type = F_RDLCK;

    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);
    
    while (fscanf(fp1, "%s %s %s\n", id1, book, copy1) == 3) {
        id = atoi(id1);
        copies = atoi(copy1);
        if (id == search_id) {
            strcat(book, " ");
            strcat(book, copy1);
            strcat(book,"\n");
            found = 1;
            return book;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);
    
    if (found == 0) {
        return "Book not found\n";
    }
    
    fclose(fp1);
}


char * Member_details(char mem_name[100]) {
    int id, copies;
    char book[100];
    
    // Member details
    FILE *fp1 = fopen("member.txt", "r");
    
    if (fp1 == NULL) {
        return "Error opening the file.\n";    
    }

    char tmp[100];
    int flag = 0;

    int fpno = fileno(fp1);
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_type = F_RDLCK;

    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);
    char *borr_books = (char *)malloc(100*sizeof(char));
    strcpy(borr_books, "");
    while (fgets(tmp, sizeof(tmp), fp1) != NULL) {
        char *token = strtok(tmp, " ");
        if (strcmp(token, mem_name) == 0) {
            // printf("Books borrowed by %s:\n", mem_name);
            token = strtok(NULL, " ");
            while (token != NULL) {
                // printf("%s ", token);
                strcat(borr_books, token);
                strcat(borr_books, " ");
                token = strtok(NULL, " ");
            }
            flag = 1;
            return borr_books;
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fpno, F_SETLKW, &lock);
    if (flag == 0) {
        return "Member not found\n";
    }
    
    fclose(fp1);
}


BookDat *UserMenu(char user_name[100])
{
    FILE *fp1 = fopen("member.txt", "r");
    if (fp1 == NULL) 
    {
        printf("Error opening the file.\n");
        return NULL;
    }

    // int fpno = fileno(fp1);
    // struct flock lock;
    // lock.l_whence = SEEK_SET;
    // lock.l_len = 0;
    // lock.l_start = 0;
    // lock.l_type = F_RDLCK;

    // lock.l_type = F_UNLCK;
    // fcntl(fpno, F_SETLKW, &lock);

    char tmp[100];
    int sum = 0;
    char *books[10];
    for(int i = 0; i < 10; i++)
    {
        books[i] = (char *)malloc(100);
    }
    while(fgets(tmp, sizeof(tmp), fp1) != NULL)
    {
        char *token = strtok(tmp, " ");
        if(strcmp(token,user_name) == 0)
        {
            token = strtok(NULL, " ");
            while (token != NULL)
            {
                strcpy(books[sum++], token);
                token = strtok(NULL, " ");
            }
            break;
        }
    }

    // lock.l_type = F_UNLCK;
    // fcntl(fpno, F_SETLKW, &lock);
    BookDat *b = (BookDat *)malloc(sizeof(BookDat));
    b->sum = sum;
    for(int i = 0; i < sum; i++)
    {
        b->book[i] = (char *)malloc(100*sizeof(char));
        strcpy(b->book[i], books[i]);
        // printf("book: %s\n", books[i]);
    }
    fclose(fp1);
    return b;
}

char *Search_book_mem(BookDat *b,int search_id){
        int sum = b->sum;
        int flag = 0;
        for(int i = 0; i < sum; i++)
        {
            if(atoi(b->book[i]) == search_id)
            {
                flag = 1;
                return "Book found\n";
            }
        }
        if(flag == 0)
        {
            return "Book not found\n";
        }
        return NULL;
}


char *Borrow_book(int borrow_id, char mem_name[100]){
      
        FILE *fp1 = fopen("book.txt", "r");
        FILE *fp2 = fopen("temp.txt", "w");
        if (fp1 == NULL || fp2 == NULL) {
            return "Error opening the file.\n";   
        }
        char id1[100];
        char copy1[100];
        char book[100];

        int fpno = fileno(fp1);
        struct flock lock;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        lock.l_start = 0;
        lock.l_type = F_RDLCK;

        lock.l_type = F_UNLCK;
        fcntl(fpno, F_SETLKW, &lock);

        while(fscanf(fp1,"%s %s %s\n", id1, book, copy1) == 3)
        {
            int id = atoi(id1);
            int copies = atoi(copy1);
            if(id != borrow_id)
            {
                fprintf(fp2, "%d %s %d\n", id, book, copies);
            }
            else fprintf(fp2, "%d %s %d\n", id, book, copies-1);
        }

        lock.l_type = F_UNLCK;
        fcntl(fpno, F_SETLKW, &lock);

        fclose(fp1);
        fclose(fp2);
        remove("book.txt");
        rename("temp.txt", "book.txt");


        FILE *fp3 = fopen("member.txt", "r");
        FILE *fp4 = fopen("temp.txt", "w");
        if (fp3 == NULL || fp4 == NULL) {
            return "Error opening the file.\n";     
        }
        char new_id[100];
        char tmp[100];

        fpno = fileno(fp3);
        struct flock lock2;
        lock2.l_whence = SEEK_SET;
        lock2.l_len = 0;
        lock2.l_start = 0;
        lock2.l_type = F_RDLCK;

        lock2.l_type = F_UNLCK;
        fcntl(fpno, F_SETLKW, &lock2);

        while(fgets(tmp, sizeof(tmp), fp3) != NULL)
        {
            char writing[100];
            strcpy(writing, tmp);
            char *token = strtok(tmp, " ");
            if(strcmp(token,mem_name) == 0)
            {
                writing[strlen(writing)-1] = '\0';
                strcat(writing, " ");
                sprintf(new_id, "%d", borrow_id);
                strcat(writing,new_id);
                strcat(writing,"\n");
                printf("%s\n", writing);
                fputs(writing, fp4);
            }
            else
            {
                fputs(writing, fp4);
            }
        }

        lock2.l_type = F_UNLCK;
        fcntl(fpno, F_SETLKW, &lock2);

        fclose(fp3);
        fclose(fp4);
        remove("member.txt");
        rename("temp.txt", "member.txt");

        return "Book borrowed successfully!!!\n";
    }

char *Return_book(int borrow_id, char user_name[100]) 
{
        FILE *fp1 = fopen("book.txt", "r");
        FILE *fp2 = fopen("temp.txt", "w");
        if (fp1 == NULL || fp2 == NULL) {
            return "Error opening the file.\n";         
        }
        char id1[100];
        char copy1[100];
        char book[100];

        int fpno = fileno(fp1);
        struct flock lock;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        lock.l_start = 0;
        lock.l_type = F_RDLCK;

        lock.l_type = F_UNLCK;
        fcntl(fpno, F_SETLKW, &lock);

        while(fscanf(fp1,"%s %s %s\n", id1, book, copy1) == 3)
        {
            int id = atoi(id1);
            int copies = atoi(copy1);
            if(id != borrow_id)
            {
                fprintf(fp2, "%d %s %d\n", id, book, copies);
            }
            else fprintf(fp2, "%d %s %d\n", id, book, copies+1);
        }

        lock.l_type = F_UNLCK;
        fcntl(fpno, F_SETLKW, &lock);

        fclose(fp1);
        fclose(fp2);
        remove("book.txt");
        rename("temp.txt", "book.txt");


        FILE *fp3 = fopen("member.txt", "r");
        FILE *fp4 = fopen("temp.txt", "w");
        if (fp3 == NULL || fp4 == NULL) {
            return "Error opening the file.\n";
        }
        char cpy_id[100];
        char tmp[100];
        fpno = fileno(fp3);
        struct flock lock2;
        lock2.l_whence = SEEK_SET;
        lock2.l_len = 0;
        lock2.l_start = 0;
        lock2.l_type = F_RDLCK;

        lock2.l_type = F_UNLCK;
        fcntl(fpno, F_SETLKW, &lock2);
        while(fgets(tmp, sizeof(tmp), fp3) != NULL)
        {
            
            char nochange[100];
            strcpy(nochange, tmp);
            char *token = strtok(tmp, " ");
            if(strcmp(token,user_name) == 0)
            {
                char writing[100];
                strcpy(writing, token);
                while (token != NULL)
                {   
                    // sprintf(cpy_id, "%d", borrow_id);
                    token = strtok(NULL, " ");
                    if(token!=NULL && atoi(token) != borrow_id)
                    {
                        strcat(writing, " ");
                        strcat(writing, token);
                    }
                }
                strcat(writing,"\n");
                fputs(writing, fp4);
            }
            else
            {
                fputs(nochange, fp4);
            }
        }

        lock2.l_type = F_UNLCK;
        fcntl(fpno, F_SETLKW, &lock2);

        fclose(fp3);
        fclose(fp4);
        remove("member.txt");
        rename("temp.txt", "member.txt");

        return "Book returned successfully!!!\n";

}     
