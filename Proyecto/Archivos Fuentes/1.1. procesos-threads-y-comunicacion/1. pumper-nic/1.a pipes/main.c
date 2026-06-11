#include "constantes.h" 

int pipe_vip_desp[2], pipe_normal_desp[2];
int pipe_desp_hamb[2], pipe_desp_papas[2], pipe_desp_veg[2];
int pipe_hamb_cli[2], pipe_papas_cli[2], pipe_veg_cli[2];

void cerrarPipesDespachador();
void cerrarPipesEmpleadoHamburguesa();
void cerrarPipesEmpleadoPapasFritas();
void cerrarPipesEmpleadoMenuVegano();
void cerrarPipesClientes();

void cliente_vip() {
    cerrarPipesClientes();
    close(pipe_normal_desp[0]);
    close(pipe_normal_desp[1]);
    close(pipe_vip_desp[0]);

    for (int i = 0; i < CANT_ITERACIONES; i++) {
        srand(time(NULL) + getpid() + i);
        int cola_llena = rand() % 20;

        if(cola_llena == 1) {
            printf(COLOR_COLA_LLENA "La cola está llena y un cliente vip se va.\n" COLOR_RESET);
            continue; 
        }

        int pedido = HAMBURGUESA + rand() % 3;
        int pedido_recibido;

        printf(COLOR_CLIENTE_VIP "Cliente VIP #%d quiere realizar el pedido de un %i.\n" COLOR_RESET, getpid(), pedido);
        write(pipe_vip_desp[1], &pedido, TAM); 

        switch (pedido) {
            case HAMBURGUESA :
                read(pipe_hamb_cli[0], &pedido_recibido, TAM);
                printf(COLOR_CLIENTE_VIP "Cliente VIP #%d recibe una hamburguesa.\n" COLOR_RESET, getpid());
                break;
            case PAPAS_FRITAS :
                read(pipe_papas_cli[0], &pedido_recibido, TAM);
                printf(COLOR_CLIENTE_VIP "Cliente VIP #%d recibe unas papas fritas.\n" COLOR_RESET, getpid());
                break;
            case MENU_VEGANO :
                read(pipe_veg_cli[0], &pedido_recibido, TAM);
                printf(COLOR_CLIENTE_VIP "Cliente VIP #%d recibe un menú vegano.\n" COLOR_RESET, getpid());
                break;
            default:
                break;
        }
    }

    close(pipe_vip_desp[1]);
    close(pipe_hamb_cli[0]);
    close(pipe_papas_cli[0]);
    close(pipe_veg_cli[0]);

    exit(0);
}

void cliente_normal() {
    cerrarPipesClientes();
    close(pipe_vip_desp[0]);
    close(pipe_vip_desp[1]);
    close(pipe_normal_desp[0]);

    for (int i = 0; i < CANT_ITERACIONES; i++) {
        srand(time(NULL) + getpid() + i);
        int cola_llena = rand() % 20;

        if(cola_llena == 1) {
            printf(COLOR_COLA_LLENA "La cola está llena y un cliente normal se va.\n" COLOR_RESET);
            continue; 
        }

        int pedido = HAMBURGUESA + rand() % 3;
        int pedido_recibido;

        printf(COLOR_CLIENTE_NORMAL "Cliente Normal #%d quiere realizar el pedido de un %i.\n" COLOR_RESET, getpid(), pedido);
        write(pipe_normal_desp[1], &pedido, TAM); 

        switch (pedido) {
            case HAMBURGUESA :
                read(pipe_hamb_cli[0], &pedido_recibido, TAM);
                printf(COLOR_CLIENTE_NORMAL "Cliente Normal #%d recibe una hamburguesa.\n" COLOR_RESET, getpid());
                break;
            case PAPAS_FRITAS :
                read(pipe_papas_cli[0], &pedido_recibido, TAM);
                printf(COLOR_CLIENTE_NORMAL "Cliente Normal #%d recibe unas papas fritas.\n" COLOR_RESET, getpid());
                break;
            case MENU_VEGANO :
                read(pipe_veg_cli[0], &pedido_recibido, TAM);
                printf(COLOR_CLIENTE_NORMAL "Cliente Normal #%d recibe un menú vegano.\n" COLOR_RESET, getpid());
                break;
            default:
                break;
        }
    }

    close(pipe_normal_desp[1]);
    close(pipe_hamb_cli[0]);
    close(pipe_papas_cli[0]);
    close(pipe_veg_cli[0]);

    exit(0);
}

