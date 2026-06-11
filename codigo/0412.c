#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CONEXIONES 5
#define CLIENTES 15

sem_t conexiones;

void* cliente(void* arg) {
    while(1) {
        int cliente_id = *((int*)arg);
        sem_wait(&conexiones);
        printf("Cliente %d conectado.\n", cliente_id);
        sleep(rand() % 5 + 1); //simula tiempo de conexión
        printf("Cliente %d desconectado.\n", cliente_id);
        sem_post(&conexiones);
    }
}

int main() {
    pthread_t hilos[CLIENTES];
    int cliente_ids[CLIENTES];
    sem_init(&conexiones, 0, MAX_CONEXIONES);

    for (int i = 0; i < CLIENTES; i++) {
        cliente_ids[i] = i + 1;
        pthread_create(&hilos[i], NULL, cliente, &cliente_ids[i]);
    }

    for (int i = 0; i < CLIENTES; i++) {
        pthread_join(hilos[i], NULL);
    }

    sem_destroy(&conexiones);
    return 0;
}