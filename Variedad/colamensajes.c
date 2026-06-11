#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

struct mensaje{
	long mtype;
};

int escritor() {
	key_t key;
	int queueID;
	struct mensaje sems;
	key = ftok("colamensajes.c",67);
	queueID = msgget(key, 0666);
	
	if (queueID == -1) {
        perror("error en msgget");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
		msgrcv (queueID, &sems, 0, WSEM, 0);
		printf("Soy el escritor %d, escribiendo... \n",getpid());
		sleep(2);
		printf("Deje de escribir...\n");
		sems.mtype = WSEM;
		msgsnd(queueID, &sems, 0, 0);
		sleep(1);
	}
	
	return 0;
}

int lector() {
	key_t key;
	int queueID;
	struct mensaje sems;
	key = ftok("colamensajes.c",67);
	queueID = msgget(key, 0666);
	
	if (queueID == -1) {
        perror("error en msgget");
        exit(EXIT_FAILURE);
    }
    while (1) {
		msgrcv(queueID, &sems, 0, RMUTEX, 0);
		if (msgrcv(queueID, &sems, 0, RSEM, IPC_NOWAIT) == 0) {
			sems.mtype = RSEM;
			msgsnd(queueID, &sems, 0, 0);
		}
		else {
			msgrcv(queueID, &sems, 0, WSEM, 0);
		}
		sems.mtype = RSEM;
		msgsnd(queueID, &sems, 0, 0);
		sems.mtype = RMUTEX;
		msgsnd(queueID, &sems, 0, 0);
		
		printf("Soy el lector %d, estoy leyendo...\n",getpid());
		sleep(2);
		printf("Soy el lector %d, deje de leer... \n", getpid());
		
		msgrcv(queueID, &sems, 0, RMUTEX, 0);
		msgrcv(queueID, &sems, 0, RSEM, 0);
		
		if (msgrcv(queueID, &sems, 0, RSEM, IPC_NOWAIT) == 0) {
			sems.mtype = RSEM;
			msgsnd(queueID, &sems, 0, 0);
		}
		else {
			printf("Soy el ultimo lector...\n");
			sems.mtype = WSEM;
			msgsnd(queueID, &sems, 0, 0);
		}
		sems.mtype = RMUTEX;
		msgsnd(queueID, &sems, 0, 0);
		sleep(1);
	}
	return 0;
}

int main() {
	pid_t pid;
	key_t key;
	struct mensaje sems;
	int queueID;
	
	key = ftok("colamensajes.c",67);
	queueID = msgget(key, 0666 | IPC_CREAT);
	msgctl(queueID, IPC_RMID, NULL);
	queueID = msgget(key, 0666 | IPC_CREAT);
	
	if (queueID == -1) {
        perror("error en msgget");
        exit(EXIT_FAILURE);
    }
    
    sems.mtype = RMUTEX;
    msgsnd(queueID, &sems, 0, 0); // inicializo el semaforo mutex en 1
    sems.mtype = WSEM;
    msgsnd(queueID, &sems, 0, 0);
    
    for (int i=0; i<LECTORES; i++) {
		pid = fork();
		if (pid == 0) {
			lector();
			exit(0);
		}
		else if (pid < 0) {
			perror("error en fork");
		}
	}
	
	for (int i=0; i<ESCRITORES; i++) {
		pid = fork();
		if (pid == 0) {
			escritor();
			exit(0);
		}
		else if (pid < 0) {
			perror("error en fork");
		}
	}
	
	for (int i=0; i<LECTORES+ESCRITORES; i++) {
		wait(NULL);
	}
	
	if (msgctl(queueID, IPC_RMID, NULL) == -1) {
		perror("error en msgctl");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}	
