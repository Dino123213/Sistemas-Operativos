#include "estructura.h"

int main(){
    char *argsCorredor[] = {"./corredor", NULL};
    char *argsMartillo[] = {"./martillo", NULL};
    char *argsJabalina[] = {"./jabalina", NULL};
	key_t clave = generateKey();
	int id = shmget(clave, 1024, IPC_CREAT | 0666);
	if(id < 0){
		printf("error fallo el shmget\n");
		exit(1);
	}
	

	//Asociar la memoria a la estructura aCompartir
	struct aCompartir* recursosCompartidos = (struct aCompartir*) shmat(id, 0, 0);
	if(recursosCompartidos <= (struct aCompartir*) (0)){
		printf("Error fallo shmat \n");
		exit(2);
	}
	for(int i =0; i<CANT;i++){
            int pidC= fork();
	    if(pidC == 0){
			    execv("./corredor", argsCorredor);
			    exit(3);
		    }
        }
	
	for(int i =0; i<CANT;i++){
            int pidM= fork();
	    if(pidM == 0){
			    execv("./martillo", argsMartillo);
			    exit(3);
		    }
        }
	
	for(int i =0; i<CANT;i++){
            int pidJ= fork();
	    if(pidJ == 0){
			    execv("./jabalina", argsJabalina);
			    exit(3);
		    }
        }
	sem_init(&(recursosCompartidos->instalaciones),1,1);
	sem_init(&(recursosCompartidos->cantCorredores),1,0);
	sem_init(&(recursosCompartidos->mutex),1,1);
	
	for(int i=0; i<CANT*3;i++){
		wait(NULL);	
	}
	
	
	return 0;
	
	
}
