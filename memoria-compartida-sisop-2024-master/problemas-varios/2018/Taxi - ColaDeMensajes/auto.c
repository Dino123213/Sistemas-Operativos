#include "estructura.h"

int main() {
    key_t clave = generateKey();
    int id = msgget(clave, 0666);
    if (id < 0) {
        printf("error al obtener la cola de mensajes\n");
        exit(1);
    }

    struct mensaje msg;
    int i;
    while (1) {
        printf("LLEGA AUTO - Esperando pasajeros\n");
        fflush(stdout);
        
        for (i = 0; i < NUM_ASIENTOS; i++) {
            msgrcv(id, &msg, MSG_SIZE, MENSAJE_LLENO, 0);
           
        }

        printf(GREEN "VIAJANDO\n" RESET);
        sleep(6);
        printf(CYAN "LLEGADA DESTINO - Todos los pasajeros bajaron del auto\n" RESET);
        fflush(stdout);
        msg.tipo = MENSAJE_ASIENTOS;
        for (i = 0; i < NUM_ASIENTOS; i++) {
            msgsnd(id, &msg, MSG_SIZE, 0);
        }
        
    }

    return 0;
}
