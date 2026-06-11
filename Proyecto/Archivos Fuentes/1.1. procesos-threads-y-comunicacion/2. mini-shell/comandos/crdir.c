#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
	int res;

	for (int i = 1; i < argc; i++){
		res = mkdir(argv[i], 0777);	
		if(!res){
			printf("\nSe ha creado un directorio con exito %s\n", argv[i]);
		} else {
			fprintf(stderr, "\nHa ocurrido un error al crear el directorio %s\n", argv[i]);
		}
	}	
	return 0;
}
