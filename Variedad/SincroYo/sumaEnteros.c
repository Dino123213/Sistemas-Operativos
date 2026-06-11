#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct mensaje {
    long mtype;   // debe ser long y primer campo
    int inicio;
    int fin;
    int resultado;
};

#define MSG_SIZE (sizeof(struct mensaje) - sizeof(long))

void suma(int key, int arreglo[], int hijoId) {
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("suma: msgget");
        exit(1);
    }

    struct mensaje msg;
    ssize_t r = msgrcv(msgid, &msg, MSG_SIZE, (long)(hijoId), 0);
    if (r == -1) {
        perror("suma: msgrcv");
        exit(1);
    }

    // Validar rangos
    if (msg.inicio < 0) msg.inicio = 0;
    if (msg.fin < msg.inicio) msg.fin = msg.inicio;

    int resultadoTotal = 0;
    for (int j = msg.inicio; j <= msg.fin && j < 10; j++) {
        resultadoTotal += arreglo[j];
        printf("Hijo %d: sumando arreglo[%d] = %d\n", hijoId + 1, j, arreglo[j]);
    }

    msg.resultado = resultadoTotal;
    printf("Hijo %d: resultado parcial = %d\n", hijoId + 1, resultadoTotal);
    if (msgsnd(msgid, &msg, MSG_SIZE, 0) == -1) {
        perror("suma: msgsnd");
        exit(1);
    }
}

int main() {
    // Intenta crear key con un archivo; si falla, usa "."
    key_t key = ftok("sumaEnteros", 65);
    if (key == -1) {
        perror("ftok('sumaEnteros') falló, usando ftok('.') en su lugar");
        key = ftok(".", 65);
        if (key == -1) {
            perror("ftok fallback falló");
            exit(1);
        }
    }

    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Error al crear/obtener la cola de mensajes");
        exit(1);
    }

    struct mensaje msg1;
    struct mensaje msg2;
    int returnValue = 0;
    int arreglo[10] = {2,2,2,2,2,3,3,3,3,3};

    // Mensaje para hijo 1 (tipo 1)
    msg1.mtype = 1;
    msg1.inicio = 0;
    msg1.fin = 4;
    msg1.resultado = 0;
    if (msgsnd(msgid, &msg1, MSG_SIZE, 0) == -1) {
        perror("msgsnd padre->hijo1");
        exit(1);
    }

    // Mensaje para hijo 2 (tipo 2)
    msg2.mtype = 2;
    msg2.inicio = 5;
    msg2.fin = 9;
    msg2.resultado = 0;
    if (msgsnd(msgid, &msg2, MSG_SIZE, 0) == -1) {
        perror("msgsnd padre->hijo2");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // hijo 1
        suma(key, arreglo, 1);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork 2");
        exit(1);
    } else if (pid2 == 0) {
        // hijo 2
        suma(key, arreglo, 2);
        exit(0);
    }

   
    // Esperar hijos
    for (int i = 0; i < 2; i++) wait(NULL);

  

    // Padre: recibe dos mensajes de respuesta (cualquiera el orden)

        if (msgrcv(msgid, &msg1, MSG_SIZE, 0, 0) == -1) {
            perror("msgrcv padre");
            exit(1);
        }
        printf("Padre: recibió resultado parcial (tipo=%ld) = %d\n", msg1.mtype, msg1.resultado);
        returnValue += msg1.resultado;
        
        
        if (msgrcv(msgid, &msg2, MSG_SIZE, 0, 0) == -1) {
            perror("msgrcv padre");
            exit(1);
        }
        printf("Padre: recibió resultado parcial (tipo=%ld) = %d\n", msg2.mtype, msg2.resultado);
        returnValue += msg2.resultado;

          printf("Padre: resultado final de la suma es %d\n", returnValue);
    // Eliminar cola
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl(IPC_RMID)");
        // no hacemos exit porque ya terminamos el trabajo
    }

     


    return returnValue;
}
