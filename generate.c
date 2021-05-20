#include <stdio.h>

int main(){
	size_t a=0;
	FILE *f=fopen("filename.csv","w");
	fwrite(&a, sizeof(size_t), 1, f);
	fclose(f);
	return 0;
}
