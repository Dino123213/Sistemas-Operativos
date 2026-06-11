#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define corredores 3
#define martillo 2
#define jabalinas 2

sem_t sem_corredores;

sem_t instalaciones;

void* atleta_corredor(void* arg){
    int id=(int) arg;
    if(sem_trywait(&sem_corredores)==0){
        sleep(1);//Simulo tiempo de llegada
        printf("Atleta corredor %d entro a las instalaciones.\n",id);
        sem_post(&sem_corredores);//hay al menos un corredor en las instalaciones
        sem_post(&sem_corredores);
    }else{
        sem_wait(&instalaciones);//No hay corredores, bloqueo hasta que las instalaciones 
        //esten disponibles
        sem_post(&sem_corredores);//Aumento la cantidad de corredores dentro de las instalaciones
        printf("Atleta corredor %d entro a las instalaciones.\n",id);
    }

    //Simulo actividad del corredor
    printf("Atleta corredor %d esta corriendo...\n",id);
    sleep(3);
    //Termina actividad
    printf("Atleta corredor %d salio de las instalaciones.\n",id);
    sem_wait(&sem_corredores);//Salgo de las instalaciones 
    if(sem_trywait(&sem_corredores)==0){
        sem_post(&sem_corredores);//Aun quedan corredores dentro de las instalaciones
    }else{
        printf("Ultimo corredor %d salio, liberando instalaciones.\n",id);
        sem_post(&instalaciones);
    }
    return NULL;
}

void* atleta_martillo(void* arg){
    int id=(int) arg;
    sem_wait(&instalaciones);//Pido permiso para ingresar a las instalaciones
    printf("Atleta martillo %d entro a las instalaciones.\n",id);
    //Simulo actividad del martillo
    printf("Atleta martillo %d esta lanzando el martillo...\n",id);
    sleep(4);
    //Termina actividad
    printf("Atleta martillo %d salio de las instalaciones.\n",id);
    sem_post(&instalaciones);
    return NULL;
}

void* atleta_jabalinas(void* arg){
    int id=(int) arg;
    sem_wait(&instalaciones);
    printf("Atleta jabalinas %d entro a las instalaciones.\n",id);
    //Simulo actividad de jabalinas
    printf("Atleta jabalinas %d esta lanzando la jabalina...\n",id);
    sleep(2);
    //Termina actividad
    printf("Atleta jabalinas %d salio de las instalaciones.\n",id);
    sem_post(&instalaciones);
    return NULL;
}


int main(){
    pthread_t t_corredores[corredores];
    pthread_t t_martillo[martillo];
    pthread_t t_jabalinas[jabalinas];


    sem_init(&sem_corredores,0,0);
    sem_init(&instalaciones,0,1);

    for(int i=0;i<corredores;i++){
        pthread_create(&t_corredores[i],NULL,atleta_corredor,(void*)i);
    }
    
    for(int i=0;i<martillo;i++){
        pthread_create(&t_martillo[i],NULL,atleta_martillo,(void*)i);
    }

    for(int i=0;i<jabalinas;i++){
        pthread_create(&t_jabalinas[i],NULL,atleta_jabalinas,(void*)i);
    }

    for(int i=0;i<corredores;i++){
        pthread_join(t_corredores[i],NULL);
    }
    for(int i=0;i<martillo;i++){
        pthread_join(t_martillo[i],NULL);
    }
    for(int i=0;i<jabalinas;i++){
        pthread_join(t_jabalinas[i],NULL);
    }


    sem_destroy(&sem_corredores);
    sem_destroy(&instalaciones);
    return 0;
}