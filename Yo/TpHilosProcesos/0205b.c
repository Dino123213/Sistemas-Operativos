#include <pthread.h>   // Para manejar hilos POSIX
#include <stdio.h>     // Para printf
#include <stdlib.h>    // Para exit, malloc, etc.

#define N 5            // Tamaño del arreglo

int arr[N] = {1, 2, 3, 4, 5};  // Arreglo de ejemplo
int suma_total = 0;            // Variable global compartida
pthread_mutex_t lock;          // Mutex para proteger suma_total

void* sumar(void* arg) {
    int indice = *(int*)arg;   // Convertimos el argumento a int (índice del arreglo)

    pthread_mutex_lock(&lock); // Bloqueamos el acceso a suma_total (evita condiciones de carrera)
    suma_total += arr[indice]; // Sumamos el elemento correspondiente
    pthread_mutex_unlock(&lock); // Desbloqueamos para que otro hilo pueda usar suma_total

    return NULL;               // Los hilos devuelven NULL en este caso
}

int main() {
    pthread_t hilos[N];        // Array para guardar los identificadores de hilos
    int indices[N];            // Array con los índices a pasar a cada hilo

    pthread_mutex_init(&lock, NULL); // Inicializamos el mutex

    // Crear N hilos, uno por cada elemento del arreglo
    for (int i = 0; i < N; i++) {
        indices[i] = i;   // Guardamos el índice
        pthread_create(&hilos[i], NULL, sumar, &indices[i]); 
        // &hilos[i] = dónde guardar el ID del hilo
        // NULL = atributos por defecto
        // sumar = función que ejecutará el hilo
        // &indices[i] = argumento que recibe la función del hilo
    }

    // Esperar a que terminen todos los hilos
    for (int i = 0; i < N; i++) {
        pthread_join(hilos[i], NULL);
        // Bloquea al hilo principal hasta que hilos[i] termine
    }

    printf("Suma total = %d\n", suma_total);

    pthread_mutex_destroy(&lock); // Liberamos el mutex
    return 0;
}
