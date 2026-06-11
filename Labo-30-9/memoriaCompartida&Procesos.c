#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>

#define KEY ((key_t) (1234)) 
#define SEG_SIZE sizeof(struct memoria_compartida)

#define N

struct memoria_compartida {
    int buffer[1];
};

void procesoA() {
    struct memoria_compartida *data;
    int id = shmget(KEY, SEG_SIZE, 0); //Obtengo la llave
    if(id < 0) {
        printf("Error: fallo en shmget.\n");
        exit(1);
    }

    data = (struct memoria_compartida*)shmat(id, 0, 0); //Hago el attach a la memoria y obtengo el puntero
    if (data == (void *) -1) {
        perror("Error en shmat");
        exit(2);
    }

    /*

        CÓDIGO

    */
}

void procesoB() {
    struct memoria_compartida *data;
    int id = shmget(KEY, SEG_SIZE, 0);
    if(id < 0) {
        printf("Error: fallo en shmget.\n");
        exit(1);
    }

    data = (struct memoria_compartida*)shmat(id, 0, 0);
    if(data <= (struct memoria_compartida *)(0)) {
        printf("Error: fallo en shmat.\n");
        exit(2);
    }

    /*

        CÓDIGO

    */
}

int main() {
    int id;
    pid_t procA, procB;

    id = shmget(KEY, SEG_SIZE, IPC_CREAT | 0666); 
    if (id == -1) {
        printf("Error: fallo en shmget\n");
        exit(1);
    }
    struct memoria_compartida *data = (struct memoria_compartida *)shmat(id, 0, 0);
    if(data <= (struct memoria_compartida *)(0)) {
        printf(" Error: fallo en shmat\n");
        exit(1);
    }

   

    procA = fork();
    if(procA == 0) {
        procesoA();
        exit(0);
    } else if (procA < 0) {
        perror("Error: creación proceso.\n");
        exit(1);
    }

    procB = fork();
    if(procB == 0) {
        procesoB();
        exit(0);
    } else if (procB < 0) {
        perror("Error: creación proceso.\n");
        exit(1);
    }

    for(int i = 0; i < 10; i++) { // modificar cantidad con los totales
        wait(NULL);
    }



    shmdt(data); 

    shmctl(id, IPC_RMID, NULL);

    return 0;
}