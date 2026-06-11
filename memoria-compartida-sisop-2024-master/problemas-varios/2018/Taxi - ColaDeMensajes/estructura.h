#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H

#include <sys/types.h>  // Para key_t
#include <sys/ipc.h>    // Para key_t y ftok()
#include <unistd.h>     // Para getcwd()
#include <stdio.h>      // Para printf()
#include <stdlib.h> 
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
// Definir constantes
#define CANT_PASAJEROS 12
#define NUM_ASIENTOS 4
#define MENSAJE_LLENO 1
#define MENSAJE_ASIENTOS 2
#define MENSAJE_LLEGADA 3

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"

// Tamaño de la estructura de mensaje
#define MSG_SIZE sizeof(struct mensaje) - sizeof(long)

// Estructura del mensaje
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

#endif // ESTRUCTURA_H
