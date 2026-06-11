#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaforos.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <id_lector>\n", argv[0]);
        exit(1);
    }
    
    int id = atoi(argv[1]);
    
    /* Obtener clave y conectarse a la memoria compartida donde están los semáforos */
    key_t key = ftok(SHM_KEY_FILE, SHM_KEY_ID);
    if (key == -1) {
        perror("error en ftok lector");
        exit(1);
    }
    
    int shmid = shmget(key, sizeof(struct semaforos), 0666);
    if (shmid == -1) {
        perror("error en shmget lector");
        exit(1);
    }
    
    /* Adjuntar la memoria compartida al espacio de direcciones del proceso */
    struct semaforos *sem = (struct semaforos *)shmat(shmid, NULL, 0);
    if (sem == (void *) -1) {
        perror("error en shmat lector");
        exit(1);
    }
    
    printf("Lector %d iniciado\n", id);
    
    /* Realizar ITER lecturas */
    for (int i = 0; i < ITER; i++) {
        /* PROTOCOLO DE ENTRADA DEL LECTOR:
         * Adquirir mutex para acceso exclusivo al contador de lectores */
        sem_wait(&sem->rmutex);
        
        /* Verificar si es el primer lector */
        if (sem_trywait(&sem->rsem) == 0) {
            /* No es el primer lector, ya hay otros leyendo */
            sem_post(&sem->rsem);
        } else {
            /* Es el primer lector, debe bloquear a los escritores */
            sem_wait(&sem->wsem);
        }
        
        /* Incrementar contador de lectores */
        sem_post(&sem->rsem);
        /* Liberar mutex del contador */
        sem_post(&sem->rmutex);
        
        /* SECCIÓN CRÍTICA: LECTURA */
        printf("Lector %d leyendo... (iteración %d)\n", id, i+1);
        sleep(2); /* Simular tiempo de lectura */
        
        /* PROTOCOLO DE SALIDA DEL LECTOR:
         * Adquirir mutex para acceso exclusivo al contador */
        sem_wait(&sem->rmutex);
        printf("Lector %d terminó de leer (iteración %d)\n", id, i+1);
        
        /* Decrementar contador de lectores */
        sem_wait(&sem->rsem);
        
        /* Verificar si es el último lector */
        if (sem_trywait(&sem->rsem) == -1) {
            /* Es el último lector, liberar acceso a escritores */
            sem_post(&sem->wsem);
        } else {
            /* No es el último, reponer el contador */
            sem_post(&sem->rsem);
        }
        
        /* Liberar mutex del contador */
        sem_post(&sem->rmutex);
        
        sleep(1); /* Pausa entre lecturas */
    }
    
    printf("Lector %d termino todas sus iteraciones\n", id);
    
    /* Desconectar de la memoria compartida */
    if (shmdt(sem) == -1) {
        perror("error en shmdt lector");
        exit(1);
    }
    
    return 0;
}