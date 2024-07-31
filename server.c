#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include "commons.h" 
#include "auth.h"
#include "menu.c"


int main(int argc, char * argv[]){
	// declare socket vars
	int l_sock, conn_sock;
	int r;
	struct sockaddr_in local, remote;
	int remote_size=sizeof(local);
	char str[100];
	char suffix[50]="Registration complete";


	fillzero(local);
	// get an internet socket
	l_sock=socket(AF_INET, SOCK_STREAM , 0);
	if (l_sock < 0 ){
		perror("socket():");
		exit(1);
	}

	// define a structure with a local address and 
	// parameters for server port
	local.sin_family = AF_INET;
    	local.sin_addr.s_addr = htonl(INADDR_ANY);
    	local.sin_port = htons( SRV_PORT );

	// associate the server socket to the address and port above
	r = bind(l_sock,(struct sockaddr *)&local, sizeof(local));
	if (r < 0 ){
		perror("bind():");
		exit(1);
	}

	// set the socket to listening for an incoming connection request
	r = listen(l_sock,1);
	if (r < 0 ){
		perror("listen():");
		exit(1);
	}

	//     The below part repeats for each new connection

	while(1){
		char str[100];
		char name[100],password[100];
		fillzero(remote);
		
		conn_sock=accept(l_sock,(struct sockaddr *)&remote,&remote_size);
		if (conn_sock < 0 ){
			perror("accept():");
			exit(1);
		}

		int x = fork();  // to handle concurrent users
		if(x==0) 
		{
		// receive
		int user;
		r = recv(conn_sock,&user,sizeof(user),0);
		if (r < 0 ){
			perror("recv():");
			exit(1);
		}
		r = recv(conn_sock,name,sizeof(name)-1,0);
		name[r]='\0';
		if (r < 0 ){
			perror("recv():");
			exit(1);
		}
		r = recv(conn_sock,password,sizeof(password)-1,0);
		if (r < 0 ){
			perror("recv():");
			exit(1);
		}
		password[r]='\0';
		//checking if user exists
 		FILE *fp = NULL;
		if(user == 1) fp = fopen("admin.txt", "r");
		else if(user == 2) fp = fopen("auth.txt", "r");
    	if (fp == NULL) {
        	printf("Error opening the file.\n");
        	return 1;
   		}
		
		char name_tmp[100];
		char password_tmp[100];
    	int match = 0;
    	// Read username and password from the file
    	while (fscanf(fp, "%s %s\n", name_tmp, password_tmp) == 2) {
        // Comparing input with file data
        	if (strcmp(name, name_tmp) == 0 && strcmp(password, password_tmp) == 0) {
            	match = 1;
           		break;
       		}
    	}
		
		write(conn_sock, &match, sizeof(match));
		// close the file
		fclose(fp);
		char *msg = malloc(200*sizeof(char));
		if(match==1 && user==1)
		{
			int f = 1;
			while(f){
			int choice;
			r = recv(conn_sock,&choice,sizeof(choice),0);
			if (r < 0 ){
				perror("recv():");
				exit(1);
			}
			if(choice == 1)
			{
				int id,copies;
				char book[100];
				r = recv(conn_sock,&id,sizeof(id),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				r = recv(conn_sock,book,sizeof(book)-1,0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				book[r]='\0';
				r = recv(conn_sock,&copies,sizeof(copies),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				strcpy(msg,Add_book(id,copies,book));	//sending message received by the function
				r = send(conn_sock,msg,strlen(msg)-1,0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				printf("%s\n",msg);
			}
			else if(choice == 2)
			{
				int del_id;
				r = recv(conn_sock,&del_id,sizeof(del_id),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				strcpy(msg,Del_book(del_id));	//sending message received by the function
				r = send(conn_sock,msg,strlen(msg)-1,0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				printf("%s\n",msg);
				
			}
			else if(choice == 3)
			{
				int mod_id,add_copies;
				char new_book[100];
				r = recv(conn_sock,&mod_id,sizeof(mod_id),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				r = recv(conn_sock,new_book,sizeof(new_book)-1,0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				new_book[r]='\0';
				r = recv(conn_sock,&add_copies,sizeof(add_copies),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				strcpy(msg,Update_book(mod_id,new_book,add_copies));	//sending message received by the function
				r = send(conn_sock,msg,strlen(msg)-1,0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				printf("%s\n",msg);
			}
			else if(choice == 4)
			{
				int search_id;
				r = recv(conn_sock,&search_id,sizeof(search_id),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				strcpy(msg,Search_book_admin(search_id));	//sending message received by the function
				r = send(conn_sock,msg,strlen(msg)-1,0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				printf("%s\n",msg);
			}
			else if(choice == 5)
			{
				char mem_name[100];
				r = recv(conn_sock,mem_name,sizeof(mem_name)-1,0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				strcpy(msg,Member_details(mem_name));	//sending message received by the function
				r = send(conn_sock,msg,strlen(msg)-1,0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				printf("%s\n",msg);
			}
			else if(choice == 6)
			{
				f = 0;
			}
			}
		}
		printf("match: %d\n",match);
		printf("user: %d\n",user);
		if(match==1 && user==2)
		{
			int fl = 1;
			while(fl){
			BookDat *bookdat = (BookDat *)malloc(sizeof(BookDat));
			bookdat = UserMenu(name);

			int choice;
			r = recv(conn_sock,&choice,sizeof(choice),0);
			if (r < 0 ){
				perror("recv():");
				exit(1);
			}

			if(choice == 1)
			{
				int search_id;
				r = recv(conn_sock,&search_id,sizeof(search_id),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				printf("id is: %d\n",search_id);
				strcpy(msg,Search_book_mem(bookdat,search_id));	//sending message received by the function
				r = send(conn_sock,msg,strlen(msg)-1,0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				printf("%s\n",msg);
			}
			else if(choice == 2)
			{
				int borrow_id;
				r = recv(conn_sock,&borrow_id,sizeof(borrow_id),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				strcpy(msg,Borrow_book(borrow_id,name));	//sending message received by the function
				r = send(conn_sock,msg,strlen(msg)-1,0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				printf("%s\n",msg);
			}
			else if(choice == 3)
			{
				int return_id;
				r = recv(conn_sock,&return_id,sizeof(return_id),0);
				if (r < 0 ){
					perror("recv():");
					exit(1);
				}
				strcpy(msg,Return_book(return_id,name));	//sending message received by the function
				r = send(conn_sock,msg,strlen(msg)-1,0);
				if (r < 0 ){
					perror("send():");
					exit(1);
				}
				printf("%s\n",msg);
			}
			else if(choice == 4) fl=0;
			}
		}

		// close the new sock
		// close(conn_sock);
	}
	else close(conn_sock);
	}
}