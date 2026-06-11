#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
	int res;

	for (int i = 1; i < argc; i++){
		res = rmdir(argv[i]);	
		if(!res){
			printf("\nUsted ha eliminado un directorio exitosamente %s\n", argv[i]);
		}else{
			fprintf(stderr, "\nHa ocurrido un error al eliminar el directorio %s\n", argv[i]);
		}
	}	
	return 0;
}
