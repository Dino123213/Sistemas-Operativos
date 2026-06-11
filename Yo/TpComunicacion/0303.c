#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct mensaje {
    long tipo;        // Tipo de mensaje siempre va arriba del mensaje
    char texto[100];  
};

int main() {
    key_t key;
    int msgid;
    pid_t pid;
    struct mensaje msg;
    
    // Generar clave única
    key = ftok(".", 1); //Genera la clave a partir del path y un identificador
    //El punto ".": indica el directorio base que se usa para generar la clave, aca el directorio actual
    //El numero es un id arbitario.

    if (key == -1) {
        perror("Error en ftok");
        exit(1);
    }
    
    // Crear cola de mensajes
    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1) {
        perror("Error en msgget");
        exit(1);
    }
    
    pid = fork();
    
    if (pid == 0) {  // Proceso hijo
        // Recibir mensaje de tipo 1
        if (msgrcv(msgid, &msg, sizeof(msg.texto), 1, 0) == -1) { //El 1 indica que queremos recibir mensajes de tipo 1, si es =-1 hubo un error.
            //msgid=identificador de la cola, &msg=puntero a la estructura donde se guarda el msg, sizeof(msg.texto), X=1 acepta mensajes de tipo 1 si fuese 0 aceptaria cualquier tipo, 0=sin opciones
            //Z=0 es la flag, al estar en 0 es una llamada bloqueante
            perror("Error en msgrcv");
            exit(1);
        }
        
        printf("Hijo recibió mensaje tipo %ld: %s\n", msg.tipo, msg.texto);
        exit(0);
    } else {  // Proceso padre
        msg.tipo = 1;
        strcpy(msg.texto, "Mensaje a través de cola"); //Copia caracter por caracter la cadena hacia msg.texto
        
        // Enviar mensaje
        if (msgsnd(msgid, &msg, sizeof(msg.texto), 0) == -1) {
            perror("Error en msgsnd");
            exit(1);
        }
        
        printf("Padre envió mensaje\n");
        wait(NULL);
        
        // Eliminar cola de mensajes
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("Error eliminando cola");
            exit(1);
        }
    }
    return 0;
}
