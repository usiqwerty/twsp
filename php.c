/*
int dirlist(char *d, char *list[]) 							-	 list directories
void string_lowercase(char *str)							-	lowercase the string
void find_cookie(char * headers[], char * cookies[], int h_num)	-	find all cookies in headers[]
char * parse_cookie(char* cookie, int pos)					-	get name or value of cookie
*/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//array of char pointers
char * li[256];
char* tmp_new;

//basic integers
//int  size;
int dirlist(char *d, char *list[]){
	DIR *dp;
	struct dirent *ep;
	dp = opendir (d);
	int i=0;
	if (dp != NULL){
		while (ep = readdir (dp)){
			list[i] = ep->d_name;
			i++;
		}
		//someone put (void) before closedir(), idk why
		closedir(dp);
		}
	else{return 1;} //error
	return 0;
}
void string_lowercase(char *str){
	int ii;
	for(ii=0;ii<=strlen(str)-1;ii++){
		if(str[ii]>=65 && str[ii]<=90)
			str[ii]+=32;
	}
}

void find_cookie(char * headers[], char * cookies[], int h_num){
	int i=0;
	int j=0;
	//while we're sure string is non-empy
	for(i=0;i<h_num;i++){
		//string_lowercase(headers[i]);
		if( strstr(headers[i], "Cookie:") ){
			tmp_new=strtok(headers[i],":");
			tmp_new=strtok(NULL,":")+1;	//because of whitespace

			cookies[j]=strtok(tmp_new, ";");
			while(cookies[j]){
				j++;
				cookies[j]=strtok(NULL, ";");
				if (cookies[j]){
					cookies[j]+=1;
				}

			}
		}
		else{
			printf("Nope, %d\n", i);
		}
	}
	i=0;

}
char * parse_cookie(char* cookie, int pos){
	char *piece;
	piece=strtok(cookie, "=");
	if (pos==1){
		return piece;
	}
	//else if(pos==2){
	else{
		puts("a");
		return strtok(NULL, "=");
	}
	return "";
}

/*int parser(char *filename, int sockfd){
	int last=0;
	FILE *f=fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	long int size = ftell(f);
	char * buffer=malloc(size);

	fseek(f, 0, SEEK_SET);
	r=read(f, buffer, size);
	
	for (i=0; i<size; i++){
		if ( buffer[i] == '<' && buffer [i+1] == '<' ){
			send(sockfd, buffer+last, i-last, 0);
			last=run_code(buffer+i+2);
			
		}
	}
	fclose(f);
	return buffer;
}
int run_code(int posit, char * cookies[]){
	int counter=0;
	char* val1, val2;
	if (!strncmp(posit, "if ", 3)){
		for (i=posit+3; buffer[i]!=' ';i++){
			counter++;
		}
		if (!strncmp(posit+3, "COOKIE0", 7)){
			val1=cookies[0];
		}
		else if (!strncmp(posit+3, "COOKIE1", 7)){
			val1=cookies[1];
		}
		else if (!strncmp(posit+3, "COOKIE3", 7)){
			val1=cookies[0];
		}
	}
}
int main(){
	int i;
	char *a="\0";
	char *b="\0";
	a=strcpy(a,"Cookie: a=11; b=; 0; c=1213!");
	b=strcpy(b,"Cookie: d=1d; e=1d; f=1vs13!");
	char * c[5];
	c[0]=a;
	c[1]=b;
	char *coo[10];
	//string_lowercase(a);
	//find_cookie(c, coo, 2);
	//for (i=0; i<6;i++)
	//	puts(coo[i]);
	return 0;
}*/

