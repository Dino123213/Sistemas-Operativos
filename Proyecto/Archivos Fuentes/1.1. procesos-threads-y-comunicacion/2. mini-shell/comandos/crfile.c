#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
	FILE *archivo;
	for(int i = 1; i < argc; i++) {
		archivo = fopen(argv[i], "w");	
		if(archivo != NULL){ 
			printf("\nSe ha creado un archivo con exito %s\n", argv[i]);
			fclose(archivo);
		} else {
			fprintf(stderr, "\nHa ocurrido un error al crear el archivo %s\n", argv[i]);
		}
	}

	return 0;
}
