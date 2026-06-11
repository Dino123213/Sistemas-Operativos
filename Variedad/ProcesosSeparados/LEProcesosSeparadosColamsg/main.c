#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define WSEM 1
#define RSEM 2
#define RMUTEX 3
#define LECTORES 4
#define ESCRITORES 2

struct mensaje {
    long mtype;
};

int main() {
    pid_t pid;
    key_t key;
    struct mensaje sems;
    int queueID;

    // Clave compartida entre todos
    key = ftok("colamensajes.c", 67);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Crear cola de mensajes limpia
    queueID = msgget(key, 0666 | IPC_CREAT);
    msgctl(queueID, IPC_RMID, NULL);
    queueID = msgget(key, 0666 | IPC_CREAT);
    if (queueID == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Inicializar los "semaforos"
    sems.mtype = RMUTEX;
    msgsnd(queueID, &sems, 0, 0); // mutex = 1
    sems.mtype = WSEM;
    msgsnd(queueID, &sems, 0, 0); // wsem = 1

    printf("Cola de mensajes inicializada (ID=%d)\n", queueID);

    // Lanzar lectores
    for (int i = 0; i < LECTORES; i++) {
        pid = fork();
        if (pid == 0) {
            execlp("./lector", "lector", NULL);
            perror("execlp lector");
            exit(1);
        } else if (pid < 0) {
            perror("fork lector");
        }
    }

    // Lanzar escritores
    for (int i = 0; i < ESCRITORES; i++) {
        pid = fork();
        if (pid == 0) {
            execlp("./escritor", "escritor", NULL);
            perror("execlp escritor");
            exit(1);
        } else if (pid < 0) {
            perror("fork escritor");
        }
    }

    // Esperar hijos
    for (int i = 0; i < LECTORES + ESCRITORES; i++) {
        wait(NULL);
    }

    // Eliminar cola
    if (msgctl(queueID, IPC_RMID, NULL) == -1) {
        perror("msgctl remove");
        exit(EXIT_FAILURE);
    }

    return 0;
}
