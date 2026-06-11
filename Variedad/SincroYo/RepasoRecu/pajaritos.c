#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PAJAROS 10
#define PORCIONES 5

sem_t vacio;     // controla las porciones disponibles
sem_t padre;     // sincroniza al padre cuando debe llenar
pthread_mutex_t mutex;     // protege acceso al plato
sem_t llenando; // protege el llenado del plato

void* consumir(void* arg) {
    int id = (int)arg;
    while (1) {
        sem_wait(&llenando);//Si no esta llenando intento consumir
        pthread_mutex_lock(&mutex);//Entro a la seccion critica
        sem_wait(&vacio);//Consumo una porcion
        printf("Pájaro %d está comiendo.\n", id);
        if (sem_trywait(&vacio) == 0) {
            sem_post(&llenando);//Habilito a que otro pajarito intente consumir xq hay porciones
            sem_post(&vacio); // Devuelvo la porcion consumida
        }else{
            printf("Pájaro %d: el plato está vacío, llamo a mi padre.\n", id);
            sem_post(&padre);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* llenar(void* arg) {
    while (1) {
        sem_wait(&padre);
        printf("Padre: desperté para llenar el plato.\n");
        sleep(2);
        for (int i = 0; i < PORCIONES; i++) {
            sem_post(&vacio);
        }
        printf("Padre: terminé de llenar el plato.\n");
        sem_post(&llenando);
    }
    return NULL;
}

int main(){

    pthread_t pajaritos[PAJAROS];
    pthread_t padrepajarito;
    

    sem_init(&vacio, 0, PORCIONES);
    sem_init(&padre, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    sem_init(&llenando, 0, 1);

    pthread_create(&padrepajarito,NULL,llenar,NULL);

    for(int i=0; i<PAJAROS; i++){
        pthread_create(&pajaritos[i],NULL,consumir,(void*) i);
    }
    pthread_join(padrepajarito, NULL);
    for(int i=0; i<PAJAROS; i++){
        pthread_join(pajaritos[i], NULL);
    }
    sem_destroy(&vacio);
    sem_destroy(&padre);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&llenando);
    return 0;
}