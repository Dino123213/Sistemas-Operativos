#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "PC.h"

int main() {
    pid_t pid_prod, pid_cons;

	key_t clave = generateKey();
	int id = shmget(clave, 1024, IPC_CREAT | 0666);
	if(id < 0){
		printf("error fallo el shmget\n");
		exit(1);
	}
	//Asociar la memoria a la estructura aCompartir
	struct aCompartir* recursosCompartidos = (struct aCompartir*) shmat(id, 0, 0);

	//Valor inicial al segmento de memoria compartida
	sem_init(&(recursosCompartidos->vacio),1,20);
	sem_init(&(recursosCompartidos->lleno),1,0);
	sem_init(&(recursosCompartidos->mutex),1,1);
	recursosCompartidos-> in = 0;
	recursosCompartidos-> out = 0;	
    // Crear el proceso del productor
    for(int i = 0; i<3;i++){
	if ((pid_prod = fork()) == 0) {
	    char id_str[3];
	    snprintf(id_str, sizeof(id_str),"%d",i);
	    execlp("./prod", "prod",id_str,NULL); // Ejecutar Prod
	    perror("Error al ejecutar Prod");
	    exit(1);
	} else if (pid_prod < 0) {
	    perror("Error al crear proceso del productor");
	    exit(1);
	}
    }

    // Crear el proceso del consumidor
    for(int i = 0; i<5; i++){
	if ((pid_cons = fork()) == 0) {
	      char id_str[4];
	    snprintf(id_str, sizeof(id_str),"%d",i);
	    execlp("./cons", "cons",id_str,NULL); // Ejecutar Cons
	    perror("Error al ejecutar Cons");
	    exit(1);
	} else if (pid_cons < 0) {
	    perror("Error al crear proceso del consumidor");
	    exit(1);
	}
    }

    // Esperar a que terminen ambos procesos
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);


    return 0;
}
