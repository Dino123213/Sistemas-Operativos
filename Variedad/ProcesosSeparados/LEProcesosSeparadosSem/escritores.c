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
        fprintf(stderr, "Uso: %s <id_escritor>\n", argv[0]);
        exit(1);
    }

    int id = atoi(argv[1]);
    key_t key = ftok("LEArchivo.c", 76);
    int shmid = shmget(key, sizeof(struct semaforos), 0666);
    if (shmid == -1) {
        perror("shmget escritor");
        exit(1);
    }

    struct semaforos *sem = (struct semaforos *)shmat(shmid, NULL, 0);
    if (sem == (void *) -1) {
        perror("shmat escritor");
        exit(1);
    }

    for (int i = 0; i < ITER; i++) {
        sem_wait(&sem->wsem);
        printf("Escritor %d escribiendo...\n", id);
        sleep(2);
        printf("Escritor %d terminó de escribir...\n", id);
        sem_post(&sem->wsem);
        sleep(1);
    }

    printf("Escritor %d terminó definitivamente.\n", id);
    shmdt(sem);
    return 0;
}
