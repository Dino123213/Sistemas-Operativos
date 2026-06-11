#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>  // sem_t, sem_init, sem_wait, sem_post, sem_trywait
#include <unistd.h>     // fork, sleep
#include <sys/ipc.h>    // ftok, IPC_CREAT, IPC_RMID
#include <sys/shm.h>    // shmget, shmat, shmdt, shmctl
#include <sys/wait.h>   // wait

#define ESCRITORES 3    // Número de procesos escritores
#define LECTORES 3      // Número de procesos lectores
#define ITER 10         // Iteraciones que hace cada proceso

/* Estructura con los semáforos compartidos entre procesos */
struct semaforos{
	sem_t wsem;   // Semáforo para controlar acceso de escritores (binario, inicial=1)
	sem_t rmutex; // Mutex para proteger acceso al contador de lectores (binario, inicial=1)
	sem_t rsem;   // Semáforo contador de lectores activos (inicial=0)
};
	
int escritor(int id) {
	/* Obtener clave y conectarse a la memoria compartida donde están los semáforos */
	key_t key = ftok("LEArchivo.c", 76);
	int shmid = shmget(key, sizeof(struct semaforos), 0666);
	if (shmid == -1) {
		perror("error en shmget escritor");
		exit(1);
	}
	
	/* Adjuntar la memoria compartida al espacio de direcciones del proceso */
	struct semaforos *sem = (struct semaforos *)shmat(shmid,NULL,0);
	if (sem == (void *) -1){
		perror("error en shmat escritor");
		exit(1);
	}
	
	/* Realizar ITER escrituras */
	for (int i=0; i<ITER; i++) {
		/* Esperar acceso exclusivo (wsem=1 significa que no hay lectores ni otros escritores) */
		sem_wait(&sem->wsem);
		printf("Escritor %d se encuentra escribiendo...\n",id);
		sleep(2); /* Simular tiempo de escritura */
		printf("Escritor %d se va...\n",id);
		/* Liberar el acceso para que otros puedan escribir/leer */
		sem_post(&sem->wsem);
		sleep(1); /* Pausa entre escrituras */
	}
	printf("Escritor %d termino\n",id);
	
	/* Desconectar de la memoria compartida */
	shmdt(sem);
	return 0;
}

int lector(int id) {
	/* Obtener clave y conectarse a la memoria compartida donde están los semáforos */
	key_t key = ftok("LEArchivo.c", 76);
	int shmid = shmget(key, sizeof(struct semaforos), 0666);
	if (shmid == -1) {
		perror("error en shmget lector");
		exit(1);
	}
	
	/* Adjuntar la memoria compartida al espacio de direcciones del proceso */
	struct semaforos *sem = (struct semaforos *)shmat(shmid,NULL,0);
	if (sem == (void *) -1){
		perror("error en shmat lector");
		exit(1);
	}
	
	/* Realizar ITER lecturas */
	for (int i=0; i<ITER; i++) {
		/* PROTOCOLO DE ENTRADA DEL LECTOR:
		 * Adquirir mutex para acceso exclusivo al contador de lectores */
		sem_wait(&sem->rmutex);
		
		/* Verificar si es el primer lector */
		if (sem_trywait(&sem->rsem) == 0) {
			/* No es el primer lector, ya hay otros leyendo */
			sem_post(&sem->rsem);
		}
		else {
			/* Es el primer lector, debe bloquear a los escritores */
			sem_wait(&sem->wsem);
		}
		
		/* Incrementar contador de lectores */
		sem_post(&sem->rsem);
		/* Liberar mutex del contador */
		sem_post(&sem->rmutex);
		
		/* SECCIÓN CRÍTICA: LECTURA */
		printf("Lector %d leyendo...\n",id);
		sleep(2); /* Simular tiempo de lectura */
		
		/* PROTOCOLO DE SALIDA DEL LECTOR:
		 * Adquirir mutex para acceso exclusivo al contador */
		sem_wait(&sem->rmutex);
		printf("Lector %d termino de leer...\n",id);
		
		/* Decrementar contador de lectores */
		sem_wait(&sem->rsem);
		
		/* Verificar si es el último lector */
		if (sem_trywait(&sem->rsem) == -1) {
			/* Es el último lector, liberar acceso a escritores */
			sem_post(&sem->wsem);
		}
		else {
			/* No es el último, reponer el contador */
			sem_post(&sem->rsem);
		}
		
		/* Liberar mutex del contador */
		sem_post(&sem->rmutex);
		sleep(1); /* ESTA BIEN USAR SLEEP PARA MINIMIZAR INANICION??? */
	}
	printf("Lector %d termino\n",id);
	
	/* Desconectar de la memoria compartida */
	shmdt(sem);
	return 0;
}

int main() {
	/* Generar clave para memoria compartida usando el archivo LEArchivo.c */
	key_t key = ftok("LEArchivo.c", 76);
	int shmid;
	
	/* Limpiar memoria compartida previa si existe */
	if ((shmid = shmget(key, sizeof(struct semaforos), 0666)) != -1) {
		shmctl(shmid, IPC_RMID, NULL);
	}
	
	/* Crear nueva memoria compartida para los semáforos */
	shmid = shmget(key, sizeof(struct semaforos), IPC_CREAT | 0666);
	if (shmid == -1) {
		perror("error en shmget");
		exit(1);
	}
	
	/* Adjuntar memoria compartida al proceso padre */
	struct semaforos  *sem = (struct semaforos *)shmat(shmid, NULL, 0);
	if (sem == (void *) -1){
		perror("error en shmat");
		exit(1);
	}
	
	/* Inicializar semáforos:
	 * - wsem: binario (1) para control de escritores
	 * - rmutex: binario (1) para mutex de lectores
	 * - rsem: contador (0) para número de lectores activos
	 * El segundo parámetro (1) indica que son compartidos entre procesos */
	sem_init(&sem->wsem, 1, 1);
	sem_init(&sem->rmutex, 1, 1);
	sem_init(&sem->rsem, 1, 0);
	
	/* Arrays para almacenar PIDs de los procesos hijos */
	pid_t pidEscritor[ESCRITORES];
	pid_t pidLector[LECTORES];
	
	/* Crear procesos escritores */
	for (int i=0; i<ESCRITORES; i++) {
		pidEscritor[i] = fork();
		if (pidEscritor[i] == 0) {
			/* Código del proceso hijo escritor */
			escritor(i+1);
			exit(0); /* Terminar proceso hijo */
		}
	}
	
	/* Crear procesos lectores */
	for (int i=0; i<LECTORES; i++) {
		pidLector[i] = fork();
		if (pidLector[i] == 0) {
			/* Código del proceso hijo lector */
			lector(i+1);
			exit(0); /* Terminar proceso hijo */
		}
	}
	
	/* Esperar a que terminen todos los procesos hijos */
	for (int i=0; i<ESCRITORES+LECTORES; i++) {
		wait(NULL);
	}
	
	/* Limpiar: eliminar memoria compartida */
	shmctl(shmid, IPC_RMID, NULL);
	
	return 0;
}