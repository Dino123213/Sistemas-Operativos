#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semAoB, semCoD, semE, semF;
//Sec (AoB)(CoD)EF (AoB)(CoD)EF (AoB)(CoD)EF (AoB)(CoD)EF
void* taskA(){
    while(1){
        sem_wait(&semAoB);
        printf("A");
        sem_post(&semCoD);
    }
}

void* taskB(){
    while(1){
        sem_wait(&semAoB);
        printf("B");
        sem_post(&semCoD);
    }
}
void* taskC(){
    while(1){
        sem_wait(&semCoD);
        printf("C");
        sem_post(&semE);
    }
}

void* taskD(){
    while(1){
        sem_wait(&semCoD);
        printf("D");
        sem_post(&semE);
    }
}

void* taskE(){
    while(1){
        sem_wait(&semE);
        printf("E");
        sem_post(&semF);
    }
}

void* taskF(){
    while(1){
        sem_wait(&semF);
        printf("F\n");
        sem_post(&semAoB);
    }
}
int main(){
    pthread_t tasks[4];
    sem_init(&semAoB, 0,1);
    sem_init(&semCoD, 0,0);
    sem_init(&semE, 0,0);
    sem_init(&semF, 0,0);

    pthread_create(&tasks[0],NULL,&taskA,NULL);
    pthread_create(&tasks[1],NULL,&taskB,NULL);
    pthread_create(&tasks[2],NULL,&taskC,NULL);
    pthread_create(&tasks[3],NULL,&taskD,NULL);
    pthread_create(&tasks[4],NULL,&taskE,NULL);
    pthread_create(&tasks[5],NULL,&taskF,NULL);
    
    pthread_join(tasks[0],NULL);
    pthread_join(tasks[1],NULL);
    pthread_join(tasks[2],NULL);
    pthread_join(tasks[3],NULL);
    pthread_join(tasks[4],NULL);
    pthread_join(tasks[5],NULL);
    
    sem_destroy(&semAoB);
    sem_destroy(&semCoD);
    sem_destroy(&semE);
    sem_destroy(&semF);
    return 0;
}
