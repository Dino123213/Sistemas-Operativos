#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define ESCRITORES 3
#define LECTORES 3
#define ITER 10

struct semaforos{
	sem_t wsem; 
	sem_t rmutex;
	sem_t rsem;
};
	
int escritor(int id) {
	key_t key = ftok("LEArchivo.c", 76);
	int shmid = shmget(key, sizeof(struct semaforos), 0666);
	if (shmid == -1) {
		perror("error en shmget escritor");
		exit(1);
	}
	struct semaforos *sem = (struct semaforos *)shmat(shmid,NULL,0);
	if (sem == (void *) -1){
		perror("error en shmat escritor");
		exit(1);
	}
	
	for (int i=0; i<ITER; i++) {
		sem_wait(&sem->wsem);
		printf("Escritor %d se encuentra escribiendo...\n",id);
		sleep(2);
		printf("Escritor %d se va...\n",id);
		sem_post(&sem->wsem);
		sleep(1);
	}
	printf("Escritor %d termino\n",id);
	shmdt(sem);
	return 0;
}

int lector(int id) {
	key_t key = ftok("LEArchivo.c", 76);
	int shmid = shmget(key, sizeof(struct semaforos), 0666);
	if (shmid == -1) {
		perror("error en shmget lector");
		exit(1);
	}
	struct semaforos *sem = (struct semaforos *)shmat(shmid,NULL,0);
	if (sem == (void *) -1){
		perror("error en shmat lector");
		exit(1);
	}
	
	for (int i=0; i<ITER; i++) {
		sem_wait(&sem->rmutex);
		if (sem_trywait(&sem->rsem) == 0) {
			sem_post(&sem->rsem);
		}
		else {
			sem_wait(&sem->wsem);
		}
		sem_post(&sem->rsem);
		sem_post(&sem->rmutex);
		printf("Lector %d leyendo...\n",id);
		sleep(2);
		sem_wait(&sem->rmutex);
		printf("Lector %d termino de leer...\n",id);
		sem_wait(&sem->rsem);
		if (sem_trywait(&sem->rsem) == -1) {
			sem_post(&sem->wsem);
		}
		else {
			sem_post(&sem->rsem);
		}
		sem_post(&sem->rmutex);
		sleep(1); //ESTA BIEN USAR SLEEP PARA MINIMIZAR INANICION???
	}
	printf("Lector %d termino\n",id);
	shmdt(sem);
	return 0;
}

int main() {
	key_t key = ftok("LEArchivo.c", 76);
	int shmid;
	if ((shmid = shmget(key, sizeof(struct semaforos), 0666)) != -1) {
		shmctl(shmid, IPC_RMID, NULL);
	}
	
	shmid = shmget(key, sizeof(struct semaforos), IPC_CREAT | 0666);
	if (shmid == -1) {
		perror("error en shmget");
		exit(1);
	}
	
	struct semaforos  *sem = (struct semaforos *)shmat(shmid, NULL, 0);
	if (sem == (void *) -1){
		perror("error en shmat");
		exit(1);
	}
	
	sem_init(&sem->wsem, 1, 1);
	sem_init(&sem->rmutex, 1, 1);
	sem_init(&sem->rsem, 1, 0);
	
	pid_t pidEscritor[ESCRITORES];
	pid_t pidLector[LECTORES];
	
	for (int i=0; i<ESCRITORES; i++) {
		pidEscritor[i] = fork();
		if (pidEscritor[i] == 0) {
			escritor(i+1);
			exit(0);
		}
	}
	
	for (int i=0; i<LECTORES; i++) {
		pidLector[i] = fork();
		if (pidLector[i] == 0) {
			lector(i+1);
			exit(0);
		}
	}
	
	for (int i=0; i<ESCRITORES+LECTORES; i++) {
		wait(NULL);
	}
	
	shmctl(shmid, IPC_RMID, NULL);
	
	return 0;
	
}

