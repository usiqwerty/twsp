#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configs.h"

#define SIZE_OF(x) sizeof(x)
int loaded=0;
char* saveptr;
char* comma_values;
char* *comma_index;
size_t count;
size_t * s;
long size;
int ii;
void csv_free(){
	free(comma_index);
	free(comma_values-7);	
}
int file_routine(FILE* filefd){
	fseek(filefd, 0, SEEK_END);
	size = ftell(filefd);

	fseek(filefd, 0, SEEK_SET);
	comma_values=malloc(size+64);
	if (comma_values==NULL) {return 2;} //superhuge error, immediate exit
	fread(comma_values, 1, size, filefd);

	count=0;
	s = comma_values;
	comma_values+=7;
	//comma_values+=SIZE_OF(size_t);
	comma_index=calloc(size ,SIZE_OF(char *));
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
char * comma_value(FILE* filefd, int type, char *val, size_t id){
//realloc(comma_index, size);
	ii=0;
	char * tmp;
	if (type==0){ //key
		for (count=0;count<(*s); count++){
			tmp=comma_index[count];
			tmp=strtok_r(tmp, ":", &saveptr);
			if(!tmp){return "";}
			if (!strcmp(tmp, val)){
				tmp=strtok_r(NULL, ":", &saveptr);
				*(tmp+strlen(tmp)-1)=':';
				return tmp;
			}
			else{
				*(tmp+strlen(tmp)-1)=':';
			}
		}
	}
	else if (id<*s) {//works only if id is smaller than amount of entries
		strtok_r(comma_index[id], ":", &saveptr);
		return strtok_r(NULL, ":", &saveptr);
	}
	return ""; //nothing found
}
FILE * put_value(FILE * filefd, char * key, char * val){


	fclose(filefd);
	filefd=fopen(CSV_FILE, "w");
	char tmp[64]=";";
	strcat(tmp, key);
	strcat(tmp, ":");
	strcat(tmp, val);
	size+=strlen(tmp);
	(*s)++;
	strcat(comma_values+7, tmp);
	fwrite(comma_values, 1, size, filefd);
	fclose(filefd);

	filefd=fopen(CSV_FILE, "r");
	return filefd;
}
