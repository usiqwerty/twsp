//c libs
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
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

			headers[0] = strtok(buffer, "\n");
			i=1;
			while(headers[i]){
				headers[i]=strtok(NULL, "\n");
				i++;
			}

			h_num=i;
			printf("%s\n", headers[0]);
			if (re(headers[0], "[Gg][Ee][Tt][:ascii:]*", 0, NULL)){
				tmp = strtok(headers[0], " ");
				tmp = strtok(NULL, " ");

				if(!strcmp(tmp, "/")){
					tmp="/index.html";
				}
				//skip `slash` symbol
				tmp++;
				FILE *f=fopen(tmp, "r");
				if (!f){
					//file not found
					send(new_socket , nf , STR_LEN(nf) , 0 );
				}
				else{
					find_cookie(headers, cookies, h_num);
					char *user = parse_cookie(cookies[0], 2); //get value
					if (strcmp(user, "") && !strcmp(tmp, "private.html")){
						strcpy(tmp, user);
						strcat(tmp, "page");
					}
					send(new_socket, server,STR_LEN(server),0);
					r=fread(buffer, 1, 1024, f);
					send(new_socket, buffer, r, 0);
					while(r==1024){
						r=fread(buffer, 1, 1024, f);
						send(new_socket, buffer, r, 0);
					}
					fclose(f);
				}


			}
			//Working on...
			/*
			else if(re(headers[0], "[Pp][Oo][Ss][Tt][:ascii:]*", 0, NULL)) {
				char * req=headers[h_num-1];
				
			}*/
			else{
				//bad request
				send(new_socket , br , STR_LEN(br) , 0 );
			}
		}
		close(new_socket);
	}
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
}

