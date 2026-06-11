#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>



sem_t semA, semB, semC;

void* A(void* arg){
	for(int i = 0; i<20;i++){
		sem_wait(&semA);
		sem_wait(&semA);
		sem_wait(&semA);
		sem_wait(&semA);
		printf("A");
		fflush(stdout);
		sem_post(&semB);
		sem_post(&semB);

	}
	pthread_exit(0);
}

void* B(void* arg){
	for(int i = 0; i<80;i++){
		
		sem_wait(&semB);
		printf("B");
		fflush(stdout);
		sem_post(&semC);
		sem_post(&semA);
		

	}
	pthread_exit(0);
}

void* C(void* arg){
	for(int i = 0; i<20;i++){
		sem_wait(&semC);
		sem_wait(&semC);
		sem_wait(&semC);
		sem_wait(&semC);
		printf("C");
		fflush(stdout);
        sem_post(&semB);
        sem_post(&semB);
        		
	}
	pthread_exit(0);
}






int main(){
	pthread_t thread_A, thread_B, thread_C;

	//Inicializacion semaforos
	sem_init(&semA,0,4);
	sem_init(&semB,0,0);
	sem_init(&semC,0,2);
	
	//creacion thread
	pthread_create(&thread_A, NULL, A, NULL);
	pthread_create(&thread_B, NULL, B, NULL);
	pthread_create(&thread_C, NULL, C, NULL);

	//espera a que terminen
	pthread_join(thread_A, NULL);
	pthread_join(thread_B, NULL);
	pthread_join(thread_C, NULL);

	sem_destroy(&semA);
	sem_destroy(&semB);
	sem_destroy(&semC);
	
	printf("\n");

	return 0;
}
