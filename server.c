//c libs
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <regex.h>

//configuration
#include "configs.h"

#define STR_LEN(x) strlen(x)
//functions
int re(char *str, char * pat, int max, regmatch_t groups[]);

//basic integers
int r, i;

//server mainloop flag
//nonzero for working state
int run=1;

//socket things
struct sockaddr_in address;
int addrlen = sizeof(address);
int server_fd, new_socket;
//client data
unsigned char buffer[1024];

//regular expressions
regex_t regex;
regmatch_t groupArr[3];

//method
char *meth;
//array with pointers to headers
char * headers[10];

//array with pointers to cookies
char * cookies[10];
//temporal char pointer
char * tmp;
//number of headers
int h_num;


int main(int argc, char const *argv[]){
	char *br = "HTTP/1.0 400 Bad Request\n\n";
	char *nf = "HTTP/1.0 404 Not found\n\n<center><h1>404 Not found :/</h1></center>";
	char *server="HTTP/1.0 200 OK\nContent-Type: text/html; charset=UTF-8\nServer:Enigma-in-C/1.0\n\n";
	char * new = "<center>You have made nothing since registration</center>";

	//maybe it is better to use 201 Created in this case
	char reg_complete[120]="HTTP/1.0 200 OK\nContent-Type: text/html; charset=UTF-8\nServer:Enigma-in-C/1.0\nSet-Cookie: ";

	meth=malloc(6);
	printf("%s\n", NAME);
	FILE *db=fopen(CSV_FILE, "r+");
	if (!db){
		printf("%s\n", "Failed to open database, exiting");
		return 1;
	}

	r=file_routine(db);
	if (r==2){
		printf("%s\n", "Superhuge error, exiting");
		fclose(db);
		return 1;
	}
	// Creating socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( server_fd == 0){
		printf("Error: %s\n", "socket failure (1)");
		fclose(db);
		return 1;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	r=bind(server_fd, (struct sockaddr *) &address, sizeof(address));
	if (r<0){
		printf("Error: %s\n", "address bind failure (2)");
		close(server_fd);
		fclose(db);
		return 1;
	}
	else{
		printf("%s\n","Server started");
	}
	r=listen(server_fd, 3);
	if (r < 0){
		printf("Error: %s\n", "socket listen failure (3)");
		close(server_fd);
		fclose(db);
		return 1;
	}

//main loop
	while (run){
		new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t*) &addrlen);
		if (new_socket<0)
		{
			printf("Error: %s\n", "socket accept failure (4)");
			close(server_fd);
			fclose(db);
			return 1;
		}
		r=read(new_socket , buffer, 1024);

		if (r!=0){
			i=0;
			headers[0] = strtok(buffer, "\n");
			while(headers[i]){
				i++;
				headers[i]=strtok(NULL, "\n");

			}

			h_num=i;
			strcpy(meth,headers[0]);
			//printf("%s\n", headers[0]);
			if (re(meth, "[Gg][Ee][Tt][:ascii:]*", 0, NULL)){
				tmp = strtok(headers[0], " ");
				tmp = strtok(NULL, " ");

				if(!strcmp(tmp, "/")){
					tmp="/index.html";
				}
				//skip `slash` symbol
				tmp++;
				char filename[64]="\0";
				puts(tmp);
				strcpy(filename, "files/");
				strcat(filename, tmp);
				tmp=filename;
				puts(tmp);

				FILE *f=fopen(tmp, "r");
				if (!f && strcmp(tmp, "files/page")){
					//file not found
					send(new_socket , nf , STR_LEN(nf) , 0 );
				}
				else{
					find_cookie(headers, cookies, h_num);
					puts("123");
					if (cookies[0]!=0){
						char *user = parse_cookie(cookies[0], 1); //get value
						char *pass = parse_cookie(cookies[0], 2); //get value

						puts("Hello");
						if ( !strcmp( comma_value(db, 0, user, 0) , pass) ){
							puts("Correct password");
							if (!strcmp(tmp, "files/page")){
								filename[0]='\0';
								strcpy(filename, user);

								strcat(filename, "/page");

								f=fopen(filename, "r");
							}
							else {
								puts("Bye");
								fclose(f);
							}
						}
						else{
							puts("Wrong Password");
						}

						
					}
					puts("Hi!");
					send(new_socket, server,STR_LEN(server),0);
					r=fread(buffer, 1, 1024, f);
					send(new_socket, buffer, r, 0);
					while(r==1024){
						r=fread(buffer, 1, 1024, f);
						send(new_socket, buffer, r, 0);
					}
					fclose(f);
					//free(tmp);
				}

			}
			else if(re(meth, "[Pp][Oo][Ss][Tt][:ascii:]*", 0, NULL)) {
				puts("PooST");
				char * user=headers[h_num-1];
				user=strtok(user, "&");
				char * pass=strtok(NULL, "&");
				user=strtok(user, "=");
				user=strtok(NULL, "=");
				pass=strtok(pass, "=");
				pass=strtok(NULL, "=");
				db=put_value(db, user, pass);
				if (!db){
					printf("%s\n", "Failed to open database, exiting");
					return 1;
				}
				mkdir(user, 0777);
				char nu[64];
				strcpy(nu, user);
				strcat(nu, "/page");
				FILE * new_user = fopen(nu, "w");
				fwrite(new, 1, STR_LEN(new), new_user);
				fclose(new_user);

				strcat(reg_complete, user);
				strcat(reg_complete, "=");
				strcat(reg_complete, pass);
				strcat(reg_complete, "\n\n");
				send(new_socket , reg_complete , STR_LEN(reg_complete) + strlen(user) + strlen(pass) +3, 0);
			}
			else{
				//bad request
				send(new_socket , br , STR_LEN(br) , 0 );
			}
		}
		close(new_socket);
		for (i=0; i<1024;i++){
			buffer[i]=0;
		}
	}
	free(meth);
	close(server_fd);
	fclose(db);
	return 0;
}

int re(char *str, char * pat, int max, regmatch_t groups[]){
	int reti;
	regcomp(&regex, pat, REG_EXTENDED);
	reti = regexec(&regex, str, max, groups, 0);
	if (!reti){
		return 1;
	}
	return 0;
}

