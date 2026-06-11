#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct arreglo_Compartido {
    int tamaño;
    int arr[]; // Flexible array member - el tamaño se define dinámicamente
};

int main(int argc, char **argv) {
    
    //Tengo que chequear si solo entre un parámetro (el valor N)
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }
    //Tengo que leer el parametro N que me pasaron
    int n = atoi(argv[1]);
    
    //Verificar que N sea válido y par
    if (n <= 0) {
        fprintf(stderr, "N debe ser un número positivo\n");
        return 1;
    }
    if (n % 2 != 0) {
        fprintf(stderr, "N debe ser un número par\n");
        return 1;
    }
    
    //El arreglo que inicializo con valores random no es compartido
    int arreglo[n];
    //Inicializo el arreglo con valores random
    for (int i = 0; i < n; i++) {
        arreglo[i] = rand() % 100;
    }

    printf("El arreglo es:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arreglo[i]);
    }
    printf("\n");

    //Vamos a crear la seccion de memoria compartida para mi struct donde iran guardadas las sumas
    key_t key = ftok("0306.c", 65); // genera una clave
    int shmid = shmget(key, sizeof(struct arreglo_Compartido) + n/2 * sizeof(int), 0666|IPC_CREAT); // crea el segmento
    struct arreglo_Compartido *datos = (struct arreglo_Compartido*) shmat(shmid, NULL, 0); // puntero a la estructura
    if (datos == (void*) -1) {
        perror("Error al adjuntar la memoria compartida");
        return 1;
    }
    datos->tamaño = n/2;

    //Tengo que crear N/2 procesos y cada proceso va a sumar dos valores del arreglo
    int num_procesos = n/2;
    for (int i = 0; i < num_procesos; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error en fork");
            return 1;
        }
        if (pid == 0) {
            //Cada hijo va a sumar una parte del arreglo
            int pos1 = i * 2;
            int pos2 = i * 2 + 1;
            int sum = arreglo[pos1] + arreglo[pos2];
            printf("Proceso hijo %d: La suma de %d + %d = %d\n", getpid(), arreglo[pos1], arreglo[pos2], sum);
            printf("Correspondientes a las posiciones %d y %d del arreglo\n", pos1, pos2);

            //Sumo y guardo en la memoria compartida
            datos->arr[i] = sum;

            exit(0); //termino el proceso hijo
        }
    }
        
    //Espero a que todos los hijos terminen
    for (int i = 0; i < num_procesos; i++) {
        int status;
        pid_t child_pid = wait(&status);
        if (child_pid == -1) {
            perror("Error en wait");
        } else {
            printf("Proceso hijo %d terminó\n", child_pid);
        }
    }
    
    printf("Todos los procesos hijos han terminado\n");
    
    printf("El arreglo de sumas es:\n");
    for (int i = 0; i < datos->tamaño; i++) {
        printf("%d ", datos->arr[i]);
    }
    printf("\n");

    shmdt(datos); // me desadjunto de la memoria compartida
    shmctl(shmid, IPC_RMID, NULL); // destruyo el segmento de memoria compartida
    return 0;
}