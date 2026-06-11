#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PASAJEROS 10
#define NUM_ASIENTOS 5

sem_t asientos,lleno,llegada;

pthread_mutex_t mutex;



void* automovil(void * p){

  int i;

  while(1){

    printf("LLEGA AUTO - Esperando ocupantes \n");

    for(i=0;i<NUM_ASIENTOS;i++){
      sem_wait(&lleno);
    }

    printf("VIAJANDO \n");
    sleep(6);

    for(i=0;i<NUM_ASIENTOS;i++){
      sem_post(&asientos);
    }

    printf("LLEGADA DESTINO - Todos los pasajeros bajaron del auto \n");

  }

 }

 void * pasajeros(void * numero){

  int num=(int)numero;

      pthread_mutex_lock(&mutex);
          sem_wait(&asientos);
          printf("Soy pasajero %i entro al auto  \n",num);
          sem_post(&lleno);
      pthread_mutex_unlock(&mutex);

}

void * pasajerosViajeros(void * numero){

  while(1){

      pasajeros(numero);
      //Espera antes de tomar otro viaje.
      sleep(3);

  }

}



int main()
{

    //Threads
    pthread_t t_pasajeros [NUM_PASAJEROS];
    pthread_t t_automovil;

    //Mutex
    pthread_mutex_init(&mutex,NULL);

    //Inicializacion de los semaforos
    sem_init(&asientos,0,NUM_ASIENTOS);
    sem_init(&lleno,0,0);
    sem_init(&llegada,0,0);

    if(pthread_create(&t_automovil,NULL,automovil,NULL))
    {
        printf("\n Inicializacion de los Threads Falló \n");
        exit(EXIT_FAILURE);
    }

    //PASAJEROS SOLO VIAJAN 1 VEZ

    /*

     for(int i=0;i<NUM_PASAJEROS;i++){

        if(pthread_create(&t_pasajeros[i],NULL,pasajeros,i))
      {
        printf("\n Inicializacion de los Threads Falló \n");
        exit(EXIT_FAILURE);
      }

    }

    */

    //PASAJEROS VIAJAN INDEFINIDAMENTE

    for(int i=0;i<NUM_PASAJEROS;i++){

        if(pthread_create(&t_pasajeros[i],NULL,pasajerosViajeros,i))
      {
        printf("\n Inicializacion de los Threads Falló \n");
        exit(EXIT_FAILURE);
      }

    }

    for(int i=0;i<NUM_PASAJEROS;i++){
      pthread_join(t_pasajeros[i],NULL);
    }

    pthread_join(t_automovil,NULL);

    return 0;

}



