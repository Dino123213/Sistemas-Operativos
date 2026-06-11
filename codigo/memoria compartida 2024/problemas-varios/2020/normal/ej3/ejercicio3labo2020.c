/*
 * 
 * Se tiene un arreglo de 100 elementos en el que se quiere realizar varias iteraciones en cada
una de las cuales se debe colocar en cada casilla la media de la suma del contenido de esa
casilla y sus dos adyacentes. Es decir que para cualquier casilla del arreglo entre 1 y 98 el
nuevo valor de la casilla ser´a v[i]= (v[i-1]+v[i]+v[i+1])/3; para la casilla 0 se supondr´a que su
adyacente izquierda es la casilla 99 y para la casilla 99 se supondr´a que su adyacente derecha
es la casilla 0.
El procedimiento se aplicar´a durante 200 iteraciones y para optimizarlo se desea que la mitad
del arreglo la procese un hilo (thread) y la otra mitad otro hilo (thread). Las operaciones de
c´alculo de los nuevos valores se realizar´an en un array auxiliar y s´olo cuando los dos hilos
(threads) hayan terminado su iteraci´on volcar´an los valores de ´este sobre el real para continuar
con la siguiente iteraci´on. Por tanto el procedimiento que seguir´a cada hilo (thread) es:
Copiar los nuevos valores en el arreglo auxiliar
Cuando haya terminado de copiarlos deber´a esperar a que termine el otro hilo (thread)
antes de volcar los datos del arreglo auxiliar en el arreglo real. De esta forma no se
modifican los datos de una iteraci´on antes de que el otro utilice la casilla que tienen en
com´un.
Una vez copiados los datos en el arreglo real deber´a esperar a que el otro hilo (thread)
termine tambi´en de copiar los datos auxilares sobre el arreglo real antes de proceder a
la siguiente iteraci´on del paso 1. Al igual que en el paso 2, hay que esperar para que las
casillas comunes est´en actualizadas.
Considere que cada casilla del arreglo est´a inicializada con el valor de la posici´on a la cual
pertenece.
a) Implemente la soluci´on para el problema planteado utilizando threads y sem´aforos para
la sincronizaci´on.
b) Explique cu´ales modificaciones tendr´ıa que realizar si se cuenta con 4 threads para
resolver el problema.
c) Proponga una soluci´on a la implementaci´on realizada en la cu´al se pueda mejorar el
espacio ocupado por las variables auxiliares
 * 
 */


 /* pseudoalgoritmo
  * 
  * 
  * seccion critica
  * hacer wait en un sem, hacer  trywait, y si sos el ultimo, entonces habilitar a que todos lean
  * seccion critica
  * 
  * leer casilla, y hacer suma,  ubicarla en arreglo auxiliar
  * 
  * 
  * region critica
  * hacer wait en un sem, hacer  trywait, y si sos el ultimo, entonces liberar a los tipos, y reiniciar el sem de antes
  * region critica
  * 
  * modificar datos
  * 
  * reiniciar el sem de antes (Aca o mas arriba, en donde esta la seccion critica)
  * 
  */ 
  
  
  
  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#define SIZE_BUFFER 10
#define CANT_HILOS 2

struct msg {

    int nrocomienzo;
    int nrofinal;
    
};

int globalarr[SIZE_BUFFER];


//usados para garantizar exclusion mutua con santa en la parte de ver si llenaron el grupo
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;





sem_t esperando_leer; //limita la cantidad de elfos que pueden pedir ayuda
sem_t esperando_escribir; //limita la cantidad de renos que pueden haber por trineo 

sem_t quiere_leer ;//contador
sem_t quiere_escribir; //contador


void *modificadorarreglo(void *arg){
	
    struct msg *mens = (struct msg *) arg;
	int nrocom = mens->nrocomienzo;
    int nrofin = mens->nrofinal;
	int arr[SIZE_BUFFER/CANT_HILOS],suma;	
			
			
			pthread_mutex_lock(&mutex);
			
				sem_wait(&quiere_leer);
				if(sem_trywait(&quiere_leer) != 0){ //si soy el ultimo
					sem_wait(&esperando_escribir);
                    for(int i = 0; i< CANT_HILOS; i++){ sem_post(&quiere_escribir); }
                    //esas dos reinician el contador para que despues puedan escribir
                    sem_post(&esperando_leer);
                    pthread_mutex_unlock(&mutex);		
				}else{
                    sem_post(&quiere_leer);
                    pthread_mutex_unlock(&mutex);
                    sem_wait(&esperando_leer);
                    sem_post(&esperando_leer);
                }
			
			//seccion de lectura
			for(int i = nrocom; i < nrofin; i++){
                if(i == nrocom && i == 0){
                    suma = globalarr[SIZE_BUFFER-1] + globalarr[i] + globalarr[i+1];
                    
                }else if(i == nrofin-1 && nrofin == SIZE_BUFFER){
                    suma = globalarr[i-1] + globalarr[i] + globalarr[0];
                    
                }
                else{
                    suma = globalarr[i-1] + globalarr[i] + globalarr[i+1];
                    
                }
                arr[i-nrocom] = suma;
            }
            
            

			pthread_mutex_lock(&mutex);
			
				sem_wait(&quiere_escribir);
				if(sem_trywait(&quiere_escribir) != 0){ //si soy el ultimo
					sem_wait(&esperando_leer);
                    for(int i = 0; i< CANT_HILOS; i++){ sem_post(&quiere_leer); }
                    //esas dos reinician el contador para que despues puedan leer
                    sem_post(&esperando_escribir);
                    pthread_mutex_unlock(&mutex);		
				}else{
                    sem_post(&quiere_escribir);
                    pthread_mutex_unlock(&mutex);
                    sem_wait(&esperando_escribir);
                    sem_post(&esperando_escribir);
                }
                
            //seccion escritura
            for(int i = nrocom; i < nrofin; i++){
               // printf("%d\n",arr[i-nrocom]);  
                globalarr[i] = arr[i-nrocom];
            }
            
            
            
		
		
	pthread_exit(0);
}





int main(){
	
	
	pthread_t hilos[CANT_HILOS];
	struct msg structs[CANT_HILOS];
	
	sem_init(&esperando_escribir,0,1);
	sem_init(&esperando_leer,0,0);
	sem_init(&quiere_escribir ,0, 0);
	sem_init(&quiere_leer,0,CANT_HILOS);


    for(int i = 0; i<SIZE_BUFFER; i++){
    
        globalarr[i]  = i+1;
    }

	
	
	int com = 0, intervalos = SIZE_BUFFER / CANT_HILOS;
	for(int i = 0; i<CANT_HILOS; i++){
        structs[i].nrocomienzo = com;
        structs[i].nrofinal = com + intervalos ;
		pthread_create(&hilos[i],NULL,modificadorarreglo,&structs[i]);
        com+=intervalos;
	}
	

	

	for(int c = 0; c<CANT_HILOS; c++){
		pthread_join(hilos[c],NULL);
	}
	
	
    for(int i = 0; i<SIZE_BUFFER; i++){
        
        printf("%d ",globalarr[i]);
    
    }
	 printf("\n");
	sem_close(&quiere_escribir );
	sem_close(&quiere_leer );
	sem_close(&esperando_escribir );
	sem_close(&esperando_leer );
	


	return 0;
	
}

