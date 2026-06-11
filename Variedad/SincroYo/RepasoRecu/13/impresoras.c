
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CANT_IMPRESORAS 2
#define CANT_USUARIOS 10

sem_t impresora[CANT_IMPRESORAS];
sem_t libres;
pthread_mutex_t mutex;



int requerirImpresora(){
    int id_impresora=-1;
    sem_wait(&libres);
        for(int i=0;i<CANT_IMPRESORAS;i++){
            if(sem_trywait(&impresora[i])==0){
                id_impresora=i;//Si hay alguna impresora libre entonces 
                printf("La impresora %d estaba libre, la bloqueo\n",id_impresora);
                return id_impresora;
            }
        }
    return id_impresora;
}

void liberarImpresora(void* arg){
    int id_imp=(int) arg;
    sem_post(&impresora[id_imp]);
    sem_post(&libres);
    printf("Libero la impresora %d\n",id_imp);
}

void* user(void* arg){
    int user=(int) arg;
    printf("Soy el usuario %d y quiero imprimir\n",user);
    sleep(2);

    int id_impresora=requerirImpresora();//Obteno el id de la impresora en la que voy a imprimir
    int sleep_time=rand()%10;
    printf("Soy el usuario %d Estoy imprimiendo en la impresora %d\n",user,id_impresora);
    sleep(sleep_time);

    printf("Soy %d Termine de imprimir y voy a devolver la impresora\n",user);
    liberarImpresora((void*)id_impresora);
    return NULL;
}




int main(){
    pthread_t usuarios[CANT_USUARIOS];

    sem_init (&libres,0,CANT_IMPRESORAS);
    sem_init(&impresora[0],0,1);
    sem_init(&impresora[1],0,1);
    
    for(int i=0;i<CANT_USUARIOS;i++){
        pthread_create(&usuarios[i],NULL,user,(void*)i);
    }

    for(int i=0;i<CANT_USUARIOS;i++){
        pthread_join(&usuarios[i],NULL);
    }
    sem_destroy(&impresora[0]);
    sem_destroy(&impresora[1]);
    sem_destroy(&libres);

    return 0;
}