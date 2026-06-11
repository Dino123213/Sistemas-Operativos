#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define COLOR_ROJO "\x1b[31m"
#define COLOR_VERDE "\x1b[92m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_NARANJA "\x1b[38;5;208m"
#define COLOR_RESET "\x1b[0m"

#define CANT_RENOS 9
#define CANT_ELFOS 10
#define MAX_ELFOS_PROBLEMAS 3

#define TIEMPO 10000 
#define TIEMPO_LLEGADA 200000

sem_t semSanta, semRenos, semElfos, cantRenos, cantElfos, semElfosSanta;
pthread_mutex_t mutexRenos, mutexElfos;

void* santa(void *arg) {
    while(1){
        sem_wait(&semSanta);
        printf(COLOR_ROJO "\nSanta es despertado!\n\n" COLOR_RESET);
        usleep(TIEMPO);
        
        pthread_mutex_lock(&mutexRenos); 
            if(sem_trywait(&cantRenos) == -1) { // renos despertaron a santa
                printf(COLOR_NARANJA "Llegaron todos los renos y santa prepara su trineo.\n" COLOR_RESET);
                usleep(TIEMPO);
                for(int i = 0; i < CANT_RENOS; i++) {
                    sem_post(&semRenos);
                    sem_post(&cantRenos);
                }
                printf(COLOR_NARANJA "Los renos hicieron su trabajo y vuelven de vacaciones.\n" COLOR_RESET);
                usleep(TIEMPO);
            }
            else { // elfos despertaron a santa
                sem_post(&cantRenos); 
                printf(COLOR_NARANJA "Santa ayuda a los tres elfos que pidieron ayuda.\n" COLOR_RESET);
                usleep(TIEMPO);
                for (int i = 0; i < MAX_ELFOS_PROBLEMAS; i++) {
                    sem_post(&cantElfos);
                    sem_post(&semElfosSanta);
                }
                printf(COLOR_NARANJA "Los 3 elfos resolvieron sus problemas y se van.\n\n" COLOR_RESET);
                sem_post(&semElfos);
            }
        pthread_mutex_unlock(&mutexRenos);
    }
    pthread_exit(NULL);
}

void* renos(void *arg) {
    while(1){
        pthread_mutex_lock(&mutexRenos);
            sem_wait(&cantRenos);
            usleep(TIEMPO);
            if(sem_trywait(&cantRenos) == 0) { 
                sem_post(&cantRenos); 
                printf(COLOR_AMARILLO "Vuelve un reno al Polo Norte luego de vacaciones.\n" COLOR_RESET);
            }
            else { 
                printf(COLOR_AMARILLO "Vuelve el último reno y va a despertar a Santa.\n\n" COLOR_RESET);
                sem_post(&semSanta);
            }
        pthread_mutex_unlock(&mutexRenos);
        sem_wait(&semRenos); 
        usleep(TIEMPO_LLEGADA); // simulación de llegada
    }
    pthread_exit(NULL); 
}

void* elfos(void *arg) {
    while(1){
        sem_wait(&semElfos);
        pthread_mutex_lock(&mutexElfos);
            sem_wait(&cantElfos);
            if(sem_trywait(&cantElfos) == 0) {
                sem_post(&semElfos);
                sem_post(&cantElfos);
                printf(COLOR_VERDE "Elfo en problemas para hacer juguetes!\n" COLOR_RESET);
                usleep(TIEMPO);
            }
            else {
                printf(COLOR_VERDE "Llega un tercer elfo en problemas, van a buscar a Santa.\n" COLOR_RESET);
                sem_post(&semSanta);
                usleep(TIEMPO);
            }
        pthread_mutex_unlock(&mutexElfos);
        sem_wait(&semElfosSanta);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t santa_t;
    pthread_t renos_t[CANT_RENOS], elfos_t[CANT_ELFOS];

    sem_init(&semSanta, 0, 0);
    sem_init(&semRenos, 0, 0);
    sem_init(&semElfos, 0, 1);
    sem_init(&semElfosSanta, 0, 0);
    sem_init(&cantRenos, 0, CANT_RENOS);
    sem_init(&cantElfos, 0, MAX_ELFOS_PROBLEMAS);

    pthread_mutex_init(&mutexRenos, NULL);
    pthread_mutex_init(&mutexElfos, NULL);

    pthread_create(&santa_t, NULL, santa, NULL);

    for(int i = 0; i < CANT_RENOS; i++) {
        pthread_create(&renos_t[i], NULL, renos, NULL);
    }

    for(int i = 0; i < CANT_ELFOS; i++) {
        pthread_create(&elfos_t[i], NULL, elfos, NULL);
    }

    pthread_join(santa_t, NULL);

    for(int i = 0; i < CANT_RENOS; i++) {
        pthread_join(renos_t[i], NULL);
    }

    for(int i = 0; i < CANT_ELFOS; i++) {
        pthread_join(elfos_t[i], NULL);
    }

    sem_destroy(&semSanta);
    sem_destroy(&semRenos);
    sem_destroy(&semElfos);
    sem_destroy(&cantRenos);
    sem_destroy(&cantElfos);
    sem_destroy(&semElfosSanta);

    pthread_mutex_destroy(&mutexRenos);
    pthread_mutex_destroy(&mutexElfos);

    return 0;
}