#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define pajaros 10
#define porciones 5

sem_t vacio;
sem_t padre;
sem_t pmutex;
sem_t llenando;

void* consumir(void* arg){
    int id=(int) arg;
    
    sem_wait(&llenando);
    sem_wait(&pmutex);
    sem_wait(&vacio);
    sleep(3);
    printf("Pajaro %d esta comiendo\n", id);
    if(sem_trywait(&vacio)==0){
        sem_post(&vacio);    
    }else{
        printf("Vacie el plato, voy a llamar a mi padre para que lo llene\n");
        sem_wait(&llenando);
        sem_post(&padre);
    }
    sem_post(&pmutex);
}

void* lleno(){
    sem_wait(&padre);
    printf("Soy el padre y desperte para llenar el plato\n");
    for(int i=0;i<porciones;i++){
        sem_post(&vacio);
    }
    printf("Termine de llenar el plato\n");
    sem_post(&llenando);
}

int main(){
    pthread_t pajaritos[pajaros];
    pthread_t padrepajarito;

    sem_init(&vacio,0,porciones);
    sem_init(&padre,0,0);
    sem_init(&pmutex,0,1);
    sem_init(&llenando,0,1);

    for(int i=0;i<pajaros;i++){
        pthread_create(&pajaritos[i],NULL,consumir,(void *)i);
    }

    pthread_create(&padrepajarito,NULL,lleno,NULL);

    for(int i=0;i<pajaros;i++){
        pthread_join(pajaritos[i],NULL);
    }
    sem_destroy(&llenando);
    sem_destroy(&padre);
    sem_destroy(&pmutex);
    sem_destroy(&vacio);
}