void despachador() {
    cerrarPipesDespachador();
    fcntl(pipe_vip_desp[0], F_SETFL, O_NONBLOCK); 

    int menu_pedido;

    while (1) {
        if(read(pipe_vip_desp[0], &menu_pedido, TAM) > 0) {
            printf(COLOR_DESPACHADOR "Despachador atiende un pedido VIP.\n" COLOR_RESET);
        }
        else {
            read(pipe_normal_desp[0], &menu_pedido, TAM);
            printf(COLOR_DESPACHADOR "Despachador atiende un pedido normal.\n" COLOR_RESET);
        }
        
        switch (menu_pedido) {
            case HAMBURGUESA:
                printf(COLOR_HAMBURGUESA "Despachador manda a cocinar una hamburguesa.\n" COLOR_RESET);
                write(pipe_desp_hamb[1], &menu_pedido, TAM); 
                break;
            case PAPAS_FRITAS: 
                printf(COLOR_PAPAS "Despachador manda a cocinar unas papas fritas.\n" COLOR_RESET);
                write(pipe_desp_papas[1], &menu_pedido, TAM); 
                break;    
            case MENU_VEGANO:
                printf(COLOR_MENU_VEGANO "Despachador manda a cocinar un menú vegano.\n" COLOR_RESET);
                write(pipe_desp_veg[1], &menu_pedido, TAM);
                break;
            default:
                printf("Ocurrió algún error con el pedido.\n");
                break;
        }
    }

    close(pipe_vip_desp[0]);
    close(pipe_normal_desp[0]);
    close(pipe_desp_hamb[1]);
    close(pipe_desp_papas[1]);
    close(pipe_desp_veg[1]);

    exit(0);
}

void hamburguesa_simple() {
    cerrarPipesEmpleadoHamburguesa();
    
    int pedido;
    while(1) {
        read(pipe_desp_hamb[0], &pedido, TAM);
        printf(COLOR_HAMBURGUESA "Empleado prepara una hamburguesa.\n" COLOR_RESET);
        sleep(TIME);
        write(pipe_hamb_cli[1], &pedido, TAM);
    }

    close(pipe_desp_hamb[0]);
    close(pipe_hamb_cli[1]);

    exit(0);
}

void menu_vegano() {
    cerrarPipesEmpleadoMenuVegano();
    
    int pedido;
    while(1) {
        read(pipe_desp_veg[0], &pedido, TAM);
        printf(COLOR_MENU_VEGANO "Empleado prepara un menú vegano.\n" COLOR_RESET);
        sleep(TIME);
        write(pipe_veg_cli[1], &pedido, TAM);
    }

    close(pipe_desp_veg[0]);
    close(pipe_veg_cli[1]);

    exit(0);
}

void papas_fritas() {
    cerrarPipesEmpleadoPapasFritas();
    
    int pedido;
    while(1) {
        read(pipe_desp_papas[0], &pedido, TAM);
        printf(COLOR_PAPAS "Empleado prepara unas papas fritas.\n" COLOR_RESET);
        sleep(TIME);
        write(pipe_papas_cli[1], &pedido, TAM);
    }
    
    close(pipe_desp_papas[0]);
    close(pipe_papas_cli[1]);
    
    exit(0);
}

