#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>

#define CANT_CLIENTES 20
#define CANT_EMPLEADOS_PAPAS 2
#define CANT_ITERACIONES 10

#define CLIENTE_VIP 1
#define CLIENTE_NORMAL 2
#define VIP_O_NORMAL -2

#define HAMBURGUESA 3
#define MENU_VEGANO 4
#define PAPAS_FRITAS 5

#define KEY ((key_t) (7777))
#define LONGITUD sizeof(struct msg_buffer) - sizeof(long)

#define TIME_U 100000
#define DIFERENCIADOR 10

#define RESET   "\033[0m" 
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define ORANGE  "\x1b[38;5;208m"
#define DARK_YELLOW "\033[0;33m"

struct msg_buffer {
    long tipo;
    int id_cliente;
    int menu;
};

#endif