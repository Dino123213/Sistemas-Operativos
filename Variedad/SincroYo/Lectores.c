#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t lectores;   // semáforo inicializado en 0
sem_t escritores;      // controla acceso de escritores o lectores al recurso
sem_t mutex;         // protege acceso a lector_flag

void* lector(void* id) {
    int lector_id = *(int*)id;
    while (1) {
        sem_wait(&mutex);
        // Si no hay ningún lector, trywait devuelve -1 y va al else
        if (sem_trywait(&lectores) == 0) {
            sem_post(&lectores);
            sem_post(&lectores);
        }else{//Primer lector entra
            sem_wait(&escritores);//Verifica si hay escritores, si no hay espera y si hay lo bloquea
            sem_post(&lectores);//Aumenta la cantidad de lectores 
        }
        sem_post(&mutex);

        // --- Simula lectura ---
        printf(" Lector %d está leyendo el archivo...\n", lector_id);
        usleep(rand() % 500000);
        printf(" Lector %d terminó de leer.\n", lector_id);
        // -------------------------------

    
        sem_wait(&mutex);
        sem_wait(&lectores); // Disminuye la cantidad de lectores
        if (sem_trywait(&lectores) == 0) {
            sem_post(&lectores); // Si hay más lectores, solo disminuye el contador  
        } else {
            sem_post(&escritores); // Último lector sale, libera escritores
        }
        sem_post(&mutex);

        usleep(rand() % 1000000); // espera antes de volver a leer
    }
    return NULL;
}

void* escritor(void* id) {
    int escritor_id = *(int*)id;
    while (1) {
        sem_wait(&escritores);  // espera a que no haya lectores, haya permiso para escribir
        sem_wait(&mutex);  // obtiene acceso exclusivo

        printf(" Escritor %d está escribiendo en el archivo...\n", escritor_id);
        usleep(rand() % 700000);
        printf(" Escritor %d terminó de escribir.\n", escritor_id);

        sem_post(&mutex); // libera acceso exclusivo
        sem_post(&escritores); // permite que otros lectores/escritores accedan
        usleep(rand() % 1500000);
    }
    
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t alectores[5], aescritores[2];
    int id_lectores[5], id_escritores[2];

    sem_init(&mutex, 0, 1);
    sem_init(&escritores, 0, 1);
    sem_init(&lectores, 0, 0);  // indica si hay lectores activos

    // Crear hilos lectores
    for (int i = 0; i < 5; i++) {
        id_lectores[i] = i + 1;
        pthread_create(&alectores[i], NULL, lector, &id_lectores[i]);
    }

    // Crear hilos escritores
    for (int i = 0; i < 2; i++) {
        id_escritores[i] = i + 1;
        pthread_create(&aescritores[i], NULL, escritor, &id_escritores[i]);
    }

    // Esperar hilos 
    for (int i = 0; i < 5; i++)
        pthread_join(alectores[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(aescritores[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&lectores);
    sem_destroy(&escritores);
    return 0;
}
