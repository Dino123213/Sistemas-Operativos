#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define WSEM 1
#define RSEM 2
#define RMUTEX 3

struct mensaje {
    long mtype;
};

int main() {
    key_t key;
    int queueID;
    struct mensaje sems;

    key = ftok("colamensajes.c", 67);
    queueID = msgget(key, 0666);
    if (queueID == -1) {
        perror("msgget lector");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Entrada de lector
        msgrcv(queueID, &sems, 0, RMUTEX, 0);
        if (msgrcv(queueID, &sems, 0, RSEM, IPC_NOWAIT) == 0) {
            sems.mtype = RSEM;
            msgsnd(queueID, &sems, 0, 0);
        } else {
            msgrcv(queueID, &sems, 0, WSEM, 0);
        }
        sems.mtype = RSEM;
        msgsnd(queueID, &sems, 0, 0);
        sems.mtype = RMUTEX;
        msgsnd(queueID, &sems, 0, 0);

        printf("Lector %d leyendo...\n", getpid());
        sleep(2);
        printf("Lector %d terminó de leer\n", getpid());

        // Salida de lector
        msgrcv(queueID, &sems, 0, RMUTEX, 0);
        msgrcv(queueID, &sems, 0, RSEM, 0);

        if (msgrcv(queueID, &sems, 0, RSEM, IPC_NOWAIT) == 0) {
            sems.mtype = RSEM;
            msgsnd(queueID, &sems, 0, 0);
        } else {
            printf("Lector %d fue el último lector\n", getpid());
            sems.mtype = WSEM;
            msgsnd(queueID, &sems, 0, 0);
        }
        sems.mtype = RMUTEX;
        msgsnd(queueID, &sems, 0, 0);
        sleep(1);
    }

    return 0;
}
