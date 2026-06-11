#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

sem_t sem_1,sem_2,sem_3,sem_4,sem_5,sem_6,sem_7,sem_8;

void * nodo1(void * args){
	while(1){
		sem_wait(&sem_1);
		printf("T1\n");
		sleep(1);
		sem_post(&sem_2);
		sem_post(&sem_3);
		sem_post(&sem_5);
	}
}

void * nodo2(void * args){
	while(1){
		sem_wait(&sem_2);
		sem_wait(&sem_2);
		printf("T2\n");
		sleep(1);
		sem_post(&sem_4);
	}
}

void * nodo3(void * args){
	while(1){
		sem_wait(&sem_3);
		printf("T3\n");
		sleep(1);
		sem_post(&sem_6);
	}
}

void * nodo4(void * args){
	while(1){
		sem_wait(&sem_4);
		sem_wait(&sem_4);
		printf("T4\n");
		sleep(1);
		sem_post(&sem_2);
		sem_post(&sem_8);
	}
}

void * nodo5(void * args){
	while(1){
		sem_wait(&sem_5);
		printf("T5\n");
		sleep(1);
		sem_post(&sem_6);
		sem_post(&sem_7);
	}
}

void * nodo6(void * args){
	while(1){
		sem_wait(&sem_6);
		sem_wait(&sem_6);
		printf("T6\n");
		sleep(1);
		sem_post(&sem_8);
	}
}

void * nodo7(void * args){
	while(1){
		sem_wait(&sem_7);
		printf("T7\n");
		sleep(1);
		sem_post(&sem_4);
		sem_post(&sem_8);
	}
}

void * nodo8(void * args){
	while(1){
		sem_wait(&sem_8);
		sem_wait(&sem_8);
		sem_wait(&sem_8);
		printf("T8\n");
		sleep(1);
		sem_post(&sem_1);
	}
}


int main(){
	sem_init(&sem_1, 0, 1);
	sem_init(&sem_2, 0, 1);
	sem_init(&sem_3, 0, 0);
	sem_init(&sem_4, 0, 0);
	sem_init(&sem_5, 0, 0);
	sem_init(&sem_6, 0, 0);
	sem_init(&sem_7, 0, 0);
	sem_init(&sem_8, 0, 0);
	
	pthread_t hilo1, hilo2, hilo3, hilo4, hilo5, hilo6, hilo7, hilo8;
	
	pthread_create(&hilo1, NULL, nodo1, NULL);
	pthread_create(&hilo2, NULL, nodo2, NULL);
	pthread_create(&hilo3, NULL, nodo3, NULL);
	pthread_create(&hilo4, NULL, nodo4, NULL);
	pthread_create(&hilo5, NULL, nodo5, NULL);
	pthread_create(&hilo6, NULL, nodo6, NULL);
	pthread_create(&hilo7, NULL, nodo7, NULL);
	pthread_create(&hilo8, NULL, nodo8, NULL);
	
	pthread_join(hilo1,NULL);
	pthread_join(hilo2,NULL);
	pthread_join(hilo3,NULL);
	pthread_join(hilo4,NULL);
	pthread_join(hilo5,NULL);
	pthread_join(hilo6,NULL);
	pthread_join(hilo7,NULL);
	pthread_join(hilo8,NULL);
	
	sem_destroy(&sem_1);
	sem_destroy(&sem_2);
	sem_destroy(&sem_3);
	sem_destroy(&sem_4);
	sem_destroy(&sem_5);
	sem_destroy(&sem_6);
	sem_destroy(&sem_7);
	sem_destroy(&sem_8);
	
	return 0;
}
