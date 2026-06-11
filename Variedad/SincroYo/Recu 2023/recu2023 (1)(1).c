#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#define CANT_PROC 10
#define PRIORIDADMAXIMA 10
#define SIZE_MSG (sizeof(struct procesoMensaje) - sizeof(long))

struct procesoMensaje{
	long mtype;
	int IO;
	int pid;
};



void cpu(int id){
	int key = 123;
	int colaListos;
	struct procesoMensaje proceso;
	colaListos = msgget(key,0666); //Creo la cola de Listos.
	
	int key2 = 456;
	int colaEspera;
	colaEspera = msgget(key2,0666); //Creo la cola de espera.
	
	
	while(1){
		if(msgrcv(colaListos, &proceso, SIZE_MSG, -PRIORIDADMAXIMA, 0)!= -1 ){
			printf("Soy la CPU %d, y comienzo a ejecutar %d, con prioridad %ld \n", id , proceso.pid, proceso.mtype);
			fflush(stdout);
			sleep(1);
			if(proceso.IO == 1){
				printf("Soy la CPU %d, y mando a %d a la cola de espera\n",id ,proceso.pid);
				fflush(stdout);
				msgsnd(colaEspera, &proceso, SIZE_MSG,0);
				}
			else{
				printf("Soy la CPU %d, Termine de ejecutar el proceso %d\n",id ,proceso.pid);
				fflush(stdout);
				}
			
			}
	}
	
}

void entradaSalida(){
	int key = 123;
	int colaListos;
	struct procesoMensaje proceso;
	colaListos = msgget(key,0666); //Creo la cola de Listos.
	
	int key2 = 456;
	int colaEspera;
	colaEspera = msgget(key2,0666); //Creo la cola de espera.

	
	while(1){
		
		if(msgrcv(colaEspera, &proceso, SIZE_MSG, -PRIORIDADMAXIMA, 0) != -1){
		printf("Soy el proceso de E/S, y comienzo a realizar la E/S solicitada del proceso %d\n",proceso.pid);
		fflush(stdout);
		sleep(2);
		proceso.IO = 0;
		msgsnd(colaListos, &proceso, SIZE_MSG, 0);
		}
	
}
}

int main(int argc, char **argv)
{
	int input = 0;
	pid_t pid;
	int key = 123;
	int colaListos;
	struct procesoMensaje proceso;
	colaListos = msgget(key,IPC_CREAT|0666); //Creo la cola de Listos.
	
	int key2 = 456;
	int colaEspera;
	colaEspera = msgget(key2,IPC_CREAT|0666); //Creo la cola de espera.
	
	for(int i = 0; i< CANT_PROC; i++){
		proceso.mtype  = (rand() % PRIORIDADMAXIMA) + 1; 
		proceso.IO = (rand() % 2);
		proceso.pid = i+1;
		msgsnd(colaListos,&proceso,SIZE_MSG,0);
	}
	
	
	if(colaEspera == -1 || colaListos == -1 ){
		perror("Error al crear la cola de mensajes");
		exit(1);
		}
		
	while(input < 1 || input > 8)
	{ 
		printf("Cuantas CPUs desea utilizar? (entre 1 y 8): ");
		scanf("%d",&input);
		
		if(input < 1 ||input > 8){
			printf("Error introduzca nuevamente\n");
	}
	}
	
	for(int i =0 ; i<input; i++){
		pid = fork();
		if(pid ==0){
			cpu((i+1));
			exit(0);
			}
		}
		
	pid = fork();
	if (pid == 0){
		entradaSalida();
		exit(0);
		}
		
	for(int i =0 ; i<(input + 1); i++){
		wait(NULL);
		}
		
	if( msgctl(queueID, IPC_RMID, NULL) == -1){
		perror("Error al eliminar la cola");
		exit(1);
	}
	
	return 0;
}

