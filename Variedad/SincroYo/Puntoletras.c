#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sA, sB, sC, sD, sE;

void* A(void* arg) {
    while (1) {
        sem_wait(&sA);
        printf("A");
        fflush(stdout);
        sem_post(&sB);
    }
}
void* B(void* arg) {
    while (1) {
        sem_wait(&sB);
        printf("B");
        fflush(stdout);
        sem_post(&sC);
    }
}
void* C(void* arg) {
    while (1) {
        sem_wait(&sC);
        printf("C");
        fflush(stdout);
        sem_post(&sD);
    }
}
void* D(void* arg) {
    while (1) {
        sem_wait(&sD);
        printf("D");
        fflush(stdout);
        sem_post(&sE);
    }
}
void* E(void* arg) {
    while (1) {
        sem_wait(&sE);
        printf("E");
        fflush(stdout);
        sem_post(&sA);
    }
}

int main() {
    pthread_t tA, tB, tC, tD, tE;

    sem_init(&sA, 0, 1);
    sem_init(&sB, 0, 0);
    sem_init(&sC, 0, 0);
    sem_init(&sD, 0, 0);
    sem_init(&sE, 0, 0);

    pthread_create(&tA, NULL, A, NULL);
    pthread_create(&tB, NULL, B, NULL);
    pthread_create(&tC, NULL, C, NULL);
    pthread_create(&tD, NULL, D, NULL);
    pthread_create(&tE, NULL, E, NULL);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);
    pthread_join(tD, NULL);
    pthread_join(tE, NULL);
}