#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // fork(), getpid(), getppid()
#include <sys/wait.h>  // wait(), waitpid()

#define CANT_PROC 3


void escribirPipe(char matriz [10][10]) {
    
}

int leerPares() {
    
}

int leeImpares() {
    
}


int main(){

    int i;
    pid_t pid;
    int pares=0;
    int impares=0;
    char matriz [10][10]; 
    int fdP[2];
    int fdI[2];
    pipe(fdP);
    pipe(fdI);
    escribirPipe(matriz);
      

    pid = fork();
    if (pid < 0) {
        perror("Fork falló");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        pares=leerPares();
        exit(0);
    }

    pid = fork();
    if (pid < 0) {
        perror("Fork falló");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        impares=leeImpares();
        exit(0);
    }

    // Proceso padre
    for(i=0; i<CANT_PROC; i++){
        wait(NULL); // Espera a que cada hijo termine
    }
    printf("Todos los procesos hijos han terminado. Proceso padre PID: %d finalizando.\n", getpid());

    return 0;

}