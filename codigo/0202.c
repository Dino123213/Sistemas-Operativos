#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // fork(), getpid(), getppid()
#include <sys/wait.h>  // wait(), waitpid()

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]); // Número de procesos hijos

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return 1;
        }

        if (pid == 0) {
            // **Proceso hijo**
            printf("HIJO %d: PID=%d, PPID=%d\n", i, getpid(), getppid());
            exit(0); // El hijo termina aquí
        }
    }

    // **Proceso padre espera a TODOS los hijos**
    for (int i = 0; i < N; i++) {
        pid_t wpid = wait(NULL);
        printf("PADRE: Hijo con PID %d terminó.\n", wpid);
    }

    printf("PADRE: Todos los hijos han terminado. Fin.\n");
    return 0;
}
