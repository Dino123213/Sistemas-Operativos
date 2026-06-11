#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) 
{
    char *a = argv[1];
    printf("El valor de a es: %s\n", a);
    sleep(10); // PARA PODER CAPTURAR EL ARBOL DE PROCESOS
}
