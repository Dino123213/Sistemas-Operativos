#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

// Definir la estructura de mensaje
struct mensaje {
    long tipo;
    int operacion;
    int N;
    int resultado;
};

// Función para realizar la operación
int realizarOperacion(int operacion, int N) {
    int resultado = 0;
    
    if (operacion == 1) {
        for (int i = 0; i < N; i++) {
            resultado += 2 * i;
        }
    } else if (operacion == 2) {
        resultado = 1;
        for (int i = 1; i <= N; i++) {
            resultado *= i;
        }
    } else if (operacion == 3) {
        for (int i = 0; i < N; i++) {
            resultado += 1 << i;
        }
    } else if (operacion == 4) {
        resultado = 1;
        for (int i = 1; i <= N; i += 2) {
            resultado *= i;
        }
    }
    return resultado;
}

int main() {
	srand(time(NULL));
    // Crear o abrir la cola de mensajes
    key_t clave = ftok("/tmp", 'A');
    int cola_id = msgget(clave, IPC_CREAT | 0666);

    // Crear 10 procesos clientes
     for (int i = 0; i < 10; i++) {
		int operacion = (rand() % 4) + 1; //simulo operacion random
        int N = rand() % 100;//numero random de 0 a 100
        
    for (int i = 0; i < 100; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Proceso hijo (cliente)
            struct mensaje msg;
            msg.tipo = 1;
            msg.operacion = operacion;
            msg.N = N;

            // Realizar la operación
            msg.resultado = realizarOperacion(operacion, N);

            // Enviar el resultado al servidor
            msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);
            exit(0);
        }
        
    }
   }

    // Proceso padre (servidor)
    for (int i = 0; i < 1000; i++) {//1000 iteraciones (100 iteraciones de 10 hijos)
        struct mensaje msg;
        msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), 1, 0);

        printf("Operación: %d, N: %d, Resultado: %d\n", msg.operacion, msg.N, msg.resultado);
    }

    // Eliminar la cola de mensajes al finalizar
    msgctl(cola_id, IPC_RMID, NULL);

    return 0;
}

