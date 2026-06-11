#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv){
	FILE *archivo;
	int c;
	if (argc == 1){
		printf("ERROR: No se ingresaron parametros\n");
	}
	for(int i = 1; i < argc; i++){
		archivo = fopen(argv[i], "r");
		printf("---------------------------------------\n");
		printf("Archivo %s:\n", argv[i]);
		if(archivo != NULL){		
			printf("\n");
			while((c = fgetc(archivo)) != EOF){
				printf("%c", c);
			}
			printf("\n");
			fclose(archivo);
		} else {
			printf("\nHa ocurrido un error al abrir el archivo\n");
		}
		printf("---------------------------------------\n");
	}

	return 0;
}
