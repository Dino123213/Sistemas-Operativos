#include <sys/ipc.h>   // para ftok(), claves
#include <sys/shm.h>   // para shmget(), shmat(), shmdt(), shmctl()
#include <stdio.h>     // printf, perror
#include <stdlib.h>    // exit
#include <unistd.h>    // fork, etc.
#include <sys/wait.h>  // wait
#include <sys/types.h> // pid_t

struct info {
    int numero;
};

int main() {
    printf("Llegue aca");
    //key_t llave = ftok("0303.c", 65); // genera una clave
    int shmid = shmget(1234, sizeof(struct info), 0666|IPC_CREAT); // crea el segmento
    struct info* ptr = (struct info*) shmat(shmid, NULL, 0); // puntero a la estructura

    ptr->numero = 42; // escribe en la memoria compartida

    pid_t pid = fork(); // crea un proceso hijo

    if(pid < 0) {
        perror("Fork fallido");
        exit(1);
    }
    if(pid == 0) {
        shmdt(ptr); // se desliga del segmento
        struct info* ptr2 = (struct info*) shmat(shmid, NULL, 0); // puntero a la estructura
        printf("Leyendo número del padre: %i\n", ptr2->numero); // lee de la memoria compartida
        shmdt(ptr2); // se desliga del segmento
        exit(0);
    }  else {
        wait(NULL); // espera al hijo
        shmdt(ptr); // se desliga del segmento
        shmctl(shmid, IPC_RMID, NULL); // destruye el segmento
    }

    if(pid == 0) {
        printf("Leyendo número del padre: %d\n", ptr->numero); // lee de la memoria compartida
        shmdt(ptr); // se desliga del segmento
        exit(0);
    }  else {
        wait(NULL); // espera al hijo
        shmdt(ptr); // se desliga del segmento
        printf("%p", ptr);
        shmctl(shmid, IPC_RMID, NULL); // destruye el segmento
    }

}