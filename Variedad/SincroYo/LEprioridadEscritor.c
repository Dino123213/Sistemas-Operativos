#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#define lectores 10
#define escritores 3


sem_t Lectores,Escritores,cantEscritores,cantLectores, escriboPrioridad,Leyendo;
sem_t mutexLect, mutexEsc;

void *escritor(void *arg){
	while(1){
        sem_post(&cantEscritores);//Indico que hay un escritor esperando
        sem_wait(&escriboPrioridad);//Espero a que no haya lectores leyendo
        sem_wait(&Leyendo);//Espero a que no haya lectores leyendo
    }
}


void *lector(void *arg){
	do{
		if(sem_trywait(&cantEscritores)==0){//Si hay escritores esperando
            sem_post(&cantEscritores);//Devuelvo el valor
            sem_wait(&escriboPrioridad);//Espero a que terminen de escribir
        }else{
            if(sem_trywait(&cantLectores)==0){//Si hay otros lectores leyendo
                sem_post(&cantLectores);//Devuelvo el valor
            }else{
                sem_post(&cantLectores);//Indico que hay un lector esperando
                sem_wait(&Leyendo);//Bloqueo a los escritores
            }
        }

        printf("Leyendo...\n");
        sleep(1);
        
        sem_post(&cantLectores);//Indico que termine de leer
        if(sem_trywait(&cantLectores)==0){//Si hay mas lectores leyendo
            sem_post(&cantLectores);//Devuelvo el valor
        }else{
            sem_post(&Leyendo);//Indico que ya no hay lectores leyendo
        }
	
	
	}while(1);
	
	pthread_exit(0);
}

int main(){
	

	
	
	
	pthread_t  hilos_lect[lectores], hilos_esc[escritores];
	
	
	sem_init(&Lectores,0,1);
	sem_init(&Escritores,0,1);
	sem_init(&cantEscritores,0,0);
	sem_init(&cantLectores,0,0);
	sem_init(&mutexEsc,0,1);
	sem_init(&mutexLect,0,1);
    sem_init(&escriboPrioridad,0,1);
    sem_init(&Leyendo,0,1);
	
	
	for(int i = 0; i<lectores; i++){
	pthread_create(&hilos_lect[i],NULL,lector,NULL);
	
	
	}
	for(int i = 0; i<escritores; i++){
	pthread_create(&hilos_esc[i],NULL,escritor,NULL);
	
	
	}
	
	
	
	for(int i = 0; i<lectores; i++){
	pthread_join(hilos_lect[i],NULL);
	
	
	}
	for(int i = 0; i<escritores; i++){
	pthread_join(hilos_esc[i],NULL);
	
	
	}
	
	sem_close(&Lectores);
	sem_close(&Escritores);
	sem_close(&cantEscritores);
	sem_close(&cantLectores);
	sem_close(&mutexEsc);
	sem_close(&mutexLect);

	return 0;
	
}
