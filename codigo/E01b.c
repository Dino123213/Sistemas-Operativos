#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    //creo dos hijos
    pid_t hijo1, hijo2;
    int status1, status2;
    
    printf("Proceso padre iniciado (PID: %d)\n", getpid());
    
    //primer hijo - ejecutar comando ls
    hijo1 = fork();
    if (hijo1 == 0) {
        //cargo imagen ejecutable (listarDirectorio) para realizar la tarea
        printf("Hijo 1 ejecutando listarDirectorio (PID: %d)\n", getpid());
        execlp("./listarDirectorio", "listarDirectorio", NULL);

        //si llegamos aquí, execlp falló
        perror("execlp falló para listarDirectorio");
        exit(1);

    } else if (hijo1 < 0) {
        perror("fork falló para hijo1");
        exit(1);
    }
    
    //segundo hijo - ejecutar programa contador
    hijo2 = fork();
    if (hijo2 == 0) {
        //cargo imagen ejecutable (contador) para realizar la tarea
        printf("Hijo 2 ejecutando contador (PID: %d)\n", getpid());
        execlp("./contador", "contador", NULL);

        //si llegamos aquí, execlp falló
        perror("execlp falló para contador");
        exit(1);
        
    } else if (hijo2 < 0) {
        perror("fork falló para hijo2");
        exit(1);
    }

    //esperar al primer hijo
    waitpid(hijo1, &status1, 0);
    printf("Hijo 1 (listarDirectorio) ha terminado con estado: %d\n", status1);
    
    //esperar al segundo hijo
    waitpid(hijo2, &status2, 0);
    printf("Hijo 2 (contador) ha terminado con estado: %d\n", status2);
    
    printf("Ambos procesos hijos han terminado. Proceso padre finalizado.\n");
    
    return 0;
}