#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // pthread_create, pthread_join

// Función que ejecutará cada hilo
void* tarea_hilo(void* arg) {
    int id = *(int*)arg; // Cast del argumento recibido
    printf("HILO %d: Estoy ejecutando. ID interno: %lu\n", id, pthread_self());
    pthread_exit(NULL); // Fin del hilo
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <M>\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);
    pthread_t hilos[M];
    int ids[M];

    // Crear M hilos
    for (int i = 0; i < M; i++) {
        ids[i] = i;
        if (pthread_create(&hilos[i], NULL, tarea_hilo, &ids[i]) != 0) { 
            //&hilos[i] es la dirección del identificador del hilo
            // NULL para atributos por defecto
            // tarea_hilo es la función que ejecutará el hilo
            // &ids[i] es el argumento pasado a la función del hilo
            perror("Error creando hilo");
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < M; i++) {
        pthread_join(hilos[i], NULL);
        // Espera a que el hilo hilos[i] termine
        // NULL porque no nos interesa el valor de retorno del hilo
    }

    printf("MAIN: Todos los hilos han terminado.\n");
    return 0;
}
