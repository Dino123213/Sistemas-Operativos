#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#define CANT 5
sem_t cantCorredor, instalaciones;
pthread_mutex_t mutex;

void* corredor(void* args){
	
while(1){
	sleep(1);
	pthread_mutex_lock(&mutex);
	if(sem_trywait(&cantCorredor) != 0){
		sem_wait(&instalaciones);
	}else{
		sem_post(&cantCorredor);
	}
	
	sem_post(&cantCorredor);
	pthread_mutex_unlock(&mutex);
	printf("Llego un corredor\n");
	sleep(1);
	
	
	pthread_mutex_lock(&mutex);
	printf("Se va un corredor\n");
	sem_wait(&cantCorredor);
	if(sem_trywait(&cantCorredor) != 0){
		sem_post(&instalaciones);
	}else{
		sem_post(&cantCorredor);
	}
	pthread_mutex_unlock(&mutex);
	

}	
	
}

void* martillo(void* args){
	while(1){
		sleep(1);
		sem_wait(&instalaciones);
		printf("Llego un lanzador de martillo\n");
		sleep(1);
		printf("Se va lanzador martillo");
		sem_post(&instalaciones);
		
	}
}

void* jabalina(void* args){
	while(1){
		sleep(1);
		sem_wait(&instalaciones);
		printf("Llego un lanzador de jabalina\n");
		sleep(1);
		printf("Se va un lanzador de jabalina\n");
		sem_post(&instalaciones);
	
	}
}

int main(){
	sem_init(&instalaciones, 0, 1);
	sem_init(&cantCorredor, 0, 0);

	

	
	pthread_t corredores[CANT];
	pthread_t jabalinas[CANT];
	pthread_t martillos[CANT];
	
	pthread_mutex_init(&mutex,NULL);
	
		
	
	for(int i = 0; i < CANT;i++){
		pthread_create(&corredores[i], NULL, corredor, NULL);
	}
	for(int i = 0; i < CANT;i++){
		pthread_create(&jabalinas[i], NULL, jabalina, NULL);
	}
	
	for(int i = 0; i < CANT;i++){
		pthread_create(&martillos[i], NULL, martillo, NULL);
	}
	
	
	for(int i = 0; i < CANT;i++){
		pthread_join(corredores[i], NULL);
	}
	for(int i = 0; i < CANT;i++){
		pthread_join(jabalinas[i], NULL);
	}
	for(int i = 0; i < CANT;i++){
		pthread_join(martillos[i], NULL);
	}

	
	sem_destroy(&instalaciones);
	sem_destroy(&cantCorredor);

	
	pthread_mutex_destroy(&mutex);
	
	
	return 0;
}

