#ifndef PC_H
#define PC_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define TAM_BUFFER 20

struct aCompartir {
    sem_t lleno;
    sem_t vacio;
    sem_t mutex;
    int buf[TAM_BUFFER];
    int in;
    int out;
};

// Definir la función como inline
 key_t generateKey() {
    int projectId = 14;
    char path[200];
    getcwd(path, 200 * sizeof(char));
    key_t key = ftok(path, projectId);
    return key;
}

#endif
