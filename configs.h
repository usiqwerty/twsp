#define NAME "TWSP v1.0"
#define PORT 8080
#define CSV_FILE "database.csv"

//csv.c
int file_routine(FILE* filefd);
char * comma_value(int filefd, int type, char *val, size_t id);

//php.c
int dirlist(char *d, char *list[]);
void string_lowercase(char *str);
void find_cookie(char * headers[], char * cookies[], int h_num);
char * parse_cookie(char* cookie, int pos);
