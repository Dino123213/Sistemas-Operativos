#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>

sem_t cant_hombres, cant_mujeres, banio;
pthread_mutex_t mutex_h = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_m = PTHREAD_MUTEX_INITIALIZER;

void *hombre(void *args)
{
	while (1)
	{
		pthread_mutex_lock(&mutex_h);
		if (sem_trywait(&cant_hombres))
			sem_post(&cant_hombres);
		else
			sem_wait(&banio);
		sem_post(&cant_hombres);
		pthread_mutex_unlock(&mutex_h);
		printf("H entra al banio\n");
		sem_wait(&cant_hombres);
		pthread_mutex_lock(&mutex_h);
		if (sem_trywait(&cant_hombres))
			sem_post(&cant_hombres);
		else
			sem_post(&banio);
		pthread_mutex_unlock(&mutex_h);
		printf("H sale del banio\n");
	}
}

void *mujer(void *args)
{
	while (1)
	{
		pthread_mutex_lock(&mutex_m);
		if (sem_trywait(&cant_mujeres))
			sem_post(&cant_mujeres);
		else
			sem_wait(&banio);
		sem_post(&cant_mujeres);
		pthread_mutex_unlock(&mutex_m);
		printf("M entra al banio\n");
		sem_wait(&cant_mujeres);
		pthread_mutex_lock(&mutex_m);
		if (sem_trywait(&cant_mujeres))
			sem_post(&cant_mujeres);
		else
			sem_post(&banio);
		pthread_mutex_unlock(&mutex_m);
		printf("M sale del banio\n");
	}
}

int main(int argc, char **argv)
{
	pthread_t H[10], M[10];
	sem_init(&banio, 0, 1);
	sem_init(&cant_mujeres, 0, 0);
	sem_init(&cant_hombres, 0, 0);
	for (int i = 0; i < 10; i++)
		pthread_create(&H[i], NULL, hombre, NULL);
	for (int i = 0; i < 10; i++)
		pthread_create(&M[i], NULL, mujer, NULL);
	for (int i = 0; i < 10; i++)
		pthread_join(H[i], NULL);
	for (int i = 0; i < 10; i++)
		pthread_join(M[i], NULL);
	return 0;
}
