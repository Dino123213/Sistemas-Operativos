/*
Escriba un algoritmo con semaforos, que controle el acceso a un archivo, de tal manera
que se permita el acceso en lectura a varios procesos o a un solo escritor en forma
exclusiva.ssss
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define LECTORES 10
#define ESCRITORES 5

pthread_mutex_t mutex;
sem_t escritor;
sem_t lectores;

void* lector(void* arg) {
    //while(1) {
        pthread_mutex_lock(&mutex);
        if (sem_trywait(&lectores) == 0) {
            //no es el primer lector
            sem_post(&lectores); //normalizo el contador de lectores
            sem_post(&lectores); //sumo al nuevo lector
        }
        else {
            //soy el primer lector
            sem_wait(&escritor); //si es el primer lector bloqueo al escritor
            sem_post(&lectores); //sumo al nuevo lector
        }
        pthread_mutex_unlock(&mutex);

        //sección de lectura
        printf("Lector %ld leyendo\n", (long)arg);
        sleep(2); //simula el tiempo de lectura

        pthread_mutex_lock(&mutex);
        sem_wait(&lectores); //decremento el numero de lectores
        if(sem_trywait(&lectores) == 0) {
            sem_post(&lectores); //normalizo el contador de lectores
            printf("Lector %ld saliendo, quedan lectores\n", (long)arg);
        }
        else {
            //soy el ultimo lector
            printf("Lector %ld saliendo, no quedan lectores\n", (long)arg);
            sem_post(&escritor); //libero al escritor
        }
        pthread_mutex_unlock(&mutex);
    //}
    return NULL;
}

void* escritores(void* arg) {
    //while(1) {
        sem_wait(&escritor); //pido permiso para escribir

        //sección de escritura
        printf("Escritor %ld escribiendo\n", (long)arg);
        sleep(2); //simula el tiempo de escritura

        sem_post(&escritor); //libero el permiso para escribir
    //}
    return NULL;
}

int main() {
    pthread_t lectores_t[5], escritores_t[2];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&escritor, 0, 1); //semáforo para el escritor
    sem_init(&lectores, 0, 0); //semáforo para contar lectores

    for(long i = 0; i < LECTORES; i++) {
        pthread_create(&lectores_t[i], NULL, lector, (void*)i);
    }
    for(long i = 0; i < ESCRITORES; i++) {
        pthread_create(&escritores_t[i], NULL, escritores, (void*)i);
    }

    for(int i = 0; i < LECTORES; i++) {
        pthread_join(lectores_t[i], NULL);
    }
    for(int i = 0; i < ESCRITORES; i++) {
        pthread_join(escritores_t[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&escritor);
    sem_destroy(&lectores);

    return 0;
}