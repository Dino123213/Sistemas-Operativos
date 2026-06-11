#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define time 0
sem_t semA, semB, semC;
// ABBCBBABBC
//  Funci�n para el hilo A
void *printA(void *arg){
    while (1){
        sem_wait(&semA);
        sem_wait(&semA);
        sem_wait(&semA);
        sem_wait(&semA);
        printf("\nA");
        usleep(time);
        sem_post(&semB);
        sem_post(&semB);
    }
}

// Funci�n para el hilo B
void *printB(void *arg){
    while (1){
        sem_wait(&semB);
        printf("B");
        usleep(time);
        sem_post(&semA);
        sem_post(&semC);
    }
}

// Funci�n para el hilo C
void *printC(void *arg){
    while (1){
        sem_wait(&semC);
        sem_wait(&semC);
        sem_wait(&semC);
        sem_wait(&semC);
        printf("C");
        usleep(time);
        sem_post(&semB);
        sem_post(&semB);
    }
}

int main(){
    pthread_t threadA, threadB, threadC;

    // Inicializaci�n de los sem�foros
    sem_init(&semA, 0, 4); // A empieza
    sem_init(&semB, 0, 0); // B espera
    sem_init(&semC, 0, 2); // C espera

    // Creaci�n de los hilos
    pthread_create(&threadA, NULL, printA, NULL);
    pthread_create(&threadB, NULL, printB, NULL);
    pthread_create(&threadC, NULL, printC, NULL);

    // Espera a que los hilos terminen (nunca suceder� en este caso)
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    // Destrucción de los sem�foros
    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);

    return 0;
}
