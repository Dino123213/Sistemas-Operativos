#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <semaphore.h>

/* Estructura con los semáforos compartidos entre procesos */
struct semaforos {
    sem_t wsem;   // Semáforo para controlar acceso de escritores (binario, inicial=1)
    sem_t rmutex; // Mutex para proteger acceso al contador de lectores (binario, inicial=1)
    sem_t rsem;   // Semáforo contador de lectores activos (inicial=0)
};

/* Constantes compartidas */
#define ESCRITORES 3
#define LECTORES 3
#define ITER 10
#define SHM_KEY_FILE "LEArchivo.c"
#define SHM_KEY_ID 76

#endif