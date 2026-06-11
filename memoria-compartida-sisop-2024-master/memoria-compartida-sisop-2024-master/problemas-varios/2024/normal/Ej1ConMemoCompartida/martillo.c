#include "estructura.h"

int main() {
    key_t clave = generateKey();
	int id = shmget(clave, 1024, 0);
	if(id < 0){
		printf("error fallo el shmget\n");
		exit(1);
	}
	
	struct aCompartir* martillo = (struct aCompartir*) shmat(id,0,0);
	if(martillo <= (struct aCompartir*) (0)){
		printf("Error fallo shmat \n");
		exit(2);
	}
	
    while(1){
		sleep(1);
			sem_wait(&(martillo-> instalaciones)); 
			printf("Llego un lanzador de martillo\n");
			fflush(stdout);
			sleep(1);
			printf("Se va lanzador martillo\n");
			fflush(stdout);
			sem_post(&(martillo-> instalaciones)); 
		
	}
	


    return 0;
}
