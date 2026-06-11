#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define threadsGatos 8
#define threadsPerros 9
#define cantGatos 7
#define cantPerros 5

/*
entran perros y gatos
condicion:
perros y gatos no puede estar juntos


*/
sem_t GatosDentro, GatosMax;
sem_t PerrosDentro, PerrosMax;

//Bloquear la entrada de la raza opuesta
sem_t peluqueria;

//Evitar la inanicion
sem_t turno;
pthread_mutex_t  mutexGatos, mutexPerros;

void* perro(){
	while(1){
		sem_wait(&turno);
		sem_wait(&PerrosMax);
		pthread_mutex_lock(&mutexPerros);
		sem_post(&turno);
		if(sem_trywait(&PerrosDentro) == 0){
			//No soy el primer perro
			sem_post(&PerrosDentro);
		}else{
			//Soy el primer perro
			//Bloque el recurso
			sem_wait(&peluqueria);
		}
		//Cuento que un perro entro
		sem_post(&PerrosDentro);
		printf("El perro %i entro\n", pthread_self());

		pthread_mutex_unlock(&mutexPerros);

		printf("Perro %i: se baña\n", pthread_self());
		//simulo el tiempo
		sleep(0);


		pthread_mutex_lock(&mutexPerros);
		
		printf("El perro %i se fue\n", pthread_self());
		sem_wait(&PerrosDentro);

		if(sem_trywait(&PerrosDentro) == 0){
			//No soy el ultimo perro
			sem_post(&PerrosDentro);
		}else{
			//Soy el ultimo perro
			//Libero el recurso
			sem_post(&peluqueria);
		}
		pthread_mutex_unlock(&mutexPerros);
		sem_post(&PerrosMax);
	}
}
void* gato(){
	while(1){
		sem_wait(&turno);
		sem_wait(&GatosMax);
		pthread_mutex_lock(&mutexGatos);
		sem_post(&turno);
		if(sem_trywait(&GatosDentro) == 0){
			//No soy el primer gato
			sem_post(&GatosDentro);
		}else{
			//Soy el primer gato
			//Bloque el recurso
			sem_wait(&peluqueria);
		}
		//Cuento que un gato entro
		printf("El gato %i entro\n", pthread_self());
		sem_post(&GatosDentro);
		pthread_mutex_unlock(&mutexGatos);

		printf("Gato %i: se baña\n", pthread_self());
		//simulo el tiempo
		sleep(0);

		pthread_mutex_lock(&mutexGatos);
		
		printf("El gato %i se fue\n",pthread_self());
		sem_wait(&GatosDentro);
		if(sem_trywait(&GatosDentro) == 0){
			//No soy el ultimo gato
			sem_post(&GatosDentro);
		}else{
			//Soy el ultimo gato
			//Libero el recurso
			sem_post(&peluqueria);
		}
		pthread_mutex_unlock(&mutexGatos);
		sem_post(&GatosMax);
	}
}


int main(int argc, char **argv){
	pthread_t perros[threadsPerros];
	pthread_t gatos[threadsGatos];

	//Contadores de lugares disponibles
	sem_init(&turno,0,1);
	sem_init(&peluqueria,0,1);
	sem_init(&PerrosDentro,0,0);
	sem_init(&GatosDentro,0,0);
	sem_init(&PerrosMax,0,cantPerros);
	sem_init(&GatosMax,0,cantGatos);
	//Mutexs 
	pthread_mutex_init(&mutexPerros, NULL);
	pthread_mutex_init(&mutexGatos, NULL);
    for (int i = 0; i < threadsGatos; i++){
		pthread_create(&gatos[i],NULL, &gato, NULL);
	}
	for (int i = 0; i < threadsPerros; i++){
		pthread_create(&perros[i],NULL, &perro, NULL);
	}
	
	
	for (int i = 0; i < threadsPerros; i++){
		pthread_join(perros[i],NULL);
	}
	for (int i = 0; i < threadsGatos; i++){
		pthread_join(gatos[i],NULL);
	}

	sem_close(&turno);
	sem_close(&peluqueria);
	sem_close(&PerrosDentro);
	sem_close(&GatosDentro);
	sem_close(&PerrosMax);
	sem_close(&GatosMax);
    pthread_mutex_destroy(&mutexPerros);
    pthread_mutex_destroy(&mutexGatos);
	return 0;
}
/*INSTRUCCIONES PARA COMPILAR: 
gcc peluqueria.c -o peluqueria -lpthread
INSTRUCCIONES PARA EJECUTAR:
./peluqueria*/
