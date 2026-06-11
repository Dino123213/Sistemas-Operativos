#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define CANT_CLIENTS 10
#define CANT_THREADS 4
#define REPETITIONS 100

#define SERVER 1
#define OPERATION1 2
#define OPERATION2 3
#define OPERATION3 4
#define OPERATION4 5

// Para correrlo: gcc main.c -o main -lpthread -lm  --> ./main  
// Más corto y prolijo con un case, pero ya fue.

struct msg_buffer {
    long type;
    int operation;
    int N;
    int id;
    int result;
};


int sum_even_numbers(int N);
int factorial(int N);
int sum_powers(int N);
int product_odd_numbers(int N);

void *sum_even_numbers_thread(void* arg) {
    struct msg_buffer message;
    int msg_id = *((int *)arg);
    while (1) {
        msgrcv(msg_id, &message, sizeof(message) - sizeof(long), OPERATION1, 0);

        message.result = sum_even_numbers(message.N);

        message.type = message.id;
        msgsnd(msg_id, &message, sizeof(message) - sizeof(long), 0);
    }
}

void *factorial_thread(void* arg) {
    struct msg_buffer message;
    int msg_id = *((int *)arg);
    while (1) {
        msgrcv(msg_id, &message, sizeof(message) - sizeof(long), OPERATION2, 0);

        message.result = factorial(message.N);

        message.type = message.id;
        msgsnd(msg_id, &message, sizeof(message) - sizeof(long), 0);
    }
}

void *sum_powers_thread(void* arg) {
    struct msg_buffer message;
    int msg_id = *((int *)arg);
    while (1) {
        msgrcv(msg_id, &message, sizeof(message) - sizeof(long), OPERATION3, 0);

        message.result = sum_powers(message.N);

        message.type = message.id;
        msgsnd(msg_id, &message, sizeof(message) - sizeof(long), 0);
    }
}

void *product_odd_numbers_thread(void* arg) {
    struct msg_buffer message;
    int msg_id = *((int *)arg);
    while (1) {
        msgrcv(msg_id, &message, sizeof(message) - sizeof(long), OPERATION4, 0);

        message.result = product_odd_numbers(message.N);
        fflush(stdout);
        
        message.type = message.id;
        msgsnd(msg_id, &message, sizeof(message) - sizeof(long), 0);
    }
}

void server(int msg_id) {
    struct msg_buffer message;
    pthread_t threads[CANT_THREADS];

    pthread_create(&threads[0], NULL, &sum_even_numbers_thread, (void *)&msg_id);
    pthread_create(&threads[1], NULL, &factorial_thread, (void *)&msg_id);
    pthread_create(&threads[2], NULL, &sum_powers_thread, (void *)&msg_id);
    pthread_create(&threads[3], NULL, &product_odd_numbers_thread, (void *)&msg_id);
    
    while (1) {
        msgrcv(msg_id, &message, sizeof(message) - sizeof(long), SERVER, 0);
        message.type = message.operation;
        msgsnd(msg_id, &message, sizeof(message) - sizeof(long), 0);
    }

    for (int i = 0; i < CANT_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

void client(int msg_id, int client_id) {
    struct msg_buffer message;
    int id_receive = client_id + 10;
    srand(time(NULL) + getpid());

    for (int i = 0; i < REPETITIONS; i++) {
        message.operation = rand() % 4 + 2;
        message.N = rand() % 10 + 1;
        message.id = client_id + 10;
        message.type = SERVER;
        msgsnd(msg_id, &message, sizeof(message) - sizeof(long), 0);
        printf("\033[1;34mClient %d requests operation %d with N=%d\033[0m\n", client_id, message.operation - 1, message.N);
        usleep(100000);
        msgrcv(msg_id, &message, sizeof(message) - sizeof(long), id_receive, 0);
        printf("\033[1;32mClient %d received result: %d\033[0m\n", client_id, message.result);
        usleep(100000);
    }
}


int main() {
    key_t key; 
    int pidServer;
    int msg_id;

    key = ftok("/tmp", 'A');
    msg_id = msgget(key, IPC_CREAT | 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    pidServer = fork();
    if(pidServer == 0) {
        server(msg_id);
        exit(0);
    }

    for (int i = 0; i < CANT_CLIENTS; i++) {
        int pid = fork();
        if (pid == 0) {
            client(msg_id, i + 1);
            exit(0);
        }
    }

    for (int i = 0; i < CANT_CLIENTS + 1; i++) {
        wait(NULL);
    }

    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
}

// Functions

int sum_even_numbers(int N) {
    int sum = 0;
    for (int i = 1; i <= N; i++) {
        sum += 2 * i;
    }
    return sum;
}

int factorial(int N) {
    int result = 1;
    for (int i = 1; i <= N; i++) {
        result *= i;
    }
    return result;
}

int sum_powers(int N) {
    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += pow(2, i);
    }
    return sum;
}

int product_odd_numbers(int N) {
    int product = 1;
    for (int i = 0; i < N; i++) {
        product *= (2 * i + 1);
    }
    return product;
}
