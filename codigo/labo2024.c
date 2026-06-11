#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_JABALINA 5
#define NUM_MARTILLO 5
#define NUM_CORREDORES 10

sem_t semMartilloJabalina;
sem_t cantCorredor;
pthread_mutex_t mutexCorredores;

void *lanzador_jabalina() {
    while (1) {
        sem_wait(&semMartilloJabalina); //reclamo el semáforo
        printf("\033[1;34mEntra un lanzador de jabalina\033[0m\n");

        sleep(1); //simula el tiempo que tarda en lanzar

        sem_post(&semMartilloJabalina); //libero el semáforo
        printf("\033[1;34mSe va un lanzador de jabalina\033[0m\n");
    }
    pthread_exit(NULL);
}

void *lanzador_martillo() {
    while (1) {
        sem_wait(&semMartilloJabalina);
        printf("\033[1;32mEntra un lanzador de martillo\033[0m\n"); //reclamo el semáforo

        sleep(1); //simula el tiempo que tarda en lanzar

        sem_post(&semMartilloJabalina);
        printf("\033[1;32mSe va un lanzador de martillo\033[0m\n");  //libero el semáforo
    }
    pthread_exit(NULL);
}

void *corredor() {
    while (1) {
        //sección de entrada
        pthread_mutex_lock(&mutexCorredores); //protejo la sección crítica

        if(sem_trywait(&cantCorredor) == 0) { //pregunto si ya hay corredor
            sem_post(&cantCorredor); //ya hay, lo dejo como estaba porque después sumo
        } else { //si soy el primero
            sem_wait(&semMartilloJabalina); //me fijo que no haya otro atleta distinto al mío (espero hasta poder entrar)
            printf("Soy el primer corredor y entro porque no hay nadie.\n");
        }

        sem_post(&cantCorredor); //se suma el corredor

        pthread_mutex_unlock(&mutexCorredores); //libero la sección crítica
        
        printf("Corredor se encuentra corriendo.\n");
        sleep(1); //simula el tiempo que tarda en correr
        
        //sección de salida
        pthread_mutex_lock(&mutexCorredores);

        sem_wait(&cantCorredor); //se va uno

        if(sem_trywait(&cantCorredor) == 0) { //pregunto si hay mas
            sem_post(&cantCorredor);
        } else { //si soy el ultimo
            sem_post(&semMartilloJabalina); //libero para todos 
        }
        printf("Se va corredor.\n"); //se va el corredor

        pthread_mutex_unlock(&mutexCorredores);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t jabalina_t[NUM_JABALINA];
    pthread_t martillo_t[NUM_MARTILLO];
    pthread_t corredor_t[NUM_CORREDORES];

    sem_init(&semMartilloJabalina, 0, 1); //semáforo binario
    sem_init(&cantCorredor, 0, 0); //semáforo contador
    pthread_mutex_init(&mutexCorredores, NULL); //mutex para seccion critica

    for (int i = 0; i < NUM_JABALINA; i++) {
        pthread_create(&jabalina_t[i], NULL, lanzador_jabalina, NULL);
    }

    for (int i = 0; i < NUM_MARTILLO; i++) {
        pthread_create(&martillo_t[i], NULL, lanzador_martillo, NULL);
    }

    for (int i = 0; i < NUM_CORREDORES; i++) {
        pthread_create(&corredor_t[i], NULL, corredor, NULL);
    }

    for (int i = 0; i < NUM_JABALINA; i++) {
        pthread_join(jabalina_t[i], NULL);
    }
    for (int i = 0; i < NUM_MARTILLO; i++) {
        pthread_join(martillo_t[i], NULL);
    }
    for (int i = 0; i < NUM_CORREDORES; i++) {
        pthread_join(corredor_t[i], NULL);
    }

    sem_destroy(&semMartilloJabalina);
    sem_destroy(&cantCorredor);
    pthread_mutex_destroy(&mutexCorredores);

    return 0;
}