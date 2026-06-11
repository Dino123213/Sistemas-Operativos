#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#define READ_END 0
#define WRITE_END 1
#define NUM_LETRAS 5
#define MATRIZ_TAM 9
#define SIZE_PIPE sizeof(struct pipemsg)
#define KEY 123

struct pipemsg{
    char pchar;
};

//otra manera:
/*void BuscarLetra(char letra, int filas, int cols, char *matriz) {
    // tratar como matriz 1D: matriz[i*cols + j]
}*/

void printMatriz(char matriz[MATRIZ_TAM][MATRIZ_TAM]) {
    for (int i = 0; i < MATRIZ_TAM; i++) {
        for (int j = 0; j < MATRIZ_TAM; j++) {
            printf("%c ", matriz[i][j]);
        }
        printf("\n");
    }
}



int BuscarLetra(char letra, char matrizBus[MATRIZ_TAM][MATRIZ_TAM], int fd_write) {
    struct pipemsg msg;
    
    for (int i=0; i<MATRIZ_TAM; i++) {
        for (int j=0; j<MATRIZ_TAM; j++) {
            if (letra == matrizBus[i][j]) {
				msg.pchar = letra;
                printf("Soy el hijo %d, encontre la letra %c en la posicion [%d][%d]\n", getpid(), letra, i, j);
                fflush(stdout);
                write(fd_write, &msg, SIZE_PIPE);
            }
        }
    }
    return 0;
}

int main() {

    const char palabras[NUM_LETRAS] = {'a','b','c','d','e'};
    char matriz[MATRIZ_TAM][MATRIZ_TAM];


    for (int i = 0; i < MATRIZ_TAM; i++) {
        for (int j = 0; j < MATRIZ_TAM; j++) {
            matriz[i][j] = 'a' + ( (i + j) % 26 );
        }
    }
    
    printMatriz(matriz);

    
    pid_t pid;
    int fd[2];
    
    if (pipe(fd) < 0) {
		perror("error en pipe");
		exit(1);
	}
    
    //paralelismo por tareas, cada proceso busca un caracter
    for (int i=0; i<NUM_LETRAS; i++) {
        pid = fork();
        if (pid == 0) {
			close(fd[READ_END]);
            char letra = palabras[i];
            BuscarLetra(letra, matriz, fd[WRITE_END]);
            close(fd[WRITE_END]);
            exit(0);
        } 
        else if (pid == -1) {
            perror("error al crear hijo");
            exit(1);
        }
    }
    
    close(fd[WRITE_END]);
    
    int encontrado[NUM_LETRAS] = {0,0,0,0,0};
    char letra;
    struct pipemsg msg;
    
    //PREGUNTAR SOBRE IPC_NOWAIT
    
    while (read(fd[READ_END], &msg, SIZE_PIPE) > 0) {
		letra = msg.pchar;
		printf("Soy el padre %d, recibi un mensaje de la letra %c\n",getpid(), letra);
		fflush(stdout);
		
		for (int i=0; i<NUM_LETRAS; i++) {
			if (letra == palabras[i]) {
				encontrado[i] += 1;
			}
		}
	}
    
    close(fd[READ_END]);
    
    for (int i=0; i<NUM_LETRAS; i++) {
        wait(NULL);
    }
    
    printf("Se finalizo la busqueda, los resultados son: \n");
    fflush(stdout);
    for (int i=0; i<NUM_LETRAS; i++) {
        printf("Letra %c: %d\n", palabras[i], encontrado[i]);
        fflush(stdout);
    }

    return 0;
}