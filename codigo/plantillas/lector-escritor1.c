#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t mutex; //protege readcount
sem_t wrt;   //permite el acceso al recurso compartido
int readcount = 0; //número de lectores activos

/*Ningún lector se mantiene en espera a menos que un escritor haya obtenido ya permiso para utilizar el objeto compartido.*/

void* escritor(void* arg) {
    while(1) {
        sem_wait(&wrt); //se prepara para escribir

        //se realiza la escritura

        sem_post(&wrt); //termina de escribir
    }
}

void* lector(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex); //permiso para modificar readcount
        readcount++;
        if(readcount == 1)
            sem_wait(&wrt); //el primer lector bloquea al escritor
        pthread_mutex_unlock(&mutex); //libera el permiso para modificar readcount

        //se realiza la lectura

        pthread_mutex_lock(&mutex); //permiso para modificar readcount
        readcount--;
        if(readcount == 0)
            sem_post(&wrt); //el último lector libera al escritor
        pthread_mutex_unlock(&mutex); //libera el permiso para modificar readcount
    }
}