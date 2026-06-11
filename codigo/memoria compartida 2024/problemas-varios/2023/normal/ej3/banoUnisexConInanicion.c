#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define CANT_MUJERES 10
#define CANT_HOMBRES 10

sem_t semMujer, semHombre, banoDisponible;
pthread_mutex_t mutex_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_h = PTHREAD_MUTEX_INITIALIZER;

void* mujeres(void* arg) {
    while(1) {
        // Sección de Entrada
        pthread_mutex_lock(&mutex_m);
            if(sem_trywait(&semMujer) == 0) {
                sem_post(&semMujer);
            }
            else {
                sem_wait(&banoDisponible); // Deshabilitamos el salón para hombres
            }
            sem_post(&semMujer); // entra una mujer
            printf("\033[1;35mMujer entra al baño\033[0m\n"); // Mensaje en color rosa
            usleep(10000);
        pthread_mutex_unlock(&mutex_m);

        sleep(1); // Simula el tiempo que pasa en el baño

        // Sección de Salida
        pthread_mutex_lock(&mutex_m);
            sem_wait(&semMujer); // se va una mujer
            printf("\033[0;35mMujer sale del baño\033[0m\n"); // Mensaje en color rosa
            usleep(10000);
            if(sem_trywait(&semMujer) == 0) {
                sem_post(&semMujer);
            }
            else {
                sem_post(&banoDisponible); // Habilitamos el salón nuevamente para hombres también
            }
        pthread_mutex_unlock(&mutex_m);
    }
        return NULL;
}

void* hombres(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex_h);
            if(sem_trywait(&semHombre) == 0) {
                sem_post(&semHombre);
            }
            else {
                sem_wait(&banoDisponible); // Deshabilitamos el salón para mujeres
            }
            sem_post(&semHombre); // entra un hombre
            printf("\033[1;34mHombre entra al baño\033[0m\n"); // Mensaje en color azul
            usleep(10000);
        pthread_mutex_unlock(&mutex_h);

        sleep(1); // Simula el tiempo que pasa en el baño

        // Sección de Salida
        pthread_mutex_lock(&mutex_h);
            sem_wait(&semHombre); // se va un hombre    
            printf("\033[0;34mHombre sale del baño\033[0m\n"); // Mensaje en color azul
            usleep(10000);
            if(sem_trywait(&semHombre) == 0) {
                sem_post(&semHombre);
            }
            else {
                sem_post(&banoDisponible); // Habilitamos el salón nuevamente para mujeres también
            }
        pthread_mutex_unlock(&mutex_h);
    }
    return NULL;
}

int main() {
    pthread_t hiloHombres[CANT_HOMBRES], hiloMujeres[CANT_MUJERES];

    // Inicialización de los semáforos
    sem_init(&semMujer, 0, 0);
    sem_init(&semHombre, 0, 0);
    sem_init(&banoDisponible, 0, 1);

    for(int i = 0; i < CANT_MUJERES; i++) {
        pthread_create(&hiloMujeres[i], NULL, mujeres, NULL);
    }

    for(int i = 0; i < CANT_HOMBRES; i++) {
        pthread_create(&hiloHombres[i], NULL, hombres, NULL);
    }

    for(int i = 0; i < CANT_MUJERES; i++) {
        pthread_join(hiloMujeres[i], NULL);
    }

    for(int i = 0; i < CANT_HOMBRES; i++) {
        pthread_join(hiloHombres[i], NULL);
    }

    sem_destroy(&semMujer);
    sem_destroy(&semHombre);
    sem_destroy(&banoDisponible);

    pthread_mutex_destroy(&mutex_m);
    pthread_mutex_destroy(&mutex_h);

    return 0;
}
