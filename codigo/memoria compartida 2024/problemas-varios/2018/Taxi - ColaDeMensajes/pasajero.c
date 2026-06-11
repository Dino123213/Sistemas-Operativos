#include "estructura.h"

int main() {
    key_t clave = generateKey();
    int id = msgget(clave, 0666);
    if (id < 0) {
        printf("error al obtener la cola de mensajes\n");
        exit(1);
    }

    struct mensaje msg;

    // Recibir el mensaje que indica que hay asientos disponibles
    msgrcv(id, &msg, MSG_SIZE, MENSAJE_ASIENTOS, 0);
    // Enviar mensaje al auto indicando que el pasajero ha abordado  
    
    msg.tipo = MENSAJE_LLENO;
    msgsnd(id, &msg, MSG_SIZE, 0);
    printf(RESET "Soy pasajero, entro al auto\n");
    fflush(stdout);

    return 0;
}
