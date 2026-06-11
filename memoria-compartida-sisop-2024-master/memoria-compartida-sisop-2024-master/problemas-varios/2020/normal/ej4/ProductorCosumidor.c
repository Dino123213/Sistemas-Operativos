#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define ROJO "\033[1;31m"              // Rojo
#define VERDE "\033[1;32m"             // Verde
#define AMARILLO "\033[1;33m"          // Amarillo
#define AZUL "\033[1;34m"              // Azul
#define MAGENTA "\033[1;35m"           // Magenta
#define CIAN "\033[1;36m"              // Cian
#define BLANCO "\033[1;37m"            // Blanco brillante
#define ROJO_BRILLANTE "\033[1;91m"    // Rojo brillante
#define VERDE_BRILLANTE "\033[1;92m"   // Verde brillante
#define AMARILLO_BRILLATE "\033[1;93m" // Amarillo brillante

#define bufferSize 30
#define cantLoop 400

int shmid;
key_t key;

typedef struct
{
    sem_t semMutex;
    sem_t semLleno;
    sem_t semVacio;
    int init;
    int fin;
    int buffer[bufferSize];
} info_t;

info_t *datos;

void consumidor(int id)
{
    int pointerFin = 0;
    int product = 0;

    while (1)
    {
        sem_wait(&datos->semLleno);
        sem_wait(&datos->semMutex);
        pointerFin = datos->fin;
        product = datos->buffer[pointerFin];

        printf("%sConsumidor %i, consume el producto %i en la posicion %i\n", ROJO, id, product, pointerFin);

        datos->buffer[pointerFin] = 0;
        datos->fin = (pointerFin + 1) % bufferSize;

        sem_post(&datos->semMutex);
        sem_post(&datos->semVacio);
    }
}

void productor(int id)
{
    int pointerIni = 0;
    int product = 0;

    while (1)
    {
        // Genero un numero entre 0 y 100
        product = (rand() % 101);

        sem_wait(&datos->semVacio);
        sem_wait(&datos->semMutex);

        pointerIni = datos->init;
        printf("%sProductor %i, crea un producto %i,en posicion %i \n", CIAN, id, product, pointerIni);
        datos->buffer[pointerIni] = product;
        datos->init = (pointerIni + 1) % bufferSize;

        sem_post(&datos->semMutex);
        sem_post(&datos->semLleno);
    }
}

// Para acceder a los datos
// puntero struct ->
// struct .
int main()
{
    key = ftok("1001110111", 64);
    shmid = shmget(key, sizeof(info_t), 0666 | IPC_CREAT);

    datos = (info_t *)shmat(shmid, NULL, 0);
    sem_init(&(datos->semMutex), 1, 1);
    sem_init(&(datos->semLleno), 1, 0);
    sem_init(&(datos->semVacio), 1, bufferSize);
    datos->init = 0;
    datos->fin = 0;

    for (int i = 1; i <= 3; i++)
    {
        pid_t productor_p = fork();
        if (productor_p == 0)
        {
            productor(i);
            exit(0);
        }
    }
    for (int i = 1; i <= 1; i++)
    {
        pid_t consumidor_p = fork();
        if (consumidor_p == 0)
        {
            consumidor(i);
            exit(0);
        }
    }

    wait(NULL);
    wait(NULL);
    sem_destroy(&datos->semMutex); // Destruir el semáforo
    sem_destroy(&datos->semLleno); // Destruir el semáforo
    sem_destroy(&datos->semVacio); // Destruir el semáforo
    shmctl(shmid, IPC_RMID, NULL); // Eliminar el segmento de memoria compartida
    return 0;
}