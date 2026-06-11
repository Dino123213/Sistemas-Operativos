#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define MAX 256
#define TAM sizeof(int)

#define CANT_CLIENTES 10
#define CANT_EMPLEADOS_PAPAS 2

#define CLIENTE_VIP 0
#define CLIENTE_NORMAL 1

#define HAMBURGUESA 3
#define MENU_VEGANO 4
#define PAPAS_FRITAS 5

#define CANT_ITERACIONES 5
#define TIME 1

#define COLOR_RESET "\033[0m"
#define COLOR_CLIENTE_VIP "\033[0;37m"    
#define COLOR_CLIENTE_NORMAL "\033[1;33m"   
#define COLOR_DESPACHADOR "\033[1;32m"    
#define COLOR_HAMBURGUESA "\033[1;34m"     
#define COLOR_MENU_VEGANO "\033[1;35m"     
#define COLOR_PAPAS "\033[1;31m"    
#define COLOR_COLA_LLENA "\x1b[31m"      

#endif