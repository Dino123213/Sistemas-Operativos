#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];// fd[0] = lectura, fd[1] = escritura
    pid_t pid;
    char mensaje[] = "Tino mogul cerra el ortito.\n";
    char buffer[100];

    // Crear el pipe
    if (pipe(fd) == -1) {
        perror("Error al crear el pipe");
        exit(1);
    }

    // Crear proceso hijo
    pid = fork();

    if (pid < 0) {
        perror("Error en fork");
        exit(1);
    }

    if (pid == 0) {  
        // PROCESO HIJO
        close(fd[1]);  // Cierra el extremo de escritura,xq solo lee
        read(fd[0], buffer, sizeof(buffer));
        printf("Hijo recibió: %s", buffer);
        close(fd[0]);  // Cierra el extremo de lectura
    } else {  
        // PROCESO PADRE
        close(fd[0]);  // Cierra el extremo de lectura,xq solo escribe
        write(fd[1], mensaje, strlen(mensaje) + 1);
        close(fd[1]);  // Cierra el extremo de escritura
    }

    return 0;
}
