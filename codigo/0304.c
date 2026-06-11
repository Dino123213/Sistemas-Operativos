#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>

struct mensaje {
    long tipo;      // DEBE ser el primer campo y de tipo long
    char texto[100];
};

int main(int argc, char *argv[]) {
    
    if(argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    
    if(n <= 0) {
        fprintf(stderr, "N debe ser un número positivo\n");
        return 1;
    }

    key_t key = ftok("0304.c", 65); // genera una clave

    //creo una cola de mensajes
    int msgid = msgget(key, 0666 | IPC_CREAT); //obtengo la id de la cola de mensajes
    if(msgid == -1) {
        perror("Error al crear la cola de mensajes");
        return 1;
    }

    struct mensaje msg;

    // creo procesos hijos que escriban en la cola de mensajes
    for(int i = 0; i < n; i++) {
        pid_t pid = fork();
        if(pid < 0) {
            perror("Error en fork");
            return 1;
        }
        if(pid == 0) { // proceso hijo
            //genero un mensaje y lo guardo en un string para despues pasarselo a la cola de mensajes
            char mensaje[100];
            sprintf(mensaje, "Hola desde el proceso %d", getpid());
            msg.tipo = 1L; // tipo de mensaje (debe ser > 0 para msgsnd/msgrcv)
            strcpy(msg.texto, mensaje); // Copiamos el contenido del string

            //envio el mensaje
            if(msgsnd(msgid, &msg, sizeof(msg.texto), 0) == -1) {
                perror("Error al enviar el mensaje");
                exit(1);
            }
            exit(0); // termino el proceso hijo
        }
    }

    // proceso padre espera a que terminen los hijos
    for(int i = 0; i < n; i++) {
        wait(NULL);
    }
    
    // proceso padre lee los mensajes y los imprime
    for(int i = 0; i < n; i++) {
        //recibo el mensaje
        if(msgrcv(msgid, &msg, sizeof(msg.texto), 1, 0) == -1) {
            perror("Error al recibir el mensaje");
            return 1;
        }
        printf("Mensaje recibido: %s\n", msg.texto);
    }
    
    // Elimino la cola de mensajes
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}