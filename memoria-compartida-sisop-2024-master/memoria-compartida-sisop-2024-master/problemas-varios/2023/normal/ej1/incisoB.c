#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>

#define NUM_PRODUCTORES 3
#define MAX 10
#define N 10 // random() % 100

#define COLOR_ROJO "\x1b[31m"
#define COLOR_VERDE "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_AZUL "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

int buffer[MAX];
sem_t huecos_vacio, elementos_lleno, n_iteraciones;
pthread_mutex_t mutex;
int pos_productor = 0;

int generarDato(void) {
    return random() % 256;
}

void* productor(void* p) {
    int id = *(int*)p;
    int dato;
    printf(COLOR_AZUL "------ Productor %d comienza a producir ------\n" COLOR_RESET, id);
    while (1) {
        if(sem_trywait(&n_iteraciones) == 0) {
            dato = generarDato();
            sem_wait(&huecos_vacio);
            pthread_mutex_lock(&mutex); 
                printf(COLOR_VERDE "Productor %d: agregando en la posición %d el dato %d.\n" COLOR_RESET, id, pos_productor, dato);
                buffer[pos_productor] = dato;
                pos_productor = (pos_productor + 1) % MAX;
            pthread_mutex_unlock(&mutex); 
            sem_post(&elementos_lleno);
            sleep(1);
        }    
        else {
            printf(COLOR_AZUL "Productor %d deja de producir.\n" COLOR_RESET, id);
            pthread_exit(NULL);
        }
    }
} 

void* consumidor(void* p) {
    int pos_consumidor = 0;
    int dato;
    for(int i = 0; i < N; i++) {
        sem_wait(&elementos_lleno);
        pthread_mutex_lock(&mutex); 
            dato = buffer[pos_consumidor];
            printf(COLOR_AMARILLO "Consumidor: consumiendo de la posición %d el dato %d.\n" COLOR_RESET, pos_consumidor, dato);
        pthread_mutex_unlock(&mutex); 
        pos_consumidor = (pos_consumidor + 1) % MAX;
        sem_post(&huecos_vacio);
        sleep(1);
    }
    printf(COLOR_ROJO "Consumidor deja de consumir.\n" COLOR_RESET);
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));  
    
    pthread_t hilos_productores[NUM_PRODUCTORES];
    pthread_t hilo_consumidor;
    int ids_productores[NUM_PRODUCTORES];

    sem_init(&huecos_vacio, 0, MAX);
    sem_init(&elementos_lleno, 0, 0);
    sem_init(&n_iteraciones, 0, N);
    pthread_mutex_init(&mutex, NULL);

    printf("---------- Se producirá %d cantidad de veces ---------- \n\n", N);

    for (int i = 0; i < NUM_PRODUCTORES; i++) {
        ids_productores[i] = i + 1;  // IDs empiezan desde 1
        pthread_create(&hilos_productores[i], NULL, productor, &ids_productores[i]);
    }

    pthread_create(&hilo_consumidor, NULL, consumidor, NULL);
    for (int i = 0; i < NUM_PRODUCTORES; i++) {
        pthread_join(hilos_productores[i], NULL);
    }
    pthread_join(hilo_consumidor, NULL);

    sem_destroy(&huecos_vacio);
    sem_destroy(&elementos_lleno);
    sem_destroy(&n_iteraciones);
    pthread_mutex_destroy(&mutex);

    printf("Programa terminado\n");

    return 0;
}
