#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>

#define KEY ((key_t) (1243)) // versión diapositivas
#define SEG_SIZE sizeof(struct memoria_compartida)

#define NUM_PRODUCTORES 3
#define MAX 10
#define N 10 // random() % 100

#define COLOR_ROJO "\x1b[31m"
#define COLOR_VERDE "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_AZUL "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

struct memoria_compartida {
    sem_t huecos_vacio;
    sem_t elementos_lleno;
    sem_t n_iteraciones;
    sem_t mutex;
    int buffer[MAX];
    int pos_productor;
};

int generarDato(void) {
    return random() % 256;
}

void productor(int indice) {
    srand(getpid()); // importante genera una nueva semilla por cada proceso ya que ahora hay memoria compartida.

    struct memoria_compartida *datos;
    int dato;

    int id1 = shmget(KEY, SEG_SIZE, 0);
    if(id1 < 0) {
        printf("Error: fallo en shmget del productor\n");
        exit(1);
    }

    datos = (struct memoria_compartida*)shmat(id1, 0, 0);
    if(datos <= (struct memoria_compartida *)(0)) {
        printf("Error: fallo en shmat del productor\n");
        exit(2);
    }

    printf(COLOR_AZUL "------ Productor %d comienza a producir ------\n" COLOR_RESET, indice);
    
    while (1) {
        if(sem_trywait(&(datos->n_iteraciones)) == 0) {
            sem_wait(&(datos->huecos_vacio));
            dato = generarDato();
            sem_wait(&(datos->mutex)); 
                printf(COLOR_VERDE "Productor %d: agregando en la posición %d el dato %d.\n" COLOR_RESET, indice, datos->pos_productor, dato);
                datos->buffer[datos->pos_productor] = dato;
                datos->pos_productor = ((datos->pos_productor) + 1) % MAX;
            sem_post(&(datos->mutex)); 
            sem_post(&(datos->elementos_lleno));
            sleep(1);
        } else {
            printf(COLOR_AZUL "Productor %d deja de producir.\n" COLOR_RESET, indice);
            shmdt(datos);
            printf(COLOR_AZUL "Productor %d se ha desconectado de la memoria compartida.\n" COLOR_RESET, indice);
            exit(0); // Terminar el proceso del productor
        }
    }
}

void consumidor() {
    struct memoria_compartida *datos;
    int pos_consumidor = 0;
    int dato;

    int id = shmget(KEY, SEG_SIZE, 0);
    if(id < 0) {
        printf("Error: fallo en shmget del consumidor\n");
        exit(1);
    }

    datos = (struct memoria_compartida*)shmat(id, 0, 0);
    if(datos <= (struct memoria_compartida *)(0)) {
        printf("Error: fallo en shmat del consumidor\n");
        exit(2);
    }
    
    for(int i = 0; i < N; i++) {
        sem_wait(&(datos->elementos_lleno));
        sem_wait(&(datos->mutex)); 
            dato = datos->buffer[pos_consumidor];
            printf(COLOR_AMARILLO "Consumidor: consumiendo de la posición %d el dato %d.\n" COLOR_RESET, pos_consumidor, dato);
        sem_post(&(datos->mutex)); 
        pos_consumidor = (pos_consumidor + 1) % MAX;
        sem_post(&(datos->huecos_vacio));
        sleep(1);
    }
    printf(COLOR_ROJO "Consumidor deja de consumir.\n" COLOR_RESET);
    if (shmdt(datos) == -1) {
        perror("Error: fallo en shmdt del consumidor");
    } else {
        printf(COLOR_AZUL "Consumidor se ha desconectado de la memoria compartida.\n" COLOR_RESET);
    }
}

int main() { 
    int id;
    pid_t consumidor_t;
    pid_t productores_t[NUM_PRODUCTORES];

    // Creación de segmentos de memoria compartida
    id = shmget(KEY, SEG_SIZE, IPC_CREAT | 0666); // Creo segmento de memoria
    if (id == -1) {
        printf("Error: fallo en shmget\n");
        exit(1);
    }
    struct memoria_compartida *datos = (struct memoria_compartida *)shmat(id, 0, 0);
    if(datos <= (struct memoria_compartida *)(0)) {
        printf(" Error: fallo en shmat\n");
        exit(1);
    }
    
    // Inicializaciones
    sem_init(&(datos->huecos_vacio), 1, MAX);
    sem_init(&(datos->elementos_lleno), 1, 0);
    sem_init(&(datos->n_iteraciones), 1, N);
    sem_init(&(datos->mutex), 1, 1);
    datos->pos_productor = 0;

    printf("---------- Se producirán %d datos ---------- \n\n", N);

    // Creación de procesos
    consumidor_t = fork();
    if(consumidor_t == 0) {
        shmdt(datos); // desvinculo desde antes por las dudas
        consumidor();
        exit(0);
    } else if (consumidor_t < 0) {
        perror("Error: creación proceso consumidor\n");
        exit(1);
    }

    for(int i = 0; i < NUM_PRODUCTORES; i++) {
        productores_t[i] = fork();
        if(productores_t[i] == 0) {
            productor(i + 1); // Pasar el índice del productor para identificarlo
            exit(0);
        } else if (productores_t[i] < 0) {
            perror("Error: creación proceso productor\n");
            exit(1);
        }
    }

    for(int i = 0; i < NUM_PRODUCTORES + 1; i++) {
        wait(NULL);
    }

    sem_destroy(&datos->huecos_vacio);
    sem_destroy(&datos->elementos_lleno);
    sem_destroy(&datos->n_iteraciones);
    sem_destroy(&datos->mutex);
    
    shmctl(id, IPC_RMID, NULL);

    return 0;
}