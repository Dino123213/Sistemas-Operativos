#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#define MAX 10
#define FIN -1
#define TRUE 1
#define FALSE 0
int buffer[MAX];
int pos_productor=0;
sem_t huecos, elementos;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int generardato(int r){return r%256;}
int numeroaleatorio(int r){return r%100;}
void* productor(void* p){
	srand(getpid());
	int num,dato;
	int n = numeroaleatorio(rand())%20;
	printf("Productor con %d datos \n", n);
	for(num = 0;num < n; num++){
		dato = generardato(rand());
		sem_wait(&huecos);
		pthread_mutex_lock(&mutex);
		buffer[pos_productor] = dato;
		pos_productor = (pos_productor + 1)%MAX;
		pthread_mutex_unlock(&mutex);
		sem_post(&elementos);
	}
	printf("termino p\n");
	pthread_exit(NULL);
}

void* consumidor(void* p){
	int pos_consumidor = 0;
	int dato;
	int continuar = TRUE;
	while(continuar){
		sem_wait(&elementos);
		pthread_mutex_lock(&mutex);
		dato = buffer[pos_consumidor];
		pthread_mutex_unlock(&mutex);
		pos_consumidor = (pos_consumidor+1)%MAX;
		sem_post(&huecos);
		if(dato==FIN)
			continuar = FALSE;
		else 
			printf("Numero aleatorio: %d\n",dato);
	}
	printf("termino c\n");
	pthread_exit(NULL);
}

int main(int argc, char** argv){
	pthread_t productor_t1,productor_t2,productor_t3,consumidor_t;
	sem_init(&huecos,0,MAX);
	sem_init(&elementos,0,0);
	pthread_create(&productor_t1,NULL,productor,NULL);
	pthread_create(&productor_t2,NULL,productor,NULL);
	pthread_create(&productor_t3,NULL,productor,NULL);
	pthread_create(&consumidor_t,NULL,consumidor,NULL);
	pthread_join(productor_t1,NULL);
	pthread_join(productor_t2,NULL);
	pthread_join(productor_t3,NULL);
	sem_wait(&huecos);
	pthread_mutex_lock(&mutex);
	buffer[pos_productor] = FIN;
	pos_productor = (pos_productor + 1)%MAX;
	pthread_mutex_unlock(&mutex);
	sem_post(&elementos);
	pthread_join(consumidor_t,NULL);
	return 0;
}



