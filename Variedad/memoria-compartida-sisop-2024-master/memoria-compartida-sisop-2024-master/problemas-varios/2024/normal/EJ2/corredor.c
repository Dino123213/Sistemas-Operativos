#include "estructura.h"

int main() {
    key_t clave = generateKey();
    int idmsg = msgget(clave, 0666);
    if (idmsg < 0) {
        printf("error al obtener la cola de mensajes\n");
        exit(1);
    }

    struct mensaje msg;

    while(1){
	sleep(1);
	msgrcv(idmsg , &msg, MSG_SIZE, MUTEX, 0);
	if(msgrcv(idmsg , &msg, MSG_SIZE, CANTC, IPC_NOWAIT) != 0){
		msgrcv(idmsg , &msg, MSG_SIZE, INSTALACIONES, 0);
	}else{
		msg.tipo=CANTC;
		msgsnd(idmsg , &msg, MSG_SIZE,0 );
	}
	
	msg.tipo=CANTC;
	msgsnd(idmsg , &msg, MSG_SIZE,0 );
	msg.tipo=MUTEX;
	msgsnd(idmsg , &msg, MSG_SIZE,0 );
	printf("Llego un corredor\n");
	fflush(stdout);
	sleep(1);
	
	
	msgrcv(idmsg , &msg, MSG_SIZE, MUTEX, 0);
	printf("Se va un corredor\n");
	fflush(stdout);
	msgrcv(idmsg , &msg, MSG_SIZE, CANTC, 0);
	if(msgrcv(idmsg , &msg, MSG_SIZE, CANTC, IPC_NOWAIT) != 0){
		msg.tipo=INSTALACIONES;
		msgsnd(idmsg , &msg, MSG_SIZE,0 );
	}else{
		msg.tipo=CANTC;
		msgsnd(idmsg , &msg, MSG_SIZE,0 );
	}
	msg.tipo=MUTEX;
	msgsnd(idmsg , &msg, MSG_SIZE,0 );
	

}	

    return 0;
}
