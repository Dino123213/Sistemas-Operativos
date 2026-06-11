#include "estructura.h"

int main(){

   
    char *argsOx[] = {"./ox", NULL};
    char *argsH[] = {"./h", NULL};
    char *argsAgua[] = {"./agua", NULL};
	key_t clave = generateKey();
	int idmsg = msgget(clave, 0666| IPC_CREAT);
	if(idmsg < 0){
		printf("error en la creación de la cola de mensajes\n");
		exit(1);
	}
	  
  
	struct mensaje  msg;
        
        for(int i=0; i<20; i++){
         msg.tipo = OXVACIO;
         msgsnd(idmsg , &msg, MSG_SIZE,0);
         msg.tipo = HVACIO;
         msgsnd(idmsg , &msg, MSG_SIZE,0);
      }
      
      msg.tipo=OXMUTEX;
      msgsnd(idmsg , &msg, MSG_SIZE,0);
      msg.tipo=HMUTEX;
      msgsnd(idmsg , &msg, MSG_SIZE,0);
      
	
      
      


	int pidO = fork();
		if(pidO == 0){
			execv("./ox", argsOx);
			exit(3);
		}

		

	int pidH = fork();
		if(pidH == 0){
			execv("./h", argsH);
			exit(3);
		}

        int pidV = fork();
		if(pidV == 0){
			execv("./agua", argsAgua);
			exit(3);
		}

	wait(NULL);for(int i=0; i<3;i++){
		wait(NULL);	
	}
	
	wait(NULL);
	return 0;
	
	
}
