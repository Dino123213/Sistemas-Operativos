
#include "estructura.h"

int main() {
    key_t clave = generateKey();
	int id = shmget(clave, 1024, 0);
	if(id < 0){
		printf("error fallo el shmget\n");
		exit(1);
	}
	
	struct aCompartir* jabalina = (struct aCompartir*) shmat(id,0,0);
	if(jabalina <= (struct aCompartir*) (0)){
		printf("Error fallo shmat \n");
		exit(2);
	}
	
    while(1){
		sleep(1);
			sem_wait(&(jabalina-> instalaciones)); 
			printf("Llego un lanzador de jabalina\n");
			fflush(stdout);
			sleep(1);
			printf("Se va lanzador jabalina\n");
			fflush(stdout);
			sem_post(&(jabalina-> instalaciones)); 
		
	}
	


    return 0;
}

