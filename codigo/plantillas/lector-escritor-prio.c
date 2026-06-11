#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

//configuración: puede ajustarse a gusto
#define NUM_READERS 5
#define NUM_WRITERS 3

/*ALGORITMO CON PRIORIDAD ABSOLUTA DE ESCRITORES:
  - Cuando un escritor quiere escribir, bloquea inmediatamente nuevos lectores
  - Los lectores activos terminan, pero ningún lector nuevo puede entrar
  - El escritor obtiene acceso exclusivo garantizado
  - Solo después de que termine el escritor, los lectores pueden continuar
*/

pthread_mutex_t resource_mutex;  // Protege el recurso compartido (lectura/escritura)
pthread_mutex_t read_count_mutex; // Protege el contador de lectores
pthread_mutex_t writer_count_mutex; // Protege el contador de escritores
sem_t read_try;    // Bloquea nuevos lectores cuando hay escritores esperando
sem_t resource;    // Semáforo del recurso (para escritores)

int read_count = 0;    // Contador de lectores activos
int writer_count = 0;  // Contador de escritores esperando

void* escritor(void* arg) {
    int id = *((int*)arg);

    while(1) {
        printf("Escritor %d: quiere escribir\n", id);

        // PROTOCOLO DE ENTRADA DEL ESCRITOR CON PRIORIDAD:
        
        // 1. Incrementar contador de escritores esperando
        pthread_mutex_lock(&writer_count_mutex);
        writer_count++;
        if (writer_count == 1) {
            // Primer escritor esperando -> BLOQUEAR INMEDIATAMENTE nuevos lectores
            sem_wait(&read_try);
            printf("Escritor %d: bloqueando nuevos lectores\n", id);
        }
        pthread_mutex_unlock(&writer_count_mutex);

        // 2. Esperar acceso exclusivo al recurso
        sem_wait(&resource);
        pthread_mutex_lock(&resource_mutex);

        // SECCIÓN CRÍTICA: ESCRITURA
        printf("Escritor %d: escribiendo (PRIORIDAD ABSOLUTA)\n", id);
        sleep(rand() % 3 + 1);  // Reducido para mostrar la prioridad más rápido
        printf("Escritor %d: terminó de escribir\n", id);

        // PROTOCOLO DE SALIDA DEL ESCRITOR:
        
        // 3. Liberar el recurso
        pthread_mutex_unlock(&resource_mutex);
        sem_post(&resource);

        // 4. Decrementar contador de escritores
        pthread_mutex_lock(&writer_count_mutex);
        writer_count--;
        if (writer_count == 0) {
            // Último escritor -> permitir que entren lectores nuevamente
            sem_post(&read_try);
            printf("Escritor %d: permitiendo lectores nuevamente\n", id);
        }
        pthread_mutex_unlock(&writer_count_mutex);

        // Pausa fuera de la sección crítica
        sleep(rand() % 4 + 2);
    }
}

void* lector(void* arg) {
    int id = *((int*)arg);

    while(1) {
        printf("Lector %d: quiere leer\n", id);

        // PROTOCOLO DE ENTRADA DEL LECTOR (con prioridad de escritores):
        
        // 1. Intentar entrar (se bloquea si hay escritores esperando)
        sem_wait(&read_try);  // Si hay escritores, este semáforo está en 0 -> BLOQUEO
        
        // 2. Proteger contador de lectores
        pthread_mutex_lock(&read_count_mutex);
        read_count++;
        if (read_count == 1) {
            // Primer lector -> bloquear acceso de escritores al recurso
            sem_wait(&resource);
        }
        pthread_mutex_unlock(&read_count_mutex);
        
        // 3. Permitir que otros lectores también entren (si no hay escritores esperando)
        sem_post(&read_try);

        // SECCIÓN CRÍTICA: LECTURA
        printf("Lector %d: leyendo (puede ser interrumpido por escritores)\n", id);
        sleep(rand() % 3 + 1);
        printf("Lector %d: terminó de leer\n", id);

        // PROTOCOLO DE SALIDA DEL LECTOR:
        
        // 4. Proteger contador de lectores
        pthread_mutex_lock(&read_count_mutex);
        read_count--;
        if (read_count == 0) {
            // Último lector -> liberar recurso para escritores
            sem_post(&resource);
        }
        pthread_mutex_unlock(&read_count_mutex);

        // Pausa fuera de la sección crítica
        sleep(rand() % 4 + 1);
    }
}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));

    printf("=== SISTEMA LECTORES-ESCRITORES CON PRIORIDAD ABSOLUTA DE ESCRITORES ===\n");
    printf("Lectores: %d, Escritores: %d\n", NUM_READERS, NUM_WRITERS);
    printf("REGLA: Cuando un escritor quiere escribir, NINGÚN lector nuevo puede entrar\n\n");

    // Inicializar mutex
    if (pthread_mutex_init(&resource_mutex, NULL) != 0 ||
        pthread_mutex_init(&read_count_mutex, NULL) != 0 ||
        pthread_mutex_init(&writer_count_mutex, NULL) != 0) {
        perror("Error inicializando mutex");
        exit(1);
    }

    // Inicializar semáforos
    if (sem_init(&read_try, 0, 1) != 0 ||    // Inicialmente permite lectores
        sem_init(&resource, 0, 1) != 0) {     // Recurso disponible inicialmente
        perror("Error inicializando semáforos");
        exit(1);
    }

    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int r_ids[NUM_READERS];
    int w_ids[NUM_WRITERS];

    // Crear hilos lectores
    for (int i = 0; i < NUM_READERS; ++i) {
        r_ids[i] = i + 1;
        if (pthread_create(&readers[i], NULL, lector, &r_ids[i]) != 0) {
            perror("pthread_create reader");
            exit(1);
        }
    }

    // Crear hilos escritores
    for (int i = 0; i < NUM_WRITERS; ++i) {
        w_ids[i] = i + 1;
        if (pthread_create(&writers[i], NULL, escritor, &w_ids[i]) != 0) {
            perror("pthread_create writer");
            exit(1);
        }
    }

    // Dejar que ejecuten por un tiempo limitado (para demostración)
    sleep(30);  // Ejecutar por 30 segundos
    
    printf("\n=== TERMINANDO DEMOSTRACIÓN ===\n");
    
    // En un programa real, podrías usar pthread_cancel o variables de control
    // Para esta demostración, terminamos abruptamente
    
    // Limpiar recursos
    pthread_mutex_destroy(&resource_mutex);
    pthread_mutex_destroy(&read_count_mutex);
    pthread_mutex_destroy(&writer_count_mutex);
    sem_destroy(&read_try);
    sem_destroy(&resource);

    printf("Fin.\n");
    return 0;
}