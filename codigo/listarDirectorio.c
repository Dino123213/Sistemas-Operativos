#include <stdio.h>
#include <unistd.h>

//Auxiliar para Ejercitación Extra inciso b

int main() {
    printf("Proceso listarDirectorio iniciado (PID: %d)\n", getpid());
    execlp("ls", "ls", "/etc", NULL);
    return 0;
}