int main() {
    pid_t pid_desp;
    pid_t pid_empleado_hamb;
    pid_t pid_empleado_veg;
    pid_t pid_empleado_papas[CANT_EMPLEADOS_PAPAS];
    pid_t pid_cliente[CANT_CLIENTES];

    if (pipe(pipe_vip_desp) == -1 || pipe(pipe_normal_desp) == -1 || pipe(pipe_desp_hamb) == -1 || 
        pipe(pipe_desp_papas) == -1 || pipe(pipe_desp_veg) == -1 || pipe(pipe_hamb_cli) == -1 || 
        pipe(pipe_papas_cli) == -1 || pipe(pipe_veg_cli) == -1) {
        fprintf(stderr, "Creación de pipes fallida.");
        return 1;
    }

    pid_desp = fork();
    if (pid_desp == 0) {
        despachador(); 
        exit(0);
    } else if (pid_desp < 0) {
        perror("Error en la creación del proceso despachador.");
        exit(1);
    }

    pid_empleado_hamb = fork();
    if (pid_empleado_hamb == 0) {
        hamburguesa_simple(); 
        exit(0);
    } else if (pid_empleado_hamb < 0) {
        perror("Error en la creación del proceso del empleado de hamburguesas.");
        exit(1);
    }

    pid_empleado_veg = fork();
    if (pid_empleado_veg == 0) {
        menu_vegano(); 
        exit(0);
    } else if (pid_empleado_veg < 0) {
        perror("Error en la creación del proceso del empleado del menú vegano.");
        exit(1);
    }

    for (int i = 0; i < CANT_EMPLEADOS_PAPAS; i++) {
        pid_empleado_papas[i] = fork();
        if (pid_empleado_papas[i] == 0) {
            papas_fritas(); 
            exit(0);
        } else if (pid_empleado_papas[i] < 0) {
            perror("Error en la creación del proceso del empleado de papas fritas.");
            exit(1);
        }
    }
    
    for (int i = 0; i < CANT_CLIENTES; i++) {
        pid_cliente[i] = fork();
        if (pid_cliente[i] == 0) {
            srand(time(NULL) + getpid());
            int vip = (rand() % 4 == 0) ? CLIENTE_VIP : CLIENTE_NORMAL;
            if(vip == CLIENTE_VIP) {
                cliente_vip();
                exit(0);
            }
            else {
                cliente_normal();
                exit(0);
            }
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

    printf("\nLa casa de comidas cerró.\n");

    return 0;
}

void cerrarPipesDespachador() {
    close(pipe_vip_desp[1]);
    close(pipe_normal_desp[1]);
    close(pipe_desp_hamb[0]);
    close(pipe_desp_papas[0]);
    close(pipe_desp_veg[0]);

    close(pipe_hamb_cli[0]); 
    close(pipe_hamb_cli[1]); 
    close(pipe_papas_cli[0]); 
    close(pipe_papas_cli[1]);
    close(pipe_veg_cli[0]);
    close(pipe_veg_cli[1]);
}

void cerrarPipesEmpleadoHamburguesa() {
    close(pipe_desp_hamb[1]);
    close(pipe_hamb_cli[0]);

    close(pipe_vip_desp[0]);
    close(pipe_vip_desp[1]);
    close(pipe_normal_desp[0]);
    close(pipe_normal_desp[1]);
    close(pipe_desp_papas[0]);
    close(pipe_desp_papas[1]);
    close(pipe_desp_veg[0]);
    close(pipe_desp_veg[1]); 
    close(pipe_papas_cli[0]); 
    close(pipe_papas_cli[1]);
    close(pipe_veg_cli[0]);
    close(pipe_veg_cli[1]);
}

void cerrarPipesEmpleadoPapasFritas() {
    close(pipe_desp_papas[1]);
    close(pipe_papas_cli[0]);

    close(pipe_vip_desp[0]);
    close(pipe_vip_desp[1]);
    close(pipe_normal_desp[0]);
    close(pipe_normal_desp[1]);
    close(pipe_desp_hamb[0]);
    close(pipe_desp_hamb[1]);
    close(pipe_desp_veg[0]);
    close(pipe_desp_veg[1]);
    close(pipe_hamb_cli[0]); 
    close(pipe_hamb_cli[1]); 
    close(pipe_veg_cli[0]);
    close(pipe_veg_cli[1]);
}

void cerrarPipesEmpleadoMenuVegano() {
    close(pipe_desp_veg[1]);
    close(pipe_veg_cli[0]);

    close(pipe_vip_desp[0]);
    close(pipe_vip_desp[1]);
    close(pipe_normal_desp[0]);
    close(pipe_normal_desp[1]);
    close(pipe_desp_hamb[0]);
    close(pipe_desp_hamb[1]);
    close(pipe_desp_papas[0]);
    close(pipe_desp_papas[1]);
    close(pipe_hamb_cli[0]); 
    close(pipe_hamb_cli[1]); 
    close(pipe_papas_cli[0]); 
    close(pipe_papas_cli[1]);
}

void cerrarPipesClientes() {
    close(pipe_hamb_cli[1]);
    close(pipe_papas_cli[1]);
    close(pipe_veg_cli[1]);

    close(pipe_desp_hamb[0]);
    close(pipe_desp_hamb[1]);
    close(pipe_desp_papas[0]);
    close(pipe_desp_papas[1]);
    close(pipe_desp_veg[0]);
    close(pipe_desp_veg[1]);
}