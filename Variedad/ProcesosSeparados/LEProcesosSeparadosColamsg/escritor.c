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
        perror("msgget escritor");
        exit(EXIT_FAILURE);
    }

    while (1) {
        msgrcv(queueID, &sems, 0, WSEM, 0);
        printf("Escritor %d escribiendo...\n", getpid());
        sleep(2);
        printf("Escritor %d terminó de escribir\n", getpid());
        sems.mtype = WSEM;
        msgsnd(queueID, &sems, 0, 0);
        sleep(1);
    }

    return 0;
}
