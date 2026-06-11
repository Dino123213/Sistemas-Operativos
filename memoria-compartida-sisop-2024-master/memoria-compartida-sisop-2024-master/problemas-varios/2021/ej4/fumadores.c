#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

sem_t agenteMesa;
sem_t tabaco, papel, cerillas;

void *agent() {
    while (1) {
        sem_wait(&agenteMesa);
        int choice = rand() % 3; // Selecciona al azar qué dos ingredientes colocar
        if (choice == 0) {
            printf(GREEN "Agente: pone tabaco y papel en la mesa\n" RESET);
            sleep(1);
            sem_post(&cerillas); 
        } else if (choice == 1) {
            printf(GREEN "Agente: pone tabaco y cerillas en la mesa\n" RESET);
            sleep(1);
            sem_post(&papel); 
        } else {
            printf(GREEN "Agente: pone papel y cerillas en la mesa\n" RESET);
            sleep(1);
            sem_post(&tabaco); 
        }
    }
}
void *smoker_with_tobacco() {
    while (1) {
            sem_wait(&tabaco); 
            printf(YELLOW "Fumador con tabaco: toma papel y cerillas y arma un cigarillo\n" RESET);
            sleep(3);
            sem_post(&agenteMesa); // Informa al agente que terminó
    }
}

void *smoker_with_paper() {
    while (1) {        
            sem_wait(&papel);
            printf(CYAN "Fumador con papel: toma tabaco y cerillas y arma un cigarillo\n" RESET);
            sleep(2);
            sem_post(&agenteMesa); // Informa al agente que terminó
    }
}

void *smoker_with_matches() {
    while (1) {
            sem_wait(&cerillas);
            printf(MAGENTA "Fumador con cerillas: toma papel y tabaco y arma un cigarillo\n" RESET);
            sleep(2);
            sem_post(&agenteMesa); // Informa al agente que terminó
    }
}

int main() {
    pthread_t agent_thread, smoker1, smoker2, smoker3;

    sem_init(&tabaco, 0, 0);
    sem_init(&papel, 0, 0);
    sem_init(&cerillas, 0, 0);
    sem_init(&agenteMesa, 0, 1); // El agente comienza
    pthread_create(&agent_thread, NULL, agent, NULL);
    pthread_create(&smoker1, NULL, smoker_with_tobacco, NULL);
    pthread_create(&smoker2, NULL, smoker_with_paper, NULL);
    pthread_create(&smoker3, NULL, smoker_with_matches, NULL);

    pthread_join(agent_thread, NULL);
    pthread_join(smoker1, NULL);
    pthread_join(smoker2, NULL);
    pthread_join(smoker3, NULL);

    sem_destroy(&tabaco);
    sem_destroy(&papel);
    sem_destroy(&cerillas);
    sem_destroy(&agenteMesa);    

    return 0;
}
