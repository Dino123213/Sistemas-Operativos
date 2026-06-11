#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define USERS 10

sem_t imp1, imp2;
pthread_mutex_t mutex;

void liberar(int id, int n) {
    if(n == 1) {
        sem_post(&imp1);
        printf("Usuario %d libera impresora 1\n", id);
    } else if(n == 2) {
        sem_post(&imp2);
        printf("Usuario %d libera impresora 2\n", id);
    }
}

void imprimir(int id) {
    printf("Usuario %d imprimiendo...\n", id);
    sleep(rand() % 3);
    printf("Usuario %d termina de imprimir.\n", id);
}

int requerir(int id) {
    while(1) {
        pthread_mutex_lock(&mutex);
        if(sem_trywait(&imp1) == 0) {
            printf("Usuario %d obtiene impresora 1\n", id);
            pthread_mutex_unlock(&mutex);
            return 1;
        } else if(sem_trywait(&imp2) == 0) {
            printf("Usuario %d obtiene impresora 2\n", id);
            pthread_mutex_unlock(&mutex);
            return 2;
        }
        pthread_mutex_unlock(&mutex);
    }
}

void* usuario(void* arg) {
    while(1) {
        int id = *((int*)arg);
        int n = requerir(id); //que impresora recibo
        imprimir(id);
        liberar(id, n);
    }
}

int main() {
    //inicializo semaforos
    sem_init(&imp1, 0, 1);
    sem_init(&imp2, 0, 1);

    //inicializo hilos
    pthread_t threads[USERS];
    int ids[USERS];
    for(int i = 0; i < USERS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, usuario, (void*)&ids[i]);
    }

    //espero que terminen los hilos
    for(int i = 0; i < USERS; i++) {
        pthread_join(threads[i], NULL);
    }

    //destruyo semaforos
    sem_destroy(&imp1);
    sem_destroy(&imp2);

    return 0;
}