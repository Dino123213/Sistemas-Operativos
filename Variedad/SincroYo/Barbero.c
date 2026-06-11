#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 3   // número de sillas

sem_t barbero;     // barbero dormido (0 si duerme)
sem_t cliente;     // clientes esperando
sem_t sillas;      // sillas disponibles
pthread_mutex_t mutex; 

void* funcion_barbero(void* arg) {
    while (1) {
        // Espera a un cliente 
        printf("Barbero duerme\n");
        sem_wait(&cliente);// si no hay clientes, duerme
        pthread_mutex_lock(&mutex);   
        // Toma una silla (siempre habrá una porque cliente la tomó)
        sem_post(&sillas);
        printf("Barbero atiende a un cliente\n");

        // Cuando termina de atender, se despierta el cliente atendido
        pthread_mutex_unlock(&mutex);
        sem_post(&barbero);
       
    }
}

void* funcion_cliente(void* arg) {
    int id=(int*) arg;
    // Intenta ocupar una silla
    pthread_mutex_lock(&mutex);//Reserva la sección crítica
    if (sem_trywait(&sillas) == 0) {
        printf("Cliente %d se sienta a esperar\n",id);
        sem_post(&cliente);        // avisa que hay cliente
        pthread_mutex_unlock(&mutex);
        sem_wait(&barbero);        // espera que lo atiendan
        printf("Cliente fue atendido y se va\n");
    } else {
        printf("Cliente %d se va (no hay sillas)\n");
        pthread_mutex_unlock(&mutex);//Libera la seccion crítica
    }
    return NULL;
}

int main() {
    pthread_t hbarbero, clientes[10];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&barbero, 0, 0);
    sem_init(&cliente, 0, 0);
    sem_init(&sillas, 0, N);

    pthread_create(&hbarbero, NULL, funcion_barbero, NULL);

    for (int i = 0; i < 10; i++) {
        pthread_create(&clientes[i], NULL, funcion_cliente,(void*) i);
    }
    pthread_mutex_destroy(&mutex);
    pthread_join(hbarbero, NULL);
    return 0;
}