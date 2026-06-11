#include "constantes.h"

int main() {
    pid_t pid_desp;
    pid_t pid_empleado_hamb;
    pid_t pid_empleado_veg;
    pid_t pid_empleado_papas[CANT_EMPLEADOS_PAPAS];
    pid_t pid_cliente[CANT_CLIENTES];

    int msg_id = msgget(KEY, IPC_CREAT | 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    char *argsD[] = { "./ejecutables/despachador", NULL };
    pid_desp = fork();
    if (pid_desp == 0) {
        execv("./ejecutables/despachador", argsD);
        perror("execv falló");
        exit(1);
    } else if (pid_desp < 0) {
        perror("Error en la creación del proceso despachador.");
        exit(1);
    }

    char *argsH[] = { "./ejecutables/hamburguesa-simple", NULL };
    pid_empleado_hamb = fork();
    if (pid_empleado_hamb == 0) {
        execv("./ejecutables/hamburguesa-simple", argsH);
        perror("execv falló");
        exit(1);
    } else if (pid_empleado_hamb < 0) {
        perror("Error en la creación del proceso del empleado de hamburguesas.");
        exit(1);
    }

    char *argsV[] = { "./ejecutables/menu-vegano", NULL };
    pid_empleado_veg = fork();
    if (pid_empleado_veg == 0) {
        execv("./ejecutables/menu-vegano", argsV);
        perror("execv falló");
        exit(1);
    } else if (pid_empleado_veg < 0) {
        perror("Error en la creación del proceso del empleado del menú vegano.");
        exit(1);
    }

    char *argsP[] = { "./ejecutables/papas-fritas", NULL };
    for(int i = 0; i < CANT_EMPLEADOS_PAPAS; i++){
        pid_empleado_papas[i] = fork();
        if (pid_empleado_papas[i] == 0) {
            execv("./ejecutables/papas-fritas", argsP);
            perror("execv falló");
            exit(1);
        } else if (pid_empleado_papas[i] < 0) {
            perror("Error en la creación del proceso del empleado de papas fritas.");
            exit(1);
        }
    }

    for (int i = 0; i < CANT_CLIENTES; i++) {        
        srand(time(NULL) + getpid() + i);

        int tipo_cliente = (rand() % 4 == 0) ? CLIENTE_VIP : CLIENTE_NORMAL; 

        pid_cliente[i] = fork();
        if (pid_cliente[i] == 0) {

            char cliente_id[10];
            char tipo_cliente_str[10];

            sprintf(cliente_id, "%d", i + 1);
            sprintf(tipo_cliente_str, "%d", tipo_cliente);

            char *argsC[] = {
                "./ejecutables/cliente",    
                cliente_id,      
                tipo_cliente_str,     
                NULL             
            };

            execv("./ejecutables/cliente", argsC);

            perror("execv falló");
            exit(1);
        } else if (pid_cliente[i] < 0) {
            perror("Error en la creación del proceso de algún cliente.");
            exit(1);
        }
    }

    for(int i = 0; i < CANT_CLIENTES; i++) {
        waitpid(pid_cliente[i], NULL, 0);
    }

    kill(pid_desp, SIGTERM);
    kill(pid_empleado_hamb, SIGTERM);
    kill(pid_empleado_veg, SIGTERM);
    kill(pid_empleado_papas[0], SIGTERM);
    kill(pid_empleado_papas[1], SIGTERM);

    printf(GREEN "\nLa casa de comidas cerró.\n" RESET);

    msgctl(msg_id, IPC_RMID, NULL); 

    return 0;
}
