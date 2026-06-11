
#include "estructura.h"

int main()
{
	char *argsAuto[] = {"./auto", NULL};
	char *argsPasajeros[] = {"./pasajero", NULL};
	key_t clave = generateKey();
	int id = msgget(clave, 0666| IPC_CREAT);
	if(id < 0){
		printf("error en la creación de la cola de mensajes\n");
		exit(1);
	}
	  struct mensaje msg;
    // Vaciar la cola de mensajes
    while (1) {
        // Intentar recibir un mensaje
        ssize_t result = msgrcv(id, &msg, MSG_SIZE, 0, IPC_NOWAIT);
        if (result < 0) {
            // Si no hay más mensajes, salir del bucle
            if (errno == ENOMSG) {
                break; // No hay más mensajes
            } else {
                perror("Error al recibir mensaje");
                break;
            }
        }
        
		}


		int pid = fork();
		if(pid == 0){
			execv("./auto", argsAuto);
			exit(3);
		}
	
		for(int i =0; i<CANT_PASAJEROS; i++){
		int pidP = fork();
		if(pidP == 0){
			execv("./pasajero", argsPasajeros);
			printf("error");
			exit(4);
			}
		}
	    msg.tipo = MENSAJE_ASIENTOS;
        for (int i = 0; i < NUM_ASIENTOS; i++) {
            msgsnd(id, &msg, MSG_SIZE, 0);
        }

	
	wait(NULL);for(int i=0; i<CANT_PASAJEROS;i++){
		wait(NULL);	
	}
	
	wait(NULL);
	return 0;
	
	
}
