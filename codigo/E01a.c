#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    //creo dos hijos
    pid_t hijo1, hijo2;
    hijo1 = fork();
    if (hijo1 == 0) {
        //listar el directorio /etc
        execlp("ls", "ls", "/etc", NULL);
        exit(0);
    } else {
        hijo2 = fork();
        if (hijo2 == 0) {
            //Un contador de 1 a 10000
            for (int i = 1; i <= 10000; i++) {
                printf("%d\n", i);
            }
            exit(0);
        }
    }
    //el padre espera a los hijos e imprime mensaje al finalizar
    waitpid(hijo1, NULL, 0);
    waitpid(hijo2, NULL, 0);
    printf("Hijos finalizados\n");
    return 0;
}