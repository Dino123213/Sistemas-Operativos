#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void main() {

    int pipe1[2], pipe2[2];  // pipe1: padre->hijo, pipe2: hijo->padre
    pid_t pid;
    char mensaje_padre[] = "Mensaje desde padre";
    char mensaje_hijo[] = "Mensaje desde hijo";
    char buffer[100];
    
    // Crear ambos pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Error creando pipes");
        exit(1);
    }
    
    pid = fork();
    
    if (pid == 0) {  // Proceso hijo
        close(pipe1[1]);  // Cerrar escritura del pipe1
        close(pipe2[0]);  // Cerrar lectura del pipe2
        
        // Leer mensaje del padre
        read(pipe1[0], buffer, sizeof(buffer));
        printf("Hijo recibió: %s\n", buffer);
        
        // Enviar respuesta al padre
        write(pipe2[1], mensaje_hijo, strlen(mensaje_hijo) + 1); //Se agrega uno por el null terminator
        
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    } else {  // Proceso padre
        close(pipe1[0]);  // Cerrar lectura del pipe1
        close(pipe2[1]);  // Cerrar escritura del pipe2
        
        // Enviar mensaje al hijo
        write(pipe1[1], mensaje_padre, strlen(mensaje_padre) + 1);
        
        // Leer respuesta del hijo
        read(pipe2[0], buffer, sizeof(buffer));
        printf("Padre recibió: %s\n", buffer);
        
        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL);
    }
}