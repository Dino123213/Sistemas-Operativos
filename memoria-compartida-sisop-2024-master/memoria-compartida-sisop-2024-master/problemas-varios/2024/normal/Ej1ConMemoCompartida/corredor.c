#include "estructura.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
int main() {
    key_t clave = generateKey();
	int id = shmget(clave, 1024, 0);
	if(id < 0){
		printf("error fallo el shmget\n");
		exit(1);
	}
	
	struct aCompartir* corredor = (struct aCompartir*) shmat(id,0,0);
	if(corredor <= (struct aCompartir*) (0)){
		printf("Error fallo shmat \n");
		exit(2);
	}

    while(1){
		sleep(1);
		sem_wait(&(corredor-> mutex)); //MUTEX
		
		if(sem_trywait(&(corredor-> cantCorredores)) != 0){
			sem_wait(&(corredor-> instalaciones)); 
		}else{
			sem_post(&(corredor-> cantCorredores));
		}
		sem_post(&(corredor-> cantCorredores));
		sem_post(&(corredor-> mutex));
		
		printf("Llego un corredor\n");
		fflush(stdout);
		sleep(1);
		
		
		sem_wait(&(corredor-> mutex)); //MUTEX
		printf("Se va un corredor\n");
		fflush(stdout);
		sem_wait(&(corredor-> cantCorredores)); 
		if(sem_trywait(&(corredor-> cantCorredores)) != 0){
			sem_post(&(corredor-> instalaciones)); 
		}else{
			sem_post(&(corredor-> cantCorredores));
		}
		sem_post(&(corredor-> mutex));
		

	}	

    return 0;
}
