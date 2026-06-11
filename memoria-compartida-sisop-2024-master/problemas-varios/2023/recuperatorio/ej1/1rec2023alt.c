#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>


#define tam_linea sizeof(char[256])



//defino los files descriptors para los pipes
int pipePar[2];
int pipeImpar[2];
int pipePadrePar[2];
int pipePadreImpar[2];
FILE *archivo;
char *nombre_archivo;

void proceso1(){
    
    close(pipeImpar[0]);
    close(pipePar[0]);
    close(pipePadrePar[0]);
    close(pipePadrePar[1]);
    close(pipePadreImpar[0]);
    close(pipePadreImpar[1]);
    
    archivo = fopen(nombre_archivo,"r");
    if(archivo == NULL) printf("no se pudo abrir: %s \n",strerror(errno));
    
    
    char linea[256];
    int current_line = 1;
    
    while(fgets(linea,tam_linea,archivo)){
		
		if(current_line % 2 == 1)  write(pipeImpar[1], linea, tam_linea);
		
		else  write(pipePar[1], linea, tam_linea);
		current_line++;
	}
	
	char linea_fin[256] = "-1";
    
	write(pipeImpar[1], linea_fin, tam_linea);
	
	write(pipePar[1], linea_fin, tam_linea);
	
	fclose(archivo);
    
}

void proceso2(){
    close(pipeImpar[1]);
    close(pipePar[1]);
    close(pipeImpar[0]);
    close(pipePadrePar[0]);
    close(pipePadreImpar[0]);
	close(pipePadreImpar[1]);
	
	char *str;
    char linea[256];
    char fin[256] = "-1"; 
    int cant_palabras = 0, seguir = 1,in_word = 0, count = 0, currentpos = 0;
    
    while(seguir){
		read(pipePar[0], linea, tam_linea);
		
		if(strcmp(fin,linea) != 0){
			
			while (linea[currentpos] != '\0' && linea[currentpos] != '\n') {
				
				if (isspace(linea[currentpos])) {
					in_word = 0;
					
				} else if (in_word == 0) {
					
					in_word = 1;
					count++;
				
				}
				currentpos++;
				
			}
			
			write(pipePadrePar[1],&count,sizeof(int));
			in_word = 0; 
			count = 0;
			currentpos = 0;
		}
		else{
				
				count = -1;
				write(pipePadrePar[1],&count,sizeof(int));
				seguir = 0;
		}
	}
    
	
}

void proceso3(){
    close(pipeImpar[1]);
    close(pipePar[1]);
    close(pipePar[0]);
    close(pipePadrePar[0]);
    close(pipePadreImpar[0]);
	close(pipePadrePar[1]);
	
	char *str;
    char linea[256];
    char fin[256] = "-1"; 
    int cant_palabras = 0, seguir = 1, count = 0, currentpos = 0;
    
    while(seguir){
		read(pipeImpar[0], linea, tam_linea);
		
		if(strcmp(fin,linea) != 0){
			
			while (linea[currentpos] != '\0' && linea[currentpos] != '\n') {
				if (linea[currentpos] == 'A') {
					count++;
				}
				currentpos++;
			}
			//printf("prc 3 %d\n", count);
			write(pipePadreImpar[1],&count,sizeof(int));
			count = 0;
			currentpos = 0;
		
		}
		else{
			
				count = -1;
				write(pipePadreImpar[1],&count,sizeof(int));
				seguir = 0;
		}
	}
}


int main(int argc,  
char *argv[]){
	pid_t proc1, proc2, proc3;
	
	nombre_archivo = argv[1];
	
	
    pipe(pipePar);
    pipe(pipeImpar);
    pipe(pipePadrePar);
	pipe(pipePadreImpar);
	proc1 = fork();
    if(proc1 == 0) {
        proceso1();
        exit(0);
    }
	
	
    proc2 = fork();
    if(proc2 == 0) {
        proceso2();
        exit(0);
    }
	
	
    proc3 = fork();
    if(proc3 == 0) {
        proceso3();
        exit(0);
    }
    int seguir1= 1, seguir2 = 1, nroPal, nroA, lineaPar = 2,lineaImpar = 1;
    
    while(seguir1 || seguir2){
		
		if(seguir1){
			
			read(pipePadreImpar[0], &nroA, sizeof(int));
			
			if(nroA == -1) seguir1 = 0;
			else{
				 printf("cantidad de A en linea %d: %d\n",lineaImpar,nroA);
				 lineaImpar+=2;
			}
		
		}
		if(seguir2){
			read(pipePadrePar[0], &nroPal, sizeof(int));
			if(nroPal == -1) seguir2 = 0;
			else{ 
				printf("cantidad de palabras en linea %d: %d\n",lineaPar,nroPal);
				lineaPar+=2;
			}
		}
		sleep(2);
		
	}

    
    
    return 0;
}
