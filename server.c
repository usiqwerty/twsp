#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <regex.h>
#define PORT 8080
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

//temporal char pointer
char * tmp;



int main(int argc, char const *argv[]){
	char *br = "HTTP/1.0 400 Bad Request\n\n";
	char *nf = "HTTP/1.0 404 Not found\n\n<center><h1>404 Not found :/</h1></center>";
	char *server="HTTP/1.0 200 OK\nContent-Type: text/html; charset=UTF-8\nServer:Enigma-in-C/1.0\n\n";

	// Creating socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( server_fd == 0){
		printf("Error: %s\n", "socket failure (1)");
		return 1;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Attaching socket to the port 8080
	r=bind(server_fd, (struct sockaddr *) &address, sizeof(address));
	if (r<0){
		printf("Error: %s\n", "address bind failure (2)");
		close(server_fd);
		return 1;
	}
	r=listen(server_fd, 3);
	if (r < 0){
		printf("Error: %s\n", "socket listen failure (3)");
		close(server_fd);
		return 1;
	}

//main loop
	while (run){
		new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t*) &addrlen);
		if (new_socket<0)
		{
			printf("Error: %s\n", "socket accept failure (4)");
			close(server_fd);
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
					send(new_socket , nf , STR_LEN(nf) , 0 );
				}
				else{
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
			else{//dummy
				send(new_socket , br , STR_LEN(br) , 0 );
			}
		}
		close(new_socket);
	}
	close(server_fd);
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

