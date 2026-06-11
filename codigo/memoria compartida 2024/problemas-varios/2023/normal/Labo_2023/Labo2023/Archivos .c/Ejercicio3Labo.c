#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define H 10
#define M 10

sem_t hombres, mujeres;

pthread_mutex_t mutexMujeres = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexHombres = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t baño = PTHREAD_MUTEX_INITIALIZER;

void* ingresaHombre(){
	while(1){
		pthread_mutex_lock(&mutexHombres);			
		if(sem_trywait(&hombres) != 0)		
			pthread_mutex_lock(&baño);			
		else
			sem_post(&hombres);
		
		
		sem_post(&hombres);
		printf("Ingresa hombre al baño \n");
		fflush(NULL);
		pthread_mutex_unlock(&mutexHombres);
		pthread_mutex_lock(&mutexHombres);
		sem_wait(&hombres);
		
		if(sem_trywait(&hombres) != 0)		
			pthread_mutex_unlock(&baño);		
		else
			sem_post(&hombres);
			
			
			
		printf("Sale hombre del baño \n");
		fflush(NULL);
		pthread_mutex_unlock(&mutexHombres);
		
	}
}


void* ingresaMujer(){
	
	while(1){
		pthread_mutex_lock(&mutexMujeres);			
		if(sem_trywait(&mujeres) != 0)		
			pthread_mutex_lock(&baño);			
		else
			sem_post(&mujeres);
		sem_post(&mujeres);
		printf("Ingresa mujer al baño");
		fflush(NULL);
		pthread_mutex_unlock(&mutexMujeres);
		pthread_mutex_lock(&mutexMujeres);
		sem_wait(&mujeres);
		
		if(sem_trywait(&mujeres) != 0)		
			pthread_mutex_unlock(&baño);
		else
			sem_post(&mujeres);
		
		printf("Sale mujer del baño \n");
		fflush(NULL);
		pthread_mutex_unlock(&mutexMujeres);
		
	}
}

int main() {
	pthread_t threads_mujeres[M];
	pthread_t threads_hombres[H];
	//Se inician los semáforos
	sem_init(&mujeres, 0, 1);
	sem_init(&hombres, 0, 1);
	
	pthread_mutex_unlock(&mutexMujeres);
	pthread_mutex_unlock(&mutexHombres);
	pthread_mutex_unlock(&baño);
	
	
	for(int i = 0; i < 10; i++){
		pthread_create(&threads_mujeres[i], NULL, ingresaMujer, NULL);			
		pthread_create(&threads_hombres[i], NULL, ingresaHombre, NULL);
	}
	for(int i = 0; i < 10; i++){
		pthread_join(threads_mujeres[i], NULL);
		pthread_join(threads_hombres[i], NULL);
	}
	
	sem_destroy(&hombres);
	sem_destroy(&mujeres);
	
	return 0;
}
