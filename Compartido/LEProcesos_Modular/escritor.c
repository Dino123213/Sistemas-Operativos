#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaforos.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <id_escritor>\n", argv[0]);
        exit(1);
    }
    
    int id = atoi(argv[1]);
    
    /* Obtener clave y conectarse a la memoria compartida donde están los semáforos */
    key_t key = ftok(SHM_KEY_FILE, SHM_KEY_ID);
    if (key == -1) {
        perror("error en ftok escritor");
        exit(1);
    }
    
    int shmid = shmget(key, sizeof(struct semaforos), 0666);
    if (shmid == -1) {
        perror("error en shmget escritor");
        exit(1);
    }
    
    /* Adjuntar la memoria compartida al espacio de direcciones del proceso */
    struct semaforos *sem = (struct semaforos *)shmat(shmid, NULL, 0);
    if (sem == (void *) -1) {
        perror("error en shmat escritor");
        exit(1);
    }
    
    printf("Escritor %d iniciado\n", id);
    
    /* Realizar ITER escrituras */
    for (int i = 0; i < ITER; i++) {
        /* Esperar acceso exclusivo (wsem=1 significa que no hay lectores ni otros escritores) */
        sem_wait(&sem->wsem);
        
        printf("Escritor %d se encuentra escribiendo... (iteración %d)\n", id, i+1);
        sleep(2); /* Simular tiempo de escritura */
        printf("Escritor %d terminó de escribir (iteración %d)\n", id, i+1);
        
        /* Liberar el acceso para que otros puedan escribir/leer */
        sem_post(&sem->wsem);
        
        sleep(1); /* Pausa entre escrituras */
    }
    
    printf("Escritor %d termino todas sus iteraciones\n", id);
    
    /* Desconectar de la memoria compartida */
    if (shmdt(sem) == -1) {
        perror("error en shmdt escritor");
        exit(1);
    }
    
    return 0;
}