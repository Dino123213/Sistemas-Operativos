#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int c = 5;
    pid_t pid = fork(); 

    if (pid == 0) { 
        c += 5;

        // FIX PARA QUE FUNCIONE
        char arg_c[12]; // Tamaño suficiente para almacenar un int
        sprintf(arg_c, "%d", c);  // Formatear el entero como cadena
        // FIN FIX

        execl("./codigo2", "codigo2", arg_c, NULL); 
        perror("execl");
        exit(1);
    } else { 
        pid = fork(); 
        c += 10;
        if (pid) {
            c += 10;
        }
    }

    fork();
    printf("%d\n", c); 
    sleep(10); // PARA PODER CAPTURAR EL ARBOL DE PROCESOS
    return 0;
}
