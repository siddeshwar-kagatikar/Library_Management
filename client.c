#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "commons.h" 
#include "auth.h"
#include "menu.c"

int main(int argc, char * argv[]){
	// declare socket vars
	int fsock;
	struct sockaddr_in server;
	char str[100];
	char suffix[50]="Client Message from  ";
	sprintf(str,"%s %u\n",suffix, getpid());
	
	fillzero(server);
	
	fsock=socket(AF_INET, SOCK_STREAM , 0);
	if (fsock < 0 ){
		perror("socket():");
		exit(1);
	}

	server.sin_family = AF_INET;
        server.sin_port = htons(SRV_PORT);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	// connect to the server on the sock
	int r = connect(fsock,(struct sockaddr *) &server, sizeof(server));
	if (r < 0 ){
		perror("connect():");
		exit(1);
	}

	// authentication
	int use = user();
	r = send(fsock,&use,sizeof(use),0);
	if (r < 0 ){
		perror("send():");
		exit(1);
	}
    auth *a = malloc(sizeof(auth));
	a = login();
	
    if(a!= NULL){
		r=send(fsock,a->name,sizeof(a->name)-1,0);
		if (r < 0 ){
			perror("send():");
			exit(1);
		}
		r = send(fsock,a->password,sizeof(a->password)-1,0);
		if (r < 0 ){
			perror("send():");
			exit(1);
		}
		int match=0;
		// receive
		r = recv(fsock,&match,sizeof(match),0);
		if (r < 0 ){
			perror("recv():");
			exit(1);
		}
		// match=1;
		if(match == 1 && use == 1) 
		{
			printf("Login successful\n");
			while(1)
			{
				printf("--------------Menu--------------\n");
				printf("1. Add a new book\n");
				printf("2. Delete a book\n");
				printf("3. Update a book\n");
				printf("4. Search a book\n");
				printf("5. Member details\n");
				printf("6. Exit\n");

				int choice;
				scanf("%d", &choice);
				r = send(fsock,&choice,sizeof(choice),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
				if(choice == 1)
				{
					int id,copies;
					char book[100];
					printf("Enter book id: \n");
            		scanf("%d", &id);
					r = send(fsock,&id,sizeof(id),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					printf("Enter book name: \n");
					scanf("%s", book);
					r = send(fsock,book,sizeof(book)-1,0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					printf("Enter number of copies: \n");
					scanf("%d", &copies);
					r = send(fsock,&copies,sizeof(copies),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					char msg[200];
					r = recv(fsock,msg,sizeof(msg)-1,0);
					if (r < 0 ){
						perror("recv():");
						exit(1);
					}
					msg[r] = '\0';
					printf("%s\n",msg);
				}
				else if(choice == 2)
				{
					int del_id;
					printf("Enter book id to be deleted: \n");
					scanf("%d", &del_id);
					r = send(fsock,&del_id,sizeof(del_id),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					char msg[200];
					r = recv(fsock,msg,sizeof(msg)-1,0);
					if (r < 0 ){
						perror("recv():");
						exit(1);
					}
					msg[r] = '\0';
					printf("%s\n",msg);
				}
				else if(choice == 3)
				{
					printf("Enter book id to be modified: \n");
					int mod_id;
					scanf("%d", &mod_id);
					r = send(fsock,&mod_id,sizeof(mod_id),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					printf("Enter modified book name: \n");
					char new_book[100];
					scanf("%s", new_book);
					r = send(fsock,new_book,sizeof(new_book)-1,0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					printf("Enter number of copies to be added: \n");
					int add_copies;
					scanf("%d",&add_copies);
					r = send(fsock,&add_copies,sizeof(add_copies),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					char msg[200];
					r = recv(fsock,msg,sizeof(msg)-1,0);
					if (r < 0 ){
						perror("recv():");
						exit(1);
					}
					msg[r] = '\0';
					printf("%s\n",msg);
				}
				else if(choice == 4)
				{
					printf("Enter book id to be searched: \n");
					int search_id;
					scanf("%d", &search_id);

					r = send(fsock,&search_id,sizeof(search_id),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					char msg[200];
					r = recv(fsock,msg,sizeof(msg)-1,0);
					if (r < 0 ){
						perror("recv():");
						exit(1);
					}
					msg[r] = '\0';
					printf("%s\n",msg);
				}
				else if(choice == 5)
				{
					printf("Enter member name: \n");
					char mem_name[100];
					scanf("%s", mem_name);
					r = send(fsock,mem_name,sizeof(mem_name)-1,0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					char msg[200];
					r = recv(fsock,msg,sizeof(msg)-1,0);
					if (r < 0 ){
						perror("recv():");
						exit(1);
					}
					msg[r] = '\0';
					printf("%s\n",msg);
				}
				else if(choice == 6)
				{
					close(fsock);
					break;
				}
			}
			// AdminMenu();
		}
		else if(match == 1 && use == 2)
		{
			printf("Login successful\n");
			while(1)
			{
				printf("--------------Menu--------------\n");
				printf("1. Search a book\n");
				printf("2. Borrow a book\n");
				printf("3. Return a book\n");
				printf("4. Exit\n");

				int choice;
				printf("Enter your choice: \n");
				scanf("%d", &choice);
				r = send(fsock,&choice,sizeof(choice),0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				
				if(choice == 1)
				{
					int search_id;
					printf("Enter book id to be searched: \n");
					scanf("%d", &search_id);
					r = send(fsock,&search_id,sizeof(search_id),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					char msg[200];
					r = recv(fsock,msg,sizeof(msg)-1,0);
					if (r < 0 ){
						perror("recv():");
						exit(1);
					}
					msg[r] = '\0';
					printf("%s\n",msg);
				
				}
				else if(choice == 2)
				{
					int borrow_id;
        			printf("Enter book id to be borrowed: \n");
        			scanf("%d", &borrow_id);
					r = send(fsock,&borrow_id,sizeof(borrow_id),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					char msg[200];
					r = recv(fsock,msg,sizeof(msg)-1,0);
					if (r < 0 ){
						perror("recv():");
						exit(1);
					}
					msg[r] = '\0';
					printf("%s\n",msg);
				}
				else if(choice == 3)
				{
					int borrow_id;
					printf("Enter book id to return: \n");
					scanf("%d", &borrow_id);
					r = send(fsock,&borrow_id,sizeof(borrow_id),0);
					if (r < 0 ){
						perror("send():");
						exit(1);
					}
					char msg[200];
					r = recv(fsock,msg,sizeof(msg)-1,0);
					if (r < 0 ){
						perror("recv():");
						exit(1);
					}
					msg[r] = '\0';
					printf("%s\n",msg);
				}
				else if(choice == 4)
				{
					close(fsock);
					break;
				}
			}
		}
		else printf("Login failed\n");

    }
	// exit
	exit(0);  // automatically socket is closed from client end
}