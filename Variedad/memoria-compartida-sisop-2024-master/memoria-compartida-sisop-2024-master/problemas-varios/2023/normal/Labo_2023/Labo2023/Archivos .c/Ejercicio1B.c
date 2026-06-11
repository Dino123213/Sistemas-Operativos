#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>

#define MAX 10
#define FIN -1

int buffer[MAX];
sem_t huecos, elementos;

int generardato(void) {
    return (rand() % 256);
}

int numeroaleatorio(void) {
    return (rand() % 100);
}

void *productor(void *p) {
    int pos_productor = 0;
    int num, dato, n;
    n = numeroaleatorio();

    printf("Productor con %d datos\n", n);

    for (num = 0; num < n; num++) {
        dato = generardato();

        sem_wait(&huecos);

        buffer[pos_productor] = dato;
        pos_productor = (pos_productor + 1) % MAX;

        sem_post(&elementos);
    }

    buffer[pos_productor] = FIN;
    pthread_exit(NULL);
}

void *consumidor(void *p) {
    int pos_consumidor = 0;
    int dato;
    bool continuar = true;

    while (continuar) {
        sem_wait(&elementos);
        dato = buffer[pos_consumidor];
        pos_consumidor = (pos_consumidor + 1) % MAX;
        if (dato == FIN)
            continuar = false;
        sem_post(&huecos);
        printf("Numero aleatorio: %d\n", dato);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    sem_init(&huecos, 0, MAX);
    sem_init(&elementos, 0, 0);

    //Inicializo hilos
    pthread_t prod_threads[3];  // Arreglo de tres hilos de productor
    pthread_t cons_thread;      //un hilo consumidor


    //creacion de hilos
    for (int i = 0; i < 3; i++)
        pthread_create(&prod_threads[i], NULL, productor, NULL);

    pthread_create(&cons_thread, NULL, consumidor, NULL);


    //Finalización ordenada de hilos
    for (int i = 0; i < 3; i++) {
        pthread_join(prod_threads[i], NULL);
    }

    pthread_join(cons_thread, NULL);

    //destruir semaforos
    sem_destroy(&huecos);
    sem_destroy(&elementos);

    return 0;
}
//Corregido para que hayan 3 hilos productores
