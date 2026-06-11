#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_PRIORITY 10
#define MIN_CPUS 1
#define MAX_CPUS 8
#define SIMULATION_TIME 60
#define MAX_PROCESSES 100

// Estructura para representar un proceso
typedef struct {
    long mtype;
    int pid;
    int priority;
    int io_required;
} Process;

// Estructura para los mensajes entre procesos
typedef struct {
    long mtype;
    Process process;
} Message;

// Funciones auxiliares
void create_process(Process *p, int pid) {
    p->pid = pid;
    p->priority = rand() % MAX_PRIORITY + 1;
    p->io_required = rand() % 2;
}

void send_message(int queue_id, Process *p, long mtype) {
    Message msg;
    msg.mtype = mtype;
    msg.process = *p;
    if (msgsnd(queue_id, &msg, sizeof(Process), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }
}

Process receive_message(int queue_id, long mtype) {
    Message msg;
    if (msgrcv(queue_id, &msg, sizeof(Process), mtype, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }
    return msg.process;
}

// Proceso CPU
void cpu_process(int cpu_id, int ready_queue, int wait_queue) {
    srand(time(NULL) + cpu_id);
    while (1) {
        Process p = receive_message(ready_queue, -MAX_PRIORITY);
        printf("CPU %d ejecutando proceso %d (prioridad %d)\n", cpu_id, p.pid, p.priority);
        fflush(stdout);
        sleep(rand() % 3 + 3); // Simular ejecución

        if (p.io_required) {
            printf("CPU %d: Proceso %d requiere E/S\n", cpu_id, p.pid);
            fflush(stdout);
            send_message(wait_queue, &p, 1);
        } else {
            printf("CPU %d: Proceso %d terminado\n", cpu_id, p.pid);
            fflush(stdout);
        }
    }
}

// Proceso de E/S
void io_process(int wait_queue, int ready_queue) {
    while (1) {
        Process p = receive_message(wait_queue, 0);
        printf("E/S: Procesando E/S para proceso %d\n", p.pid);
        fflush(stdout);
        sleep(rand() % 5 + 1); // Simular E/S
        p.io_required = rand() % 2; // Puede requerir más E/S en el futuro
        send_message(ready_queue, &p, p.priority);
        printf("E/S: Proceso %d vuelve a la cola de listos\n", p.pid);
        fflush(stdout);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numero_cpus>\n", argv[0]);
        exit(1);
    }

    int num_cpus = atoi(argv[1]);
    if (num_cpus < MIN_CPUS || num_cpus > MAX_CPUS) {
        fprintf(stderr, "El número de CPUs debe estar entre %d y %d\n", MIN_CPUS, MAX_CPUS);
        fflush(stdout);
        exit(1);
    }

    srand(time(NULL));

    // Crear colas de mensajes
    int ready_queue = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    msgctl(ready_queue, IPC_RMID, NULL);
    ready_queue = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    int wait_queue = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    msgctl(wait_queue, IPC_RMID, NULL);
    wait_queue = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    if (ready_queue == -1 || wait_queue == -1) {
        perror("msgget");
        exit(1);
    }

    // Crear procesos CPU
    for (int i = 0; i < num_cpus; i++) {
        if (fork() == 0) {
            cpu_process(i, ready_queue, wait_queue);
            exit(0);
        }
    }

    // Crear proceso de E/S
    if (fork() == 0) {
        io_process(wait_queue, ready_queue);
        exit(0);
    }

    // Proceso principal: generar procesos y enviarlos a la cola de listos
    for (int i = 0; i < MAX_PROCESSES; i++) {
        Process p;
        create_process(&p, i);
        send_message(ready_queue, &p, p.priority);
        printf("Proceso %d creado con prioridad %d\n", p.pid, p.priority);
        fflush(stdout);
        sleep(rand() % 3); // Esperar un poco antes de crear el siguiente proceso
    }

    // Esperar un tiempo para que la simulación se ejecute
    sleep(SIMULATION_TIME);

    // Terminar todos los procesos hijos
    kill(0, SIGTERM);

    // Esperar a que todos los procesos hijos terminen
    while (wait(NULL) > 0);

    // Eliminar las colas de mensajes
    msgctl(ready_queue, IPC_RMID, NULL);
    msgctl(wait_queue, IPC_RMID, NULL);

    return 0;
}
