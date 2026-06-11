#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// configuración: puede ajustarse a gusto
#define NUM_READERS 5
#define NUM_WRITERS 3

/*
 Implementación con prioridad a escritores (writers-preference).
 Idea: usar un semáforo `readTry` que bloquea la entrada de nuevos lectores cuando
 un escritor quiere escribir. `resource` protege el recurso compartido.
 `rmutex` protege la variable read_count.

 Comportamiento deseado: si hay un escritor esperando, los nuevos lectores quedan bloqueados
 y el escritor entra tan pronto como el recurso sea libre (no compite con lectores nuevos).
*/

sem_t readTry;    // cuando un escritor llega hace sem_wait(readTry) para bloquear lectores nuevos
sem_t resource;   // acceso exclusivo al recurso
sem_t rmutex;     // protege read_count

int read_count = 0;

void* escritor(void* arg) {
    int id = *((int*)arg);

    while (1) {
        printf("Escritor %d: quiere escribir\n", id);

        // Indicar intención de escribir: bloquear la entrada de nuevos lectores
        sem_wait(&readTry);

        // Obtener acceso exclusivo al recurso (espera a que los lectores actuales salgan)
        sem_wait(&resource);

        // Sección crítica de escritura
        printf("Escritor %d: escribiendo\n", id);
        sleep(rand() % 3 + 1);

        // Liberar recurso y permitir lectores (o siguiente escritor) pasar
        sem_post(&resource);
        sem_post(&readTry);

        // pausa fuera de la sección crítica
        sleep(rand() % 3 + 1);
    }

    return NULL;
}

void* lector(void* arg) {
    int id = *((int*)arg);

    while (1) {
        printf("Lector %d: quiere leer\n", id);

        // Intento de entrar: debo pasar por readTry para asegurar que no haya escritores esperando
        sem_wait(&readTry);

        // Entrar a la sección de lectores
        sem_wait(&rmutex);
        read_count++;
        if (read_count == 1) {
            // primer lector bloquea el recurso para escritores
            sem_wait(&resource);
        }
        sem_post(&rmutex);

        // Permitir que otros lectores entren rápidamente
        sem_post(&readTry);

        // Sección crítica de lectura
        printf("Lector %d: leyendo\n", id);
        sleep(rand() % 3 + 1);

        // Salir de la sección de lectores
        sem_wait(&rmutex);
        read_count--;
        if (read_count == 0) {
            // último lector libera el recurso
            sem_post(&resource);
        }
        sem_post(&rmutex);

        // pausa fuera de la sección crítica
        sleep(rand() % 3 + 1);
    }

    return NULL;
}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));

    // inicializar semáforos
    sem_init(&readTry, 0, 1);
    sem_init(&resource, 0, 1);
    sem_init(&rmutex, 0, 1);

    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int r_ids[NUM_READERS];
    int w_ids[NUM_WRITERS];

    // crear hilos lectores
    for (int i = 0; i < NUM_READERS; ++i) {
        r_ids[i] = i + 1;
        if (pthread_create(&readers[i], NULL, lector, &r_ids[i]) != 0) {
            perror("pthread_create reader");
            exit(1);
        }
    }

    // crear hilos escritores
    for (int i = 0; i < NUM_WRITERS; ++i) {
        w_ids[i] = i + 1;
        if (pthread_create(&writers[i], NULL, escritor, &w_ids[i]) != 0) {
            perror("pthread_create writer");
            exit(1);
        }
    }

    // dejar que los hilos corran indefinidamente (join para mantener el main vivo)
    for (int i = 0; i < NUM_READERS; ++i) pthread_join(readers[i], NULL);
    for (int i = 0; i < NUM_WRITERS; ++i) pthread_join(writers[i], NULL);

    // destruir semáforos (nunca llegará aquí en este ejemplo)
    sem_destroy(&readTry);
    sem_destroy(&resource);
    sem_destroy(&rmutex);

    printf("Fin.\n");
    return 0;
}