#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define number 10
#define MAX_CONEXIONES 3

sem_t conexiones;

void* conectar(void* arg){
    int id = (int)arg;
    while(1){
    printf("Usuario %d intentando conectar...\n", id);
    sem_wait(&conexiones);
    printf("Usuario %d conectado.\n", id);
    sleep(5); // Simula tiempo de uso del recurso
    printf("Usuario %d desconectando...\n", id);
    sem_post(&conexiones);
    printf("Usuario %d desconectado.\n", id);
    }
    sleep(1);
    return NULL;
}

int main(){
    pthread_t usuarios[number];

    if (sem_init(&conexiones, 0, MAX_CONEXIONES) != 0) {
        perror("sem_init");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < number ; i++) {
        if (pthread_create(&usuarios[i], NULL, conectar, (void*)i) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }


    for (int i = 0; i < number ; i++) {
        pthread_join(usuarios[i], NULL);
    }

    sem_destroy(&conexiones);
    return 0;
}