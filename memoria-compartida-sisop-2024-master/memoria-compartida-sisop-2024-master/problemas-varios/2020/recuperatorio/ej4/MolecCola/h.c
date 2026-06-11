#include "estructura.h"
int main() {
    key_t clave = generateKey();
    int idmsg = msgget(clave, 0666);
    if (idmsg < 0) {
        printf("error al obtener la cola de mensajes\n");
        exit(1);
    }

    struct mensaje msg;
    
    for (int i = 0; i < CANT; ++i) {
      for (int i = 0; i < 20; ++i) {
        msgrcv(idmsg , &msg, MSG_SIZE, HVACIO, 0);
            msgrcv(idmsg , &msg, MSG_SIZE, HMUTEX, 0);
                
                printf("Generé Hidrogeno\n"); 
			msg.tipo=HMUTEX;
            msgsnd(idmsg , &msg, MSG_SIZE, 0);
        msg.tipo=HLLENO;
        msgsnd(idmsg , &msg, MSG_SIZE, 0);

        sleep(1);
    }
    sleep(3);
}
    
    
    
    
    
 }
