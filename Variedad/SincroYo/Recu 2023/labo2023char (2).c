#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // fork(), getpid(), getppid()
#include <sys/wait.h>  // wait(), waitpid()

#define READ_END 0
#define WRITE_END 1
#define CANT_PROC 3
#define SIZE_MAT 10
#define SIZE_PIPE_MSG sizeof(struct pipemsg)
#define SIZE_PIPE_RESULTADO sizeof(struct piperesultado)

struct pipemsg{
	char letra;
};

struct piperesultado{
	int resultado;
};

void printMatriz(char matriz[SIZE_MAT][SIZE_MAT]) {
    for (int i = 0; i < SIZE_MAT; i++) {
        for (int j = 0; j < SIZE_MAT; j++) {
            printf("%c ", matriz[i][j]);
        }
        printf("\n");
    }
}

int escribirPipe(char matriz[SIZE_MAT][SIZE_MAT], int pipePar[2], int pipeImpar[2]) {
	struct pipemsg msg;
	close(pipePar[READ_END]);
	close(pipeImpar[READ_END]);
	
	for (int i=0; i<SIZE_MAT; i++) {
		for (int j=0; j<SIZE_MAT; j++) {
			
			if (i % 2 == 0) {
				msg.letra = matriz[i][j];
				write(pipePar[WRITE_END], &msg, SIZE_PIPE_MSG);
			}
			else {
				msg.letra = matriz[i][j];
				write(pipeImpar[WRITE_END], &msg, SIZE_PIPE_MSG);
			}
		}
	}
	close(pipePar[WRITE_END]);
    close(pipeImpar[WRITE_END]);
	return 0;
}

int leerPares(int pipePar[2], int pipeResultado[2]) {
	struct pipemsg msg;
	struct piperesultado res;
	char letra;
	int pares = 0;
	close(pipePar[WRITE_END]);
	while (read(pipePar[READ_END], &msg, SIZE_PIPE_MSG) > 0) {
		letra = msg.letra;
		if (letra == 'e' || letra == 'i' || letra == 'o' || letra == 'u') {
			pares++;
		}
	}
	close(pipePar[READ_END]);
	res.resultado = pares;
	close(pipeResultado[READ_END]);
	write(pipeResultado[WRITE_END], &res, SIZE_PIPE_RESULTADO);
	close(pipeResultado[WRITE_END]);
	return 0;
}

int leeImpares(int pipeImpar[2], int pipeResultado[2]) {
	struct pipemsg msg;
	struct piperesultado res;
	char letra;
	int impares = 0;
	close(pipeImpar[WRITE_END]);
	while (read(pipeImpar[READ_END], &msg, SIZE_PIPE_MSG) > 0) {
		letra = msg.letra;
		if (letra == 'a') {
			impares++;
		}
	}
	close(pipeImpar[READ_END]);
	res.resultado = impares;
	close(pipeResultado[READ_END]);
	write(pipeResultado[WRITE_END], &res, SIZE_PIPE_RESULTADO);
	close(pipeResultado[WRITE_END]);
	return 0;
}


int main(){
	
	struct piperesultado res;
    int i;
    pid_t pid;
    char matriz [SIZE_MAT][SIZE_MAT]; 
    int fdP[2];
    int fdI[2];
    int fdResPar[2];
    int fdResImpar[2];
    pipe(fdP);
    pipe(fdI);
    pipe(fdResPar);
    pipe(fdResImpar);
    
    for (int i = 0; i < SIZE_MAT; i++) {
        for (int j = 0; j < SIZE_MAT; j++) {
            matriz[i][j] = 'a' + ( (i + j) % 26 );
        }
    }
    
    printMatriz(matriz);    

    pid = fork();
    if (pid < 0) {
        perror("Fork falló");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
		close(fdResPar[WRITE_END]);
		close(fdResImpar[WRITE_END]);
		close(fdResPar[READ_END]);
		close(fdResImpar[READ_END]);
        escribirPipe(matriz, fdP, fdI);
        exit(0);
    }

    pid = fork();
    if (pid < 0) {
        perror("Fork falló");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
		close(fdResImpar[WRITE_END]);
		close(fdI[WRITE_END]);	
		close(fdResImpar[READ_END]);
		close(fdI[READ_END]);				
        leerPares(fdP, fdResPar);
        exit(0);
    }

    pid = fork();
    if (pid < 0) {
        perror("Fork falló");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
		close(fdResPar[WRITE_END]);
		close(fdP[WRITE_END]);	
		close(fdResPar[READ_END]);
		close(fdP[READ_END]);			
        leeImpares(fdI, fdResImpar);
        exit(0);
    }
    
    close(fdP[READ_END]);
    close(fdP[WRITE_END]);
    close(fdI[READ_END]);
    close(fdI[WRITE_END]);
        
    close(fdResPar[WRITE_END]);
    read(fdResPar[READ_END], &res, SIZE_PIPE_RESULTADO);
    printf("Soy el padre %d este es el resultado de los pares: %d\n", getpid(), res.resultado);
    close(fdResPar[READ_END]);
    
    close(fdResImpar[WRITE_END]);
    read(fdResImpar[READ_END], &res, SIZE_PIPE_RESULTADO);
    printf("Soy el padre %d este es el resultado de los impares: %d\n", getpid(), res.resultado);
    close(fdResImpar[READ_END]);

    // Proceso padre
    for(i=0; i<CANT_PROC; i++){
        wait(NULL); // Espera a que cada hijo termine
    }
    printf("Todos los procesos hijos han terminado. Proceso padre PID: %d finalizando.\n", getpid());

    return 0;

}
