#include "PC.h"
#include <time.h>
int main(int argc, char*argv[]){
	key_t clave = generateKey();
	int id = shmget(clave, 1024, 0);
	if(id < 0){
		printf("error fallo el shmget\n");
		exit(1);
	}

	struct aCompartir* recursosCompartidos = (struct aCompartir*) shmat(id,0,0);
	int prod_id = atoi(argv[1]);
	printf(MAGENTA "Llegó el productor %d \n" RESET, prod_id);
		unsigned int seed = time(NULL) + getpid();
		srandom(seed);
	for(int k = 0; k < 100; k++){
			sem_wait(&(recursosCompartidos-> vacio));
			sem_wait(&(recursosCompartidos-> mutex)); //MUTEX
				int prod = random() % 100;
				(recursosCompartidos -> buf[recursosCompartidos -> in]) = prod;
				printf(CYAN "Productor %d: Produje un %d \n" RESET,prod_id, prod);
				fflush(stdout);
				(recursosCompartidos -> in) = ((recursosCompartidos -> in) + 1) % TAM_BUFFER;
				sem_post(&(recursosCompartidos -> lleno));
			sem_post(&(recursosCompartidos-> mutex)); //MUTEX

			sleep(2);

		}

	return 0;
}
