#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "semaforos.h"

int main() {
    printf("=== INICIANDO SISTEMA LECTORES-ESCRITORES ===\n");
    printf("Escritores: %d, Lectores: %d, Iteraciones: %d\n\n", ESCRITORES, LECTORES, ITER);
    
    /* Generar clave para memoria compartida */
    key_t key = ftok(SHM_KEY_FILE, SHM_KEY_ID);
    if (key == -1) {
        perror("error en ftok");
        exit(1);
    }
    
    int shmid;
    
    /* Limpiar memoria compartida previa si existe */
    if ((shmid = shmget(key, sizeof(struct semaforos), 0666)) != -1) {
        printf("Limpiando memoria compartida anterior...\n");
        shmctl(shmid, IPC_RMID, NULL);
    }
    
    /* Crear nueva memoria compartida para los semáforos */
    shmid = shmget(key, sizeof(struct semaforos), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("error en shmget");
        exit(1);
    }
    
    /* Adjuntar memoria compartida al proceso padre */
    struct semaforos *sem = (struct semaforos *)shmat(shmid, NULL, 0);
    if (sem == (void *) -1) {
        perror("error en shmat");
        exit(1);
    }
    
    /* Inicializar semáforos:
     * - wsem: binario (1) para control de escritores
     * - rmutex: binario (1) para mutex de lectores
     * - rsem: contador (0) para número de lectores activos */
    if (sem_init(&sem->wsem, 1, 1) == -1) {
        perror("error inicializando wsem");
        exit(1);
    }
    if (sem_init(&sem->rmutex, 1, 1) == -1) {
        perror("error inicializando rmutex");
        exit(1);
    }
    if (sem_init(&sem->rsem, 1, 0) == -1) {
        perror("error inicializando rsem");
        exit(1);
    }
    
    printf("Semáforos inicializados correctamente\n");
    printf("Creando procesos...\n\n");
    
    /* Arrays para almacenar PIDs de los procesos hijos */
    pid_t pidEscritor[ESCRITORES];
    pid_t pidLector[LECTORES];
    
    /* Crear procesos escritores */
    for (int i = 0; i < ESCRITORES; i++) {
        pidEscritor[i] = fork();
        if (pidEscritor[i] == 0) {
            /* Proceso hijo: ejecutar programa escritor */
            char id_str[10];
            sprintf(id_str, "%d", i + 1);
            execl("./escritor", "escritor", id_str, NULL);
            perror("error en execl escritor");
            exit(1);
        } else if (pidEscritor[i] == -1) {
            perror("error en fork escritor");
            exit(1);
        }
    }
    
    /* Crear procesos lectores */
    for (int i = 0; i < LECTORES; i++) {
        pidLector[i] = fork();
        if (pidLector[i] == 0) {
            /* Proceso hijo: ejecutar programa lector */
            char id_str[10];
            sprintf(id_str, "%d", i + 1);
            execl("./lector", "lector", id_str, NULL);
            perror("error en execl lector");
            exit(1);
        } else if (pidLector[i] == -1) {
            perror("error en fork lector");
            exit(1);
        }
    }
    
    printf("Todos los procesos han sido creados\n");
    printf("Esperando a que terminen...\n\n");
    
    /* Esperar a que terminen todos los procesos hijos */
    for (int i = 0; i < ESCRITORES + LECTORES; i++) {
        int status;
        pid_t terminated_pid = wait(&status);
        if (terminated_pid == -1) {
            perror("error en wait");
        } else {
            printf("Proceso %d terminó con estado %d\n", terminated_pid, WEXITSTATUS(status));
        }
    }
    
    printf("\n=== TODOS LOS PROCESOS HAN TERMINADO ===\n");
    
    /* Destruir semáforos */
    sem_destroy(&sem->wsem);
    sem_destroy(&sem->rmutex);
    sem_destroy(&sem->rsem);
    
    /* Desconectar y limpiar memoria compartida */
    if (shmdt(sem) == -1) {
        perror("error en shmdt");
    }
    
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("error eliminando memoria compartida");
    } else {
        printf("Memoria compartida limpiada correctamente\n");
    }
    
    return 0;
}