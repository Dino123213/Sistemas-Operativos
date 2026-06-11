#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>

#define MAX 10
#define FIN -1

//Buffer compartido
struct Buffer {
    int data[MAX];
    int pos_productor;
    int pos_consumidor;
};

// Declaración de semáforos
sem_t *huecos;
sem_t *elementos;

int generardato(void) {
    return (rand() % 256);
}

int numeroaleatorio(void) {
    return (rand() % 100);
}

void *productor(void *p) {
    int shmid;
    struct Buffer *buffer;
    int num, dato, n;
    n = numeroaleatorio();

    printf("Productor con %d datos\n", n);
    if ((shmid = shmget(IPC_PRIVATE, sizeof(struct Buffer), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    buffer = (struct Buffer *)shmat(shmid, NULL, 0);
	for (num = 0; num < n; num++) {
        dato = generardato();

        sem_wait(huecos);

        buffer->data[buffer->pos_productor] = dato;
        buffer->pos_productor = (buffer->pos_productor + 1) % MAX;

        sem_post(elementos);
    }

    buffer->data[buffer->pos_productor] = FIN;
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, NULL);

    pthread_exit(NULL);
}

void *consumidor(void *p) {
    int shmid;
    struct Buffer *buffer;
    int dato;
    bool continuar = true;

    if ((shmid = shmget(IPC_PRIVATE, sizeof(struct Buffer), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    buffer = (struct Buffer *)shmat(shmid, NULL, 0);

    while (continuar) {
        sem_wait(elementos);
        dato = buffer->data[buffer->pos_consumidor];
        buffer->pos_consumidor = (buffer->pos_consumidor + 1) % MAX;
        if (dato == FIN)
            continuar = false;
        sem_post(huecos);
        printf("Numero aleatorio: %d\n", dato);
    }
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, NULL);

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    huecos = sem_open("/huecos", O_CREAT | O_EXCL, 0644, MAX);
    elementos = sem_open("/elementos", O_CREAT | O_EXCL, 0644, 0);

    int shmid;
    if ((shmid = shmget(IPC_PRIVATE, sizeof(struct Buffer), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    struct Buffer *buffer = (struct Buffer *)shmat(shmid, NULL, 0);
    buffer->pos_productor = 0;
    buffer->pos_consumidor = 0;

    // Creacion de procesos
    pid_t pid;
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0) {
            productor(NULL);
            exit(0);
        }
    }

    consumidor(NULL);

    // Esperar a que todos los procesos terminen
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    sem_unlink("/huecos");
    sem_unlink("/elementos");
    sem_close(huecos);
    sem_close(elementos);

    return 0;
}



