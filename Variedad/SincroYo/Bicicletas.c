#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t so1, so2, so3, s_mont;

void* OP1(void* arg) {
    while (1) {
        sem_wait(&so1);
        //Monta rueda s
        fflush(stdout);
        sem_post(&so1);
    }
}

void* OP2(void* arg) {
    while (1) {
        sem_wait(&so2);
        //Monta cuadro
        fflush(stdout);
        sem_post(&so2);
    }
}

void* OP3(void* arg) {
    while (1) {
        sem_wait(&so3);
        //Monta manillar
        fflush(stdout);
        sem_post(&so3);
    }
}

void* Montador(void* arg) {
    while (1) {
        sem_wait(&s_mont);
        sem_wait(&so1);
        sem_wait(&so2);
        sem_wait(&so3);
        //Montador
        fflush(stdout);
        sem_post(&s_mont);
    }
}

int main() {
    pthread_t tA, tB, tC, tD;

    sem_init(&so1, 0, 1);
    sem_init(&so2, 0, 0);
    sem_init(&so3, 0, 0);
    sem_init(&s_mont, 0, 0);

    pthread_create(&tA, NULL, OP1, NULL);
    pthread_create(&tB, NULL, OP2, NULL);
    pthread_create(&tC, NULL, OP3, NULL);
    pthread_create(&tD, NULL, Montador, NULL);
  

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);
    pthread_join(tD, NULL);
}