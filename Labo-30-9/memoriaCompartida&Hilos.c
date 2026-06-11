#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 10

struct memoria_compartida {
    int buffer[1]; // Tamaño del buffer 1, puede ser modificado
};

struct parametros {
    int id;
    char mensaje[50];
};

struct memoria_compartida data; // memoria compartida global entre hilos

void* tareaA(void* arg) {
    for (int i = 0; i < N; i++) {
        data.buffer[0] = i;
        printf("ProcesoA: produjo %d\n", i);
        
    }
    pthread_exit(NULL);
}


void* tareaB(void* arg) {
    for (int i = 0; i < N; i++) {
        int valor = data.buffer[0];
        printf("ProcesoB: consumió %d\n", valor);
        printf("Informacion extra: ID %d, Mensaje: %s\n", ((struct parametros*)arg)->id, ((struct parametros*)arg)->mensaje); //Casteo el argumento para poder utilizarlo accediendo a sus campos.
    }
    pthread_exit(NULL);
}


int main() {
    pthread_t hA, hB;

    // Crear hilos
    if (pthread_create(&hA, NULL, tareaA, NULL) != 0) { //&ha direccion del identificador del hilo, NULL configuracion por defecto, tareaA funcion que ejecutara el hilo, NULL argumento pasado a la funcion del hilo
        perror("Error creando hilo A"); 
        exit(1);
    } 
    struct parametros* p2 = malloc(sizeof(struct parametros));
    p2->id = 2;
    if (pthread_create(&hB, NULL, tareaB,p2) != 0) {//Pasaje por parametros, se necesita malloc para asegurar la no desaparicion de la variable
        perror("Error creando hilo B");
        exit(1);
    }

    // Esperar a que terminen los hilos
    pthread_join(hA, NULL);
    pthread_join(hB, NULL);
    free(p2); //Liberar memoria
    return 0;
}
