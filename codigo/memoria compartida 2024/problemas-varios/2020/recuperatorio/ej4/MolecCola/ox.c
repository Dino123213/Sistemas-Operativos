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
      for (int i = 0; i < 15; ++i) {//se duerme 2 seg despues de generar 15
        msgrcv(idmsg , &msg, MSG_SIZE, OXVACIO, 0);
            msgrcv(idmsg , &msg, MSG_SIZE, OXMUTEX, 0);
                
                printf("Generé Oxígeno\n"); 
			msg.tipo=OXMUTEX;
            msgsnd(idmsg , &msg, MSG_SIZE, 0);
        msg.tipo=OXLLENO;
        msgsnd(idmsg , &msg, MSG_SIZE, 0);
        
        
        sleep(1);
    }
        sleep(2);
    }
    
    
    
    
    
 }
