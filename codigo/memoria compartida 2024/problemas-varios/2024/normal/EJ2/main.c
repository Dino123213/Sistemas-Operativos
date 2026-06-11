#include "estructura.h"

int main(){
    char *argsCorredor[] = {"./corredor", NULL};
    char *argsMartillo[] = {"./martillo", NULL};
    char *argsJabalina[] = {"./jabalina", NULL};
	key_t clave = generateKey();
	int idmsg = msgget(clave, 0666| IPC_CREAT);
	if(idmsg < 0){
		printf("error en la creación de la cola de mensajes\n");
		exit(1);
	}
	  
  
	struct mensaje msg;

      



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
	
		
        msg.tipo = INSTALACIONES;
       
        msgsnd(idmsg , &msg, MSG_SIZE,0);
	
	msg.tipo = MUTEX;
       
        msgsnd(idmsg , &msg, MSG_SIZE,0);


	for(int i=0; i<CANT*3;i++){
		wait(NULL);	
	}
	
	
	return 0;
	
	
}
