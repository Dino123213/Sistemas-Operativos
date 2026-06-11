#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 10
#define FIN -1
#define CANTPRODUCTORES 3
int cant_iteraciones, pos_productor = 0;
sem_t huecos,elementos,semN, finalizados;
pthread_mutex_t mutex;

int buffer[MAX];

int generardato(void){
	return rand()%256;
	}

int numeroaleatorio(void){
	return rand()%100;
	}
	
void* productor1(void* p){
	
	int dato;
	while(1){
		if(sem_trywait(&semN) == 0){
			sem_wait(&huecos);
			dato = generardato();
			pthread_mutex_lock(&mutex);
				buffer[pos_productor] = dato;
				pos_productor = (pos_productor + 1) % MAX;
			pthread_mutex_unlock(&mutex);
			sem_post(&elementos);
		}
		else{
			sem_wait(&huecos);
			pthread_mutex_lock(&mutex);
				sem_wait(&finalizados);
				if(sem_trywait(&finalizados) != 0){
					
					buffer[pos_productor] = FIN;
					pos_productor = (pos_productor + 1) % MAX;
					sem_post(&elementos);
				}
				else{ sem_post(&finalizados); sem_post(&huecos);}
			pthread_mutex_unlock(&mutex);
			printf("finiquitao \n");
			pthread_exit(NULL);
		}
		
	}


}
	
void* consumidor(void *p){
	int pos_consumidor, dato, cont;
	int continuar = 1;
	cont = 0;
	pos_consumidor = 0;
	while(continuar){
		sem_wait(&elementos);
			pthread_mutex_lock(&mutex);
				dato = buffer[pos_consumidor];
				cont++;
				pos_consumidor = (pos_consumidor + 1) % MAX;
			pthread_mutex_unlock(&mutex);
		sem_post(&huecos);
		
		if(dato == FIN){ continuar = 0; }
		else printf("%d. Numero aleatorio: %d \n",cont, dato);
	}
	pthread_exit(NULL);

	}
	
int main(){
	srand(getpid());
	pthread_t hiloProd1, hiloProd2, hiloProd3, hiloCons;
	
	// Inicializacion de semaforos
	sem_init(&elementos, 0,0);
	sem_init(&huecos,0,MAX); 
	sem_init(&finalizados,0,CANTPRODUCTORES); 
	cant_iteraciones = numeroaleatorio();
	printf("%d \n",cant_iteraciones);
	sem_init(&semN, 0, cant_iteraciones);
	
	// Inicializacion mutex
	pthread_mutex_init(&mutex,NULL);
	
	// Inicializacion de los hilos
	pthread_create(&hiloProd1, NULL, productor1, NULL);
	pthread_create(&hiloProd2, NULL, productor1, NULL);
	pthread_create(&hiloProd3, NULL, productor1, NULL);
	pthread_create(&hiloCons, NULL, consumidor, NULL);
	
	// Destruye los semaforos
	sem_destroy(&elementos);
	sem_destroy(&huecos);
	
	// Destruye el mutex
	pthread_mutex_destroy(&mutex);
		
	// Espera a que terminen los hilos
	pthread_join(hiloProd1, NULL);
	pthread_join(hiloProd2, NULL);
	pthread_join(hiloProd3, NULL);
	pthread_join(hiloCons, NULL);
}
