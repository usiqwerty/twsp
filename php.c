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
	}
	i=0;

}
char * parse_cookie(char* cookie, int pos){
	char *piece;
	piece=strtok(cookie, "=");
	if (pos==1){
		return piece;
	}
	else{
		return strtok(NULL, "=");
	}
	return "";
}
