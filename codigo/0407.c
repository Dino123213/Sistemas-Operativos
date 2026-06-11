#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#define NUM_SILLAS 5
#define NUM_CLIENTES 10

sem_t sillas; //semáforo para sillas vacías
sem_t clientes; //semáforo para sillas llenas
sem_t sem_barbero; //semáforo para el barbero
pthread_mutex_t mutex; //mutex para proteger el acceso a las sillas

void* cliente(void* arg) {
    //obtengo el numero del cliente
    int id = *((int*)arg);
    pthread_mutex_lock(&mutex);
    //implementación de la función cliente
    if(sem_trywait(&sillas) == 0) {
        //el cliente se sienta y espera ser atendido
        printf("El cliente %d se ha sentado en una silla.\n", id);
        sem_post(&sem_barbero); //despertar al barbero
        pthread_mutex_unlock(&mutex);
        sem_wait(&clientes);
        printf("Soy el cliente %d y ya he sido atendido y me voy.\n", id);
        return NULL;
    }
    else {
        //no hay sillas disponibles, el cliente se va
        printf("El cliente %d no encontró silla y se va.\n", id);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
}


void* barbero() {
    while(1) {
        sem_wait(&sem_barbero);
        pthread_mutex_lock(&mutex);
        sem_post(&sillas); //liberar la silla después de atender
        printf("El barbero está atendiendo a un cliente.\n");
        sleep(10); //simular tiempo de corte de cabello
        sem_post(&clientes); //dejo que el cliente se vaya
        pthread_mutex_unlock(&mutex);
    }
}
int main() {
    //inicialización de semaforos
    sem_init(&sillas, 0, NUM_SILLAS);
    sem_init(&clientes, 0, 0);
    sem_init(&sem_barbero, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    //crear hilos de clientes y barbero
    pthread_t barber_thread;
    pthread_create(&barber_thread, NULL, &barbero, NULL);

    pthread_t client_threads[NUM_CLIENTES];
    int ids[NUM_CLIENTES];

    for(int i = 0; i < NUM_CLIENTES; i++) {
        ids[i] = i;
        pthread_create(&client_threads[i], NULL, &cliente, &ids[i]);
    }

    //esperar a que los hilos terminen
    pthread_join(barber_thread, NULL);
    for(int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(client_threads[i], NULL);
    }

    return 0;
}