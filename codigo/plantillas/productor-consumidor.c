#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

sem_t empty; //cuenta los espacios vacíos en el buffer
sem_t full;  //cuenta los ítems en el buffer
pthread_mutex_t mutex; //protege el acceso al buffer

/*Esta solución produce y consume ítems de a uno*/

void* productor(void* arg) {
    //produce un ítem

    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    //añade el ítem al buffer

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

void* consumidor(void* arg) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    //retira el ítem del buffer

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    //consume el ítem
}