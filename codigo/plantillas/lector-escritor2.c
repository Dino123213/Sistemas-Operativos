#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

//configuración: puede ajustarse a gusto
#define NUM_READERS 5
#define NUM_WRITERS 3

/*Siempre que en escritor está listo realiza la escritura lo más rápido posible.*/

pthread_mutex_t rw_mutex; //protege la lectura/escritura del recurso
pthread_mutex_t mutex; //protege read_count
sem_t prefer_writer; //da prioridad a escritores (bloquea lectores cuando un escritor espera)

int read_count = 0;

void* escritor(void* arg) {
    int id = *((int*)arg);

    while(1) {
        printf("Escritor %d: quiere escribir\n", id);

        //indicar que hay un escritor esperando -> bloquear nuevos lectores
       
        sem_wait(&prefer_writer);

        //esperar acceso exclusivo al recurso
        pthread_mutex_lock(&rw_mutex);

        //escritura (sección crítica)
        printf("Escritor %d: escribiendo\n", id);
        sleep(rand() % 5 + 1);

        //liberar recurso y permitir que otros (posiblemente lectores o escritores) procedan
        pthread_mutex_unlock(&rw_mutex);
        sem_post(&prefer_writer);

        //pausa fuera de la sección crítica
        sleep(rand() % 5 + 1);
    }
}

void* lector(void* arg) {
    int id = *((int*)arg);

    while(1) {
        printf("Lector %d: quiere leer\n", id);

        //si hay un escritor esperando, este sem_wait bloqueará hasta que el escritor lo libere.
        sem_wait(&prefer_writer);
        pthread_mutex_lock(&mutex);

        read_count++;
        if (read_count == 1) {
            //primer lector bloquea el acceso de escritores/otros lectores exclusivos
            pthread_mutex_lock(&rw_mutex);
        }

        pthread_mutex_unlock(&mutex);
        
        
        sem_post(&prefer_writer); //permitir que otros lectores entren rápidamente

        //lectura (sección crítica de lectores)
        printf("Lector %d: leyendo\n", id);
        sleep(rand() % 5 + 1);

        //salir de la zona de lectura
        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0) {
            //último lector libera el recurso para escritores
            pthread_mutex_unlock(&rw_mutex);
        }
        pthread_mutex_unlock(&mutex);

        //pausa fuera de la sección crítica
        sleep(rand() % 5 + 1);
    }
}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));

    //inicializar semáforos
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
    sem_init(&prefer_writer, 0, 1);

    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int r_ids[NUM_READERS];
    int w_ids[NUM_WRITERS];

    //crear hilos lectores
    for (int i = 0; i < NUM_READERS; ++i) {
        r_ids[i] = i + 1;
        if (pthread_create(&readers[i], NULL, lector, &r_ids[i]) != 0) {
            perror("pthread_create reader");
            exit(1);
        }
    }

    //crear hilos escritores
    for (int i = 0; i < NUM_WRITERS; ++i) {
        w_ids[i] = i + 1;
        if (pthread_create(&writers[i], NULL, escritor, &w_ids[i]) != 0) {
            perror("pthread_create writer");
            exit(1);
        }
    }

    //esperar a que terminen
    for (int i = 0; i < NUM_READERS; ++i) pthread_join(readers[i], NULL);
    for (int i = 0; i < NUM_WRITERS; ++i) pthread_join(writers[i], NULL);

    //destruir semáforos
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    sem_destroy(&prefer_writer);

    printf("Fin.\n");
    return 0;
}