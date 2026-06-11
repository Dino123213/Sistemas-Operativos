
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CANT 10
#define CANT_SILLAS 5

pthread_mutex_t mutex; 
sem_t clientes;
sem_t barbero_ocupado;
sem_t lleno;

void* barber(void* arg){
while(1){
    printf("Barbero durmiendo...\n");
    sem_wait(&clientes);//Espero a que llegue un cliente
    printf("Me desperte, atiendo clientes\n");
    sleep(2);
    printf("Corte terminado, siguiente cliente\n");
    
    sem_post(&lleno);//Libero un lugar en la barberia
    }

    return NULL;
}

void* corte(void* arg){
    sleep(5);
    int id=(int) arg;
    if(sem_trywait(&lleno)==0){
        printf("Hay lugar en la barberia, voy a entrar, soy Cliente %d\n",id);
        sem_post(&clientes);//Aviso que hay un cliente esperando
    }else{
        printf("No hay lugar en la barberia, me voy, soy Cliente %d\n",id);
    }
    return NULL;
} 

int main(){
    pthread_t client[CANT];
    pthread_t barbero;

    sem_init (&lleno,0,CANT_SILLAS);//Semaforo que se encarga de modelar que el barbero se despierte con la llegada de un cliente
    sem_init(&clientes,0,0);//Semaforo que se encarga de modelar la cantidad de clientes en el local
    

    for(int i=0; i<CANT; i++){
        pthread_create(&client,NULL,corte,(void*) i);
    }

    pthread_create(&barbero,NULL,barber,NULL);

    pthread_join(barbero,NULL);
    for(int i=0; i<CANT; i++){
        pthread_join(client[i],NULL);
    }   
    
    sem_destroy(&clientes);
    sem_destroy(&lleno);

    return 0;
}