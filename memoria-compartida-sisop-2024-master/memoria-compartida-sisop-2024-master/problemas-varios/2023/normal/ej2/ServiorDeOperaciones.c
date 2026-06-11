#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>

#define NUM_CLIENTES 10
#define NUM_OPERACIONES 4
#define CICLOS 100

// Estructura del mensaje
typedef struct mensaje {
    long tipo; // Tipo de mensaje
    int cliente_id; // ID del cliente que envía la solicitud
    int operacion; // Código de operación (1 a 4)
    int iteraciones; // Número de iteraciones
    int resultado; // Resultado calculado (enviado por el proceso de cálculo)
} Mensaje;

// Funciones de las operaciones
int sumatoria_pares(int n) {
    int suma = 0;
    for (int i = 1; i <= n; i++) {
        suma += 2 * i;
    }
    return suma;
}

int factorial(int n) {
    int fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

int sumatoria_potencias(int n) {
    int suma = 0;
    for (int i = 0; i < n; i++) {
        suma += (1 << i); // 2^i usando bit-shift
    }
    return suma;
}

int productoria_impares(int n) {
    int prod = 1;
    for (int i = 1; i <= n; i++) {
        prod *= (2 * i - 1);
    }
    return prod;
}

// Procesos de cálculo
void proceso_calculo(int cola, int operacion) {
    Mensaje mensaje;
    while (1) {
        // Recibir mensaje correspondiente a la operación
        if (msgrcv(cola, &mensaje, sizeof(Mensaje) - sizeof(long), operacion, 0) == -1) {
            perror("Error al recibir mensaje en proceso de cálculo");
            exit(1);
        }
        printf("Realizando operacion %d, para cliente %d\n",mensaje.operacion,mensaje.cliente_id);
        fflush(stdout);
        
        sleep(3);
        // Realizar la operación correspondiente
        switch (operacion) {
            case 1:
                mensaje.resultado = sumatoria_pares(mensaje.iteraciones);
                break;
            case 2:
                mensaje.resultado = factorial(mensaje.iteraciones);
                break;
            case 3:
                mensaje.resultado = sumatoria_potencias(mensaje.iteraciones);
                break;
            case 4:
                mensaje.resultado = productoria_impares(mensaje.iteraciones);
                break;
        }
        printf("Operacion %d terminada para cliente %d \n",mensaje.operacion, mensaje.cliente_id);
        fflush(stdout);
        
        mensaje.tipo = mensaje.cliente_id+10; // Enviar al cliente
        if (msgsnd(cola, &mensaje, sizeof(Mensaje) - sizeof(long), 0) == -1) {
            perror("Error al enviar mensaje desde proceso de cálculo");
            exit(1);
        }
    }
}

// Proceso servidor
void servidor(int cola) {
    Mensaje mensaje;
    while (1) {
        // Recibir solicitud de cualquier cliente
        if (msgrcv(cola, &mensaje, sizeof(Mensaje) - sizeof(long), 1, 0) == -1) {
            perror("Error al recibir mensaje en el servidor");
            exit(1);
        }

        // Enviar la solicitud al proceso correspondiente
        mensaje.tipo = mensaje.operacion; // Tipo según operación
        if (msgsnd(cola, &mensaje, sizeof(Mensaje) - sizeof(long), 0) == -1) {
            perror("Error al enviar mensaje al proceso de cálculo");
            exit(1);
        }
    }
}

// Proceso cliente
void cliente(int cola, int id) {
    Mensaje mensaje;
    srand(getpid());
    for (int i = 0; i < CICLOS; i++) {
        mensaje.tipo = 1; // Tipo para el servidor
        mensaje.cliente_id = id;
        mensaje.operacion = rand() % NUM_OPERACIONES + 1; // Operación aleatoria (1 a 4)
        mensaje.iteraciones = rand() % 10 + 1; // Iteraciones aleatorias (1 a 10)

        printf("Cliente %d solicita operación %d con %d iteraciones\n",
               id, mensaje.operacion, mensaje.iteraciones);
               fflush(stdout);

        // Enviar solicitud al servidor
        if (msgsnd(cola, &mensaje, sizeof(Mensaje) - sizeof(long), 0) == -1) {
            perror("Error al enviar mensaje desde cliente");
            exit(1);
        }

        // Esperar respuesta del servidor
        if (msgrcv(cola, &mensaje, sizeof(Mensaje) - sizeof(long), id+10, 0) == -1) {
            perror("Error al recibir mensaje en cliente");
            exit(1);
        }

        // Mostrar resultado
        printf("Cliente %d recibió resultado: %d\n", id, mensaje.resultado);
        fflush(stdout);
        sleep(2);
    }
}

int main() {
    int cola;
    pid_t pid;

    // Crear cola de mensajes
    if ((cola = msgget(IPC_PRIVATE, IPC_CREAT | 0666)) == -1) {
        perror("Error al crear la cola de mensajes");
        exit(1);
    }
    msgctl(cola, IPC_RMID, NULL);
    if ((cola = msgget(IPC_PRIVATE, IPC_CREAT | 0666)) == -1) {
        perror("Error al crear la cola de mensajes");
        exit(1);
    }
    // Crear procesos de cálculo
    for (int i = 1; i <= NUM_OPERACIONES; i++) {
        if ((pid = fork()) == 0) {
            proceso_calculo(cola, i);
            exit(0);
        }
    }

    // Crear proceso servidor
    if ((pid = fork()) == 0) {
        servidor(cola);
        exit(0);
    }

    // Crear procesos clientes
    for (int i = 0; i < NUM_CLIENTES; i++) {
        if ((pid = fork()) == 0) {
            cliente(cola, i + 1);
            exit(0);
        }
    }

    // Esperar a que terminen los clientes
    for (int i = 0; i < NUM_CLIENTES; i++) {
        wait(NULL);
    }

    // Eliminar la cola de mensajes
    if (msgctl(cola, IPC_RMID, NULL) == -1) {
        perror("Error al eliminar la cola de mensajes");
        exit(1);
    }

    return 0;
}
