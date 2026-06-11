#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <time.h>

struct msg_buffer {
    long tipo;
    // más si tengo
};

void procesoA(int msg_id) {
    struct msg_buffer mensaje;
    while (1) {
        msgrcv(msg_id, &mensaje, sizeof(mensaje) - sizeof(long), 0, 0);
        mensaje.tipo = 0; // <-- cambiar msjs acorde
        msgsnd(msg_id, &mensaje, sizeof(mensaje) - sizeof(long), 0);
    }
}

int main() {
    key_t key; 
    int pidA;
    int msg_id;

    key = ftok("/tmp", 'A');
    msg_id = msgget(key, IPC_CREAT | 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    pidA = fork();
    if(pidA == 0) {
        server(msg_id);
        exit(0);
    }

    wait(NULL);

    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
}