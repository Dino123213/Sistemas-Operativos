#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_JABALINA 5
#define NUM_MARTILLO 5
#define NUM_CORREDORES 10

sem_t corren;
sem_t marJab;
pthread_mutex_t mutex;

void* corredores() {
    while(1) {
        pthread_mutex_lock(&mutex);

        if(sem_trywait(&corren) == 0) {
            //no soy el primer corredor
            sem_post(&corren);
        }
        else {
            //soy el primer corredor
            sem_wait(&marJab);
        }

        sem_post(&corren); //entra el nuevo atleta

        pthread_mutex_unlock(&mutex);

        printf("Soy el corredor y estoy entrenando");
        sleep(rand() % 3 + 1); //simulo que corre

        pthread_mutex_lock(&mutex);

        sem_wait(&corren); //se va un atleta

        if(sem_trywait(&corren) == 0) {
            //hay más corredores
            sem_post(&corren);
        }
        else {
            //soy el último corredor
            sem_post(&marJab);
        }

        pthread_mutex_unlock(&mutex);
    }
}

void* jabalina() {
    while(1) {
        sem_wait(&marJab); //reclamo el semáforo
        printf("Soy el lanzador de jabalina y estoy entrenando");
        sleep(rand() % 3 + 1); //simulo entrenamiento
        sem_post(&marJab); // libero el semaforo
    }
}

void* martillo() {
    while(1) {
        sem_wait(&marJab); //reclamo el semáforo
        printf("Soy el lanzador de martillo y estoy entrenando");
        sleep(rand() % 3 + 1); //simulo entrenamiento
        sem_post(&marJab); // libero el semaforo
    }
}

int main() {
    srand((unsigned)time(NULL));

    sem_init(&marJab, 0, 1);
    sem_init(&corren, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t corredores_t[NUM_CORREDORES];
    pthread_t jabalina_t[NUM_JABALINA];
    pthread_t martillo_t[NUM_MARTILLO];

    for(int i = 0; i < NUM_CORREDORES; i++) {
        if(pthread_create(&corredores_t[i], NULL, corredores, NULL) != 0) {
            perror("pthread_create corredor");
            exit(1);
        }
    }

    for(int i = 0; i < NUM_JABALINA; i++) {
        if(pthread_create(&jabalina_t[i], NULL, jabalina, NULL) != 0) {
            perror("pthread_create jabalina");
            exit(1);
        }
    }

    for(int i = 0; i < NUM_MARTILLO; i++) {
        if(pthread_create(&martillo_t[i], NULL, martillo, NULL) != 0) {
            perror("pthread_create martillo");
            exit(1);
        }
    }

    for(int i = 0; i < NUM_CORREDORES; i++) pthread_join(corredores_t[i], NULL);
    for(int i = 0; i < NUM_JABALINA; i++) pthread_join(jabalina_t[i], NULL);
    for(int i = 0; i < NUM_MARTILLO; i++) pthread_join(martillo_t[i], NULL);

    sem_destroy(&marJab);
    sem_destroy(&corren);
    pthread_mutex_destroy(&mutex);

    return 0;
}