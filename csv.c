#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configs.h"

#define SIZE_OF(x) sizeof(x)
int loaded=0;
char* comma_values;
char** comma_index;
size_t count,s;
long size;
int ii;
int file_routine(FILE* filefd){
	fseek(filefd, 0, SEEK_END);
	size = ftell(filefd);

	fseek(filefd, 0, SEEK_SET);
	comma_values=malloc(size);
	if (comma_values==NULL) {return 2;} //superhuge error, immediate exit
	fread(comma_values, 1, size, filefd);

	count=0;
	s= *comma_values;
	//comma_value+=SIZE_OF(size_t);
	comma_index=calloc(s ,SIZE_OF(char *));
	comma_index[0]=strtok(comma_values, ";");
	while(comma_index[count]){
		count++;
		comma_index[count]=strtok(NULL, ";");
	}

	return 0;
}
/*
filefd - fopen("db.csv")	type -- 0 for text key	val - text key	id - ID of entry
*/
char * comma_value(int filefd, int type, char *val, size_t id){
//realloc(comma_index, size);
	ii=0;
	if (type=0){ //key
		for (count=0;count<s; count++){
			char * tmp=strtok(comma_index[count], ":");
			if (!strcmp(tmp, val)){
				return strtok(NULL, ":");
			}
		}
	}
	else if (id<s) {//works only if id is smaller than amount of entries
		strtok(comma_index[id], ":");
		return strtok(NULL, ":");
	}

	return 0; //nothing found
}
FILE * put_value(FILE * filefd, char * key, char * val){
	fclose(filefd);
	filefd=fopen(CSV_FILE, "a");
	char * tmp = strcat(";", key);
	tmp = strcat(tmp, ":");
	tmp = strcat(tmp, val);
	fwrite(tmp,1, strlen(tmp), filefd);
	fclose(filefd);
	filefd=fopen(CSV_FILE, "r");
	return filefd;
}
