#include "PC.h"

int main(int argc, char*argv[]){
	key_t clave = generateKey();
	int id = shmget(clave, 1024, 0);
	if(id < 0){
		printf("error fallo el shmget\n");
		exit(1);
	}

	struct aCompartir* recursosCompartidos = (struct aCompartir*) shmat(id,0,0);
	int cons_id = atoi(argv[1]);
	printf(RED "Llegó el consumidor %d \n" RESET, cons_id);
	while(1){

			sem_wait(&(recursosCompartidos-> lleno));
			sem_wait(&(recursosCompartidos-> mutex)); //MUTEX
			printf(GREEN "Consumior %d: Consumi %d \n" RESET,cons_id, recursosCompartidos->buf[recursosCompartidos->out]);
			fflush(stdout);
			recursosCompartidos -> out= ((recursosCompartidos -> out) + 1) % TAM_BUFFER;
			sem_post(&(recursosCompartidos-> mutex));	//MUTEX
			sem_post(&(recursosCompartidos -> vacio));
			
			sleep(1);
	}
	return 0;


}
