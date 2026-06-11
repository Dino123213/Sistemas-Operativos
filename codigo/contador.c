#include <stdio.h>
#include <unistd.h>

//Auxiliar para Ejercitación Extra inciso b

int main() {
    printf("Proceso contador iniciado (PID: %d)\n", getpid());
    printf("Contando de 1 a 10000:\n");
    for (int i = 1; i <= 10000; i++) {
        printf("%d ", i);
        //añadir salto de línea cada 20 números para mejor legibilidad
        if (i % 20 == 0) {
            printf("\n");
        }
        //pequeña pausa cada 1000 números para que sea más visible
        if (i % 1000 == 0) {
            usleep(100000); // 0.1 segundos
        }
    }
    printf("\nContador terminado.\n");
    
    return 0;
}