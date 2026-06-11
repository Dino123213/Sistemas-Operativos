#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#define ITER 10

struct semaforos {
    sem_t wsem;
    sem_t rmutex;
    sem_t rsem;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <id_lector>\n", argv[0]);
        exit(1);
    }

    int id = atoi(argv[1]);
    key_t key = ftok("LEArchivo.c", 76);
    int shmid = shmget(key, sizeof(struct semaforos), 0666);
    if (shmid == -1) {
        perror("shmget lector");
        exit(1);
    }

    struct semaforos *sem = (struct semaforos *)shmat(shmid, NULL, 0);
    if (sem == (void *) -1) {
        perror("shmat lector");
        exit(1);
    }

    for (int i = 0; i < ITER; i++) {
        sem_wait(&sem->rmutex);
        if (sem_trywait(&sem->rsem) == 0) {
            sem_post(&sem->rsem);
        } else {
            sem_wait(&sem->wsem);
        }
        sem_post(&sem->rsem);
        sem_post(&sem->rmutex);

        printf("Lector %d leyendo...\n", id);
        sleep(2);
        printf("Lector %d terminó de leer...\n", id);

        sem_wait(&sem->rmutex);
        sem_wait(&sem->rsem);
        if (sem_trywait(&sem->rsem) == -1) {
            sem_post(&sem->wsem);
        } else {
            sem_post(&sem->rsem);
        }
        sem_post(&sem->rmutex);
        sleep(1);
    }

    printf("Lector %d terminó definitivamente.\n", id);
    shmdt(sem);
    return 0;
}
