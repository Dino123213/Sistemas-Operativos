#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PAJAROS 10
#define PORCIONES 5

sem_t vacio;     // controla las porciones disponibles
sem_t padre;     // sincroniza al padre cuando debe llenar
sem_t mutex;     // protege acceso al plato
sem_t llenando; // protege el llenado del plato

void* consumir(void* arg) {
    int id = (int)arg;

    while (1) {
        sem_wait(&llenando);//Si no esta llenando intento consumir
        sem_wait(&mutex);//Entro a la seccion critica
        sem_wait(&vacio);//Consumo una porcion
        if (sem_trywait(&vacio) == 0) {
            sem_post(&llenando);//Habilito a que otro pajarito intente consumir xq hay porciones
            printf("Pájaro %d está comiendo.\n", id);
            sem_post(&vacio); // Devuelvo la porcion consumida
            sem_post(&mutex);
            sleep(1);
        } else {
            printf("Pájaro %d: el plato está vacío, llamo a mi padre.\n", id);
            sem_post(&padre);
            sem_post(&mutex);
            sleep(1); // espera mientras el padre llena
        }
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

int main() {
    pthread_t pajaritos[PAJAROS];
    pthread_t padrepajarito;
 

    sem_init(&vacio, 0, PORCIONES);
    sem_init(&padre, 0, 0);
    sem_init(&mutex, 0, 1);
    sem_init(&llenando, 0, 1);

    pthread_create(&padrepajarito, NULL, llenar, NULL);

    for (int i = 0; i < PAJAROS; i++) {
        pthread_create(&pajaritos[i], NULL, consumir, (void*)i);
    }

    for (int i = 0; i < PAJAROS; i++) {
        pthread_join(pajaritos[i], NULL);
    }
    pthread_join(padrepajarito, NULL);

    sem_destroy(&vacio);
    sem_destroy(&padre);
    sem_destroy(&mutex);
    sem_destroy(&llenando);
    return 0;
}