#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char** argv){

	DIR* directorio;
    struct dirent *direc;
	if (argc == 1){
		printf("ERROR: No se ingresaron parametros\n");
	}
    for(int i = 1; i < argc; i++){
        directorio = opendir (argv[i]);
        printf("---------------------------------------\n");
        printf("-> Directorio %s:\n", argv[i]);
        if (directorio != NULL){
            while ((direc = readdir(directorio)) != NULL){
                 if ((strcmp(direc->d_name,".")!=0) && (strcmp(direc->d_name,"..")!= 0)){
                     printf(":> %s\n", direc->d_name);
                  }
            }    
        closedir (directorio);
        }else{
            fprintf(stderr, "\nError al abrir el directorio %s\n", argv[i]);
        }
        printf("---------------------------------------\n");
    }
    return 0;
}
