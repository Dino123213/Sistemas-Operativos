#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semA, semB, semC;
//ABBCBBABBC
// Función para el hilo A
void* printA(void* arg) {
    while(1) {
            sem_wait(&semA);
            sem_wait(&semA);
            printf("A");
            sem_post(&semB);
            sem_post(&semB);
    }
}

// Función para el hilo B
void* printB(void* arg) {
    while(1) {
        for(int i=0;i <=1; i++){
                sem_wait(&semB);
                printf("B");
                sem_post(&semC);
        }



        for(int i=0;i <=1; i++){
                sem_wait(&semB);
                printf("B");
                sem_post(&semA);
        }




    }
}

// Función para el hilo C
void* printC(void* arg) {
    while(1) {
        sem_wait(&semC);
        sem_wait(&semC);
        printf("C");
        sem_post(&semB);
        sem_post(&semB);
    }

}

int main() {
    pthread_t threadA, threadB, threadC;

    // Inicialización de los semáforos
    sem_init(&semA, 0, 2);  // A empieza
    sem_init(&semB, 0, 0);  // B espera
    sem_init(&semC, 0, 0);  // C espera

    // Creación de los hilos
    pthread_create(&threadA, NULL, printA, NULL);
    pthread_create(&threadB, NULL, printB, NULL);
    pthread_create(&threadC, NULL, printC, NULL);

    // Espera a que los hilos terminen (nunca sucederá en este caso)
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    // Destrucción de los semáforos
    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);


    return 0;
}
