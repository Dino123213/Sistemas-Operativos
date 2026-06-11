#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H

#include <sys/types.h>  
#include <sys/ipc.h>    
#include <unistd.h>     
#include <stdio.h>      
#include <stdlib.h> 
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

#define INSTALACIONES 1
#define CANTC 2
#define MUTEX 3

#define CANT 5

// Tamaño de la estructura de mensaje
#define MSG_SIZE sizeof(struct mensaje) - sizeof(long)


// Estructura para almacenar los pedidos
struct mensaje {
    long tipo;
};
// Declarar la función generateKey
key_t generateKey();

// Implementación de la función generateKey
key_t generateKey() {
    int projectId = 14;
    char path[200];
    getcwd(path, sizeof(path));  // Obtener el directorio de trabajo actual
    key_t key = ftok(path, projectId);  // Generar clave única
    return key;
}

#endif 
