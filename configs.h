#define NAME "TWSP v1.0"
#define PORT 8080

int dirlist(char *d, char *list[]);
void string_lowercase(char *str);
void find_cookie(char * headers[], char * cookies[], int h_num);
char * parse_cookie(char* cookie, int pos);
