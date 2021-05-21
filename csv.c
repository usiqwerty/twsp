#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configs.h"

#define SIZE_OF(x) sizeof(x)
int loaded=0;
char* comma_values;
char** comma_index;
size_t count;
size_t * s;
long size;
int ii;
int file_routine(FILE* filefd){
	fseek(filefd, 0, SEEK_END);
	size = ftell(filefd);

	fseek(filefd, 0, SEEK_SET);
	comma_values=malloc(size+64);
	if (comma_values==NULL) {return 2;} //superhuge error, immediate exit
	fread(comma_values, 1, size, filefd);

	count=0;
	s = comma_values;
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
		for (count=1;count<(*s); count++){
			printf("%ld\n", count);
			tmp=strtok(comma_index[count], ";");
			if (!strcmp(tmp, val)){
				puts("OK!");
				return strtok(tmp, ":");
			}
			else{
				puts("No");
				*(tmp-1)=':';
			}
			puts(tmp);
		}
	}
	else if (id<*s) {//works only if id is smaller than amount of entries
		strtok(comma_index[id], ":");
		return strtok(NULL, ":");
	}
	printf("Done");
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
	puts(tmp);
	(*s)++;
	strcat(comma_values+7, tmp);
	puts(comma_values+7);
	fwrite(comma_values, 1, size, filefd);
	fclose(filefd);

	filefd=fopen(CSV_FILE, "r");
	puts("FInally opened");
	return filefd;
}
