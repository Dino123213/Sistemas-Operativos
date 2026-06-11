#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10
#define M 20

sem_t vacios[N]; //binario, 1 indica que la porción está disponible para ser llenada por una abeja
sem_t llenos[N]; //0 indica vacia, 1 indica llena (la abeja hace post cuando llene)

void *abejas(void *arg) {
    int i = *((int *)arg);
    while (1) {
        //esperamos a que la porción esté disponible para llenar
        sem_wait(&vacios[i]);
        printf("Abeja %d llenando porción %d del tarro\n", i, i);
        sleep(1); // simulamos que llena la porción
        sem_post(&llenos[i]); //indicamos que la porción está llena
    }
}

void *oso() {
    while (1) {
        //el oso espera a que todas las porciones estén llenas
        for (int i = 0; i < N; i++) {
            sem_wait(&llenos[i]);
        }
        printf("Oso comiendo del tarro lleno\n"); //simulamos el consumo del tarro
        sleep(2);
        //liberamos las porciones para que las abejas puedan llenarlas de nuevo
        for (int i = 0; i < N; i++) {
            sem_post(&vacios[i]);
        }
    }
}

int main() {
    pthread_t abeja_threads[N], oso_thread;
    int abeja_ids[N];

    //inicializamos los semáforos
    for (int i = 0; i < N; i++) {
        sem_init(&vacios[i], 0, 1); //porción disponible inicialmente
        sem_init(&llenos[i], 0, 0); //porción vacía inicialmente
        abeja_ids[i] = i;
    }

    //creamos las hebras de las abejas
    for (int i = 0; i < N; i++) {
        pthread_create(&abeja_threads[i], NULL, abejas, &abeja_ids[i]);
    }

    //creamos la hebra del oso
    pthread_create(&oso_thread, NULL, oso, NULL);

    //esperamos a que las hebras terminen (no lo harán en este ejemplo)
    for (int i = 0; i < N; i++) {
        pthread_join(abeja_threads[i], NULL);
    }
    pthread_join(oso_thread, NULL);

    //destruimos los semáforos
    for (int i = 0; i < N; i++) {
        sem_destroy(&vacios[i]);
        sem_destroy(&llenos[i]);
    }

    return 0;
}
