#include "constantes.h"

int main(int argc, char *argv[]) {
    int id_cliente = atoi(argv[1]);
    int tipo_cliente = atoi(argv[2]);
    int id_recibir, menu; 

    struct msg_buffer mensaje;

    int msg_id = msgget(KEY, 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    mensaje.id_cliente = id_cliente;
    id_recibir = id_cliente + DIFERENCIADOR;

    for(int i = 0; i < CANT_ITERACIONES; i++) {
        srand(time(NULL) + getpid() + i);

        menu = HAMBURGUESA + rand() % 3;
        mensaje.menu = menu;

        int cola_llena = rand() % 20;

        if(cola_llena == 1) {
            printf(RED "La cola está llena y un cliente se va.\n" RESET);
            continue; 
        }

        char tipo_cliente_str[10]; 
        if (tipo_cliente == CLIENTE_VIP) {
            strcpy(tipo_cliente_str, "VIP"); 
        } 
        else {
            strcpy(tipo_cliente_str, "Normal");
        }

        printf(ORANGE "Cliente %d de tipo %s quiere realizar un pedido de menú %d.\n" RESET, id_cliente, tipo_cliente_str, menu);
        usleep(TIME_U);

        mensaje.tipo = tipo_cliente;
        if (msgsnd(msg_id, &mensaje, LONGITUD, 0) == -1) {
            perror("Error al enviar el mensaje");
            exit(1);
        } 
        
        if (msgrcv(msg_id, &mensaje, LONGITUD, id_recibir, 0) == -1) {
            perror("Error al recibir el mensaje");
            exit(1); 
        }

        printf(ORANGE "Cliente %d recibió su pedido de menú %d.\n" RESET, id_cliente, menu);
        usleep(TIME_U);
    }
    
    exit(0);
}