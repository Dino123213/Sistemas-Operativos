#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define ESCRITORES 3
#define LECTORES 3
#define ITER 10

struct semaforos {
    sem_t wsem;     // Exclusión entre escritores
    sem_t rmutex;   // Mutex para acceso al contador de lectores
    sem_t rsem;     // Contador de lectores
};

int main() {
    key_t key = ftok("LEArchivo.c", 76);
    int shmid;

    // Limpiar memoria previa
    if ((shmid = shmget(key, sizeof(struct semaforos), 0666)) != -1) {
        shmctl(shmid, IPC_RMID, NULL);
    }

    // Crear nueva memoria compartida
    shmid = shmget(key, sizeof(struct semaforos), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Error en shmget");
        exit(1);
    }

    // Mapear memoria
    struct semaforos *sem = (struct semaforos *)shmat(shmid, NULL, 0);
    if (sem == (void *) -1) {
        perror("Error en shmat");
        exit(1);
    }

    // Inicializar semáforos compartidos (pshared = 1)
    sem_init(&sem->wsem, 1, 1);
    sem_init(&sem->rmutex, 1, 1);
    sem_init(&sem->rsem, 1, 0);

    printf("Memoria y semáforos inicializados correctamente.\n");

    // Crear escritores
    for (int i = 0; i < ESCRITORES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            char id[10];
            sprintf(id, "%d", i + 1);
            execlp("./escritor", "escritor", id, NULL);
            perror("execlp escritor");
            exit(1);
        }
    }

    // Crear lectores
    for (int i = 0; i < LECTORES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            char id[10];
            sprintf(id, "%d", i + 1);
            execlp("./lector", "lector", id, NULL);
            perror("execlp lector");
            exit(1);
        }
    }

    // Esperar a todos los hijos
    for (int i = 0; i < LECTORES + ESCRITORES; i++) {
        wait(NULL);
    }

    // Liberar memoria
    shmctl(shmid, IPC_RMID, NULL);
    printf("Memoria compartida liberada.\n");
    return 0;
}
