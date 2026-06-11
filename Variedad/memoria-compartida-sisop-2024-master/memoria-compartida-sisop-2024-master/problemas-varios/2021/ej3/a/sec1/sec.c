#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semAoB, semC,semD;
//Sec (AoB)CD (AoB)CD (AoB)CD (AoB)CD
void* taskA(){
    while(1){
        sem_wait(&semAoB);
        printf("A");
        sem_post(&semC);
    }
}

void* taskB(){
    while(1){
        sem_wait(&semAoB);
        printf("B");
        sem_post(&semC);
    }
}
void* taskC(){
    while(1){
        sem_wait(&semC);
        printf("C");
        sem_post(&semD);
    }
}

void* taskD(){
    while(1){
        sem_wait(&semD);
        printf("D\n");
        sem_post(&semAoB);
    }
}
int main(){
    pthread_t tasks[4];
    sem_init(&semAoB, 0,1);
    sem_init(&semC, 0,0);
    sem_init(&semD, 0,0);
    
    pthread_create(&tasks[0],NULL,&taskA,NULL);
    pthread_create(&tasks[1],NULL,&taskB,NULL);
    pthread_create(&tasks[2],NULL,&taskC,NULL);
    pthread_create(&tasks[3],NULL,&taskD,NULL);
    
    pthread_join(tasks[0],NULL);
    pthread_join(tasks[1],NULL);
    pthread_join(tasks[2],NULL);
    pthread_join(tasks[3],NULL);
    
    sem_destroy(&semAoB);
    sem_destroy(&semC);
    sem_destroy(&semD);
    return 0;
}
