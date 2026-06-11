#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define LECTURA 0
#define ESCRITURA 1

#define MAX 256
#define TAM sizeof(char[MAX])

int pipeA[2]; 
int pipeB[2]; 
int pipeC[2];

/*
        Lectura         Escritura
    close(pipeA[0]); close(pipeA[1]);
    close(pipeB[0]); close(pipeB[1]);
    close(pipeC[0]); close(pipeC[1]);
*/

void procesoA() {
    char mensaje[MAX];

    // read(pipeA[0], &mensaje, TAM);

    // Preparar el mensaje

	// write(pipeA[1], &mensaje, TAM);
}

void procesoB() {
    // read(pipeB[0], &mensaje, TAM);

    // Preparar el mensaje

	// write(pipeB[1], &mensaje, TAM);
}

int main() {
    pid_t pidA, pidB, pidC;

    if(pipe(pipeA) == -1 || pipe(pipeB) == -1 || pipe(pipeC) == -1) {
        fprintf(stderr, "Creación de pipes fallida.");
        return 1;
    }

    pidA = fork();
    if(pidA == 0) {
        procesoA();
        exit(0);
    } else if(pidA < 0) {
        perror("Error en la creación del proceso hijo.");
        exit(1);
    }

    pidB = fork();
    if(pidB == 0) {
        procesoB();
        exit(0);
    } else if(pidB < 0) {
        perror("Error en la creación del proceso hijo.");
        exit(1);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}

