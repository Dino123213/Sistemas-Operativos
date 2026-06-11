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
		msgrcv(idmsg , &msg, MSG_SIZE, INSTALACIONES, 0);
		printf("Llego un lanzador de martillo\n");
		fflush(stdout);
		sleep(1);
		printf("Se va lanzador martillo\n");
		fflush(stdout);
		msg.tipo=INSTALACIONES;
		msgsnd(idmsg , &msg, MSG_SIZE,0 );
		
	}
	


    return 0;
}
