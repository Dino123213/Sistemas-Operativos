#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 5

int arr[N] = {1, 2, 3, 4, 5};

int main() {
    int fd[2]; // Pipe para comunicar padre-hijo
    pipe(fd);  // Creamos el pipe (fd[0] lectura, fd[1] escritura)

    pid_t pid = fork(); // Creamos un proceso hijo

    if (pid == 0) { // Código del hijo
        close(fd[0]); // El hijo no lee, solo escribe

        int suma_hijo = 0;
        for (int i = 0; i < N/2; i++)
            suma_hijo += arr[i];

        write(fd[1], &suma_hijo, sizeof(suma_hijo)); // Enviamos resultado al padre
        close(fd[1]);
        exit(0);
    } else { // Código del padre
        close(fd[1]); // El padre no escribe, solo lee

        int suma_padre = 0;
        for (int i = N/2; i < N; i++)
            suma_padre += arr[i];

        int suma_hijo;
        read(fd[0], &suma_hijo, sizeof(suma_hijo)); // Recibimos resultado del hijo
        close(fd[0]);

        wait(NULL); // Esperamos a que el hijo termine

        printf("Suma total = %d\n", suma_padre + suma_hijo);
    }

    return 0;
}
