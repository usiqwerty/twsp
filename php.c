#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int dirlist(char *d){
	DIR *dp;
	struct dirent *ep;
	dp = opendir (d);
	if (dp != NULL){
		while (ep = readdir (dp))
			puts (ep->d_name);
			(void) closedir (dp);
		}
	else{return 1;}
	return 0;
}
int login(){

}
int main(){
dirlist("./");
return 0;
}
