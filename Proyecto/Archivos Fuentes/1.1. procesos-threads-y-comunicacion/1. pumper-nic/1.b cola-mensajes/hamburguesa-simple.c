#include "constantes.h"

int main() {
    struct msg_buffer order;

    int msg_id = msgget(KEY, 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    while (1) {
        msgrcv(msg_id, &order, LONGITUD, HAMBURGUESA, 0);
        printf(BLUE "Se prepara una hamburguesa simple para cliente %d.\n" RESET, order.id_cliente);
        usleep(TIME_U);
        order.tipo = order.id_cliente + DIFERENCIADOR;
        msgsnd(msg_id, &order, LONGITUD, 0);
    }

    exit(0);
}