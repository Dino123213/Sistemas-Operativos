#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semA, semB;
pthread_mutex_t mutex;

void *accionA(void *arg) {
    while (1) {
        /*
            MANEJO DE LÓGICA SEMAFORO
        */
    }
    pthread_exit(NULL);
}

void *accionB(void *arg) {
    while (1) {
        /*
            MANEJO DE LÓGICA SEMAFORO
        */
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t hiloA_t;
    pthread_t hiloB_t;

    sem_init(&semA, 0, 0);
    sem_init(&semB, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&hiloA_t, NULL, accionA, NULL);
    pthread_create(&hiloB_t, NULL, accionB, NULL);

    pthread_join(hiloA_t, NULL);
    pthread_join(hiloB_t, NULL);

    sem_destroy(&semA);
    sem_destroy(&semB);
    pthread_mutex_destroy(&mutex);

    return 0;
}