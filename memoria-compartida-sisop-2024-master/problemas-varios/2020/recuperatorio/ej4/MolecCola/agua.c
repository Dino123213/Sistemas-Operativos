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
      //oxigeno
        msgrcv(idmsg , &msg, MSG_SIZE, OXLLENO, 0);
            msgrcv(idmsg , &msg, MSG_SIZE, OXMUTEX, 0);
                
                printf("Tomé un Oxígeno\n"); 
			msg.tipo=OXMUTEX;
            msgsnd(idmsg , &msg, MSG_SIZE, 0);
        msg.tipo=OXVACIO;
        msgsnd(idmsg , &msg, MSG_SIZE, 0);

        sleep(2);
    
    
    //hid
      for (int j = 0; j < 2; ++j) {
        msgrcv(idmsg , &msg, MSG_SIZE, HLLENO, 0);
            msgrcv(idmsg , &msg, MSG_SIZE, HMUTEX, 0);
                
                printf("Tomé un Hidrogeno\n"); 
			msg.tipo=HMUTEX;
            msgsnd(idmsg , &msg, MSG_SIZE, 0);
        msg.tipo=HVACIO;
        msgsnd(idmsg , &msg, MSG_SIZE, 0);

        sleep(2);
       }
       
        printf("Se generó una molécula de agua!!\n"); 
        sleep(1);
    }
    
    
  }
    
    
    
    
