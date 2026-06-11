#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CANT_BUSCADORES 3
#define CANT_INSERTADORES 2
#define CANT_BORRADORES 2
#define ITER 25

sem_t noInsertador, noEliminador, semBuscador;
pthread_mutex_t mutexBuscador;

void *Buscador(void *arg) {
	int id = (int) arg;
	for (int i=0; i<ITER; i++) {
		pthread_mutex_lock(&mutexBuscador);
		if (sem_trywait(&semBuscador) != 0) {
			sem_post(&semBuscador);
		}
		else {
			sem_wait(&noEliminador);
		}
		sem_post(&semBuscador);
		pthread_mutex_unlock(&mutexBuscador);
		
		printf("Soy el buscador %d, empiezo a buscar...\n",id);
		fflush(stdout);
		sleep(1);
		printf("Soy el buscador %d, termine de buscar...\n",id);
		fflush(stdout);
		
		pthread_mutex_lock(&mutexBuscador);
		sem_wait(&semBuscador);
		if (sem_trywait(&semBuscador) == 0) {
			sem_post(&semBuscador);
		}
		else {
			printf("No quedan buscadores...\n");
			fflush(stdout);
			sem_post(&noEliminador);
		}
		pthread_mutex_unlock(&mutexBuscador);
		sleep(1);
	}
	return NULL;
}

void *Insertador(void *arg) {
	int id = (int) arg;
	for (int i=0; i<ITER; i++) {
		sem_wait(&noInsertador);
		printf("Soy el insertador %d, inserto en la lista...\n",id);
		fflush(stdout);
		sleep(2);
		printf("Soy el insertador %d, termine de insertar...\n",id);
		fflush(stdout);
		sem_post(&noInsertador);
		sleep(1);
	}
	return NULL;
}

void *Borrador(void *arg) {
	int id = (int) arg;
	for (int i=0; i<ITER; i++) {
		sem_wait(&noInsertador);
		sem_wait(&noEliminador);
		printf("Soy el borrador %d, elimino de la lista...\n",id);
		fflush(stdout);
		sleep(2);
		printf("Termine de borrar...\n");
		fflush(stdout);
		sem_post(&noEliminador);
		sem_post(&noInsertador);
		sleep(1);
	}
	return NULL;
}

int main() {
	pthread_t buscador[CANT_BUSCADORES], insertador[CANT_INSERTADORES], borrador[CANT_BORRADORES];
	sem_init(&semBuscador,0,0);
	sem_init(&noInsertador,0,1);
	sem_init(&noEliminador,0,1);
	pthread_mutex_init(&mutexBuscador, NULL);
	
	for (int i=0; i<CANT_BUSCADORES; i++) {
		pthread_create(&buscador[i],NULL, Buscador, (void *) i+1);
	}
	for (int i=0; i<CANT_BORRADORES; i++) {
		pthread_create(&borrador[i],NULL, Borrador, (void *) i+1);
	}
	for (int i=0; i<CANT_INSERTADORES; i++) {
		pthread_create(&insertador[i],NULL, Insertador, (void *) i+1);
	}
	
	for (int i=0; i<CANT_BUSCADORES; i++) {
		pthread_join(buscador[i],NULL);
	}
	for (int i=0; i<CANT_BORRADORES; i++) {
		pthread_join(borrador[i],NULL);
	}
	for (int i=0; i<CANT_INSERTADORES; i++) {
		pthread_join(insertador[i],NULL);
	}
	
	sem_destroy(&semBuscador);
	sem_destroy(&noEliminador);
	sem_destroy(&noInsertador);
	pthread_mutex_destroy(&mutexBuscador);
	
	return 0;
}
