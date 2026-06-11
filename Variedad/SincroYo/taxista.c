#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define asientos 5
#define pasajeros 20

sem_t sem_asientos;
sem_t sem_taxista;
sem_t viaje;


void* subir(void* arg){
    int id=(int*) arg;
    while(1){
    sem_wait(&viaje);//Si el taxista no comenzo viaje espero
    sem_wait(&sem_asientos);//Verifico que hayan asientos disponibles, si no hay espero
    printf("Soy %d me subi al taxi y estoy esperando que arranque el viaje\n",id);
    sleep(2);
    if(sem_trywait(&sem_asientos)==0){//Verifico si hay mas lugares en el taxi
        //Si hay mas lugar devuelvo el que ocupe para simular
        sem_post(&sem_asientos);
        sem_post(&viaje);//Como sigue habiendo lugar, permito que otro pasajero suba
    }else{
        //Si no hay mas lugares entonces le aviso al chofer que maneje
        sem_post(&sem_taxista);
    }
}
    return NULL;
}



void* tachero(){
    while(1){
        sem_wait(&sem_taxista);
        printf("Taxista: esperando pasajeros...\n");
        sleep(2); // Simula tiempo de espera
        printf("Taxista: comenzando viaje con un pasajero.\n");
        for(int i=0;i<asientos;i++){
            sem_post(&sem_asientos);
        }
        sleep(5); // Simula tiempo de viaje
        sem_post(&viaje);
        printf("Taxista: viaje terminado.\n");
    }
    return NULL;
}


int main(){
    pthread_t taxista;
    pthread_t clientes[pasajeros];

    sem_init(&sem_asientos,0,asientos);
    sem_init(&sem_taxista,0,0);
    sem_init(&viaje,0,1);

    pthread_create(&taxista,NULL,tachero,NULL);

    for(int i=0;i<pasajeros;i++){
        pthread_create(&clientes[i],NULL,subir,(void*)i);
    }

    for(int i=0;i<pasajeros;i++){
        pthread_join(clientes[i],NULL);
    }

    sem_destroy(&sem_asientos);
    sem_destroy(&sem_taxista);
    sem_destroy(&viaje);
    return 0;
}