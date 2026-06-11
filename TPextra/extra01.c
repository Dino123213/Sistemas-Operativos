#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    // Primer hijo → listar /etc
    pid1 = fork();
    if (pid1 == 0) {
        execlp("ls", "ls", "/etc", NULL);
        perror("execlp"); // solo si falla
        exit(1);
    }

    // Segundo hijo → contador
    pid2 = fork();
    if (pid2 == 0) {
        for (int i = 1; i <= 10000; i++) {
            printf("Contador: %d\n", i);
        }
        exit(0);
    }

    // Padre espera a ambos
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Ambos hijos finalizaron.\n");
    return 0;
}