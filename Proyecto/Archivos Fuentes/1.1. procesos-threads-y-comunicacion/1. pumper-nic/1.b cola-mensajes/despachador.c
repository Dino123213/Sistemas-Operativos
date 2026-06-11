#include "constantes.h"

int main() {
    struct msg_buffer orden;

    int msg_id = msgget(KEY, 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    while (1) {
        if (msgrcv(msg_id, &orden, LONGITUD, VIP_O_NORMAL, 0) != -1) {
            char tipo_cliente_s[10]; 

            if (orden.tipo == CLIENTE_VIP) {
                strcpy(tipo_cliente_s, "VIP"); 
            } 
            else {
                strcpy(tipo_cliente_s, "NORMAL");
            }

            printf(YELLOW "El despachador atiende al cliente %d de tipo %s.\n" RESET, orden.id_cliente, tipo_cliente_s);
            usleep(TIME_U);
            orden.tipo = orden.menu;        
            printf(DARK_YELLOW "El despachador manda a realizar un menu %d para el cliente %d de tipo %s.\n" RESET, orden.menu, orden.id_cliente, tipo_cliente_s);
            msgsnd(msg_id, &orden, LONGITUD, 0);
        } 
        else {
            perror("Error al recibir el mensaje.\n");
        }
    }

    exit(0);
}