#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    //creo un hijo que crea dentro 3 nietos
    pid_t p1;
    
    p1 = fork();
    if (p1 == 0) {
        //soy el hijo
        pid_t p2, p3, p4;

        p2 = fork();
        if (p2 == 0) {
            //soy p2
            printf("Soy p2 PID: %d, y mi padre es PPID: %d\n", getpid(), getppid());
            exit(0);
        }

        p3 = fork();
        if (p3 == 0) {
            //soy p3
            printf("Soy p3 PID: %d, y mi padre es PPID: %d\n", getpid(), getppid());
            //creo dos hijos más
            pid_t p5, p6;
            p5 = fork();
            if (p5 == 0) {
                //soy p5
                printf("Soy p5 PID: %d, y mi padre es PPID: %d\n", getpid(), getppid());
                exit(0);
            }

            p6 = fork();
            if (p6 == 0) {
                //soy p6
                printf("Soy p6 PID: %d, y mi padre es PPID: %d\n", getpid(), getppid());
                exit(0);
            }
            exit(0);
        }

        p4 = fork();
        if (p4 == 0) {
            // Soy p4
            printf("Soy p4 PID: %d, y mi padre es PPID: %d\n", getpid(), getppid());
            exit(0);
        }
        exit(0);
    }

    //espero a que terminen p2, p3, p4
        for(int i = 0; i < 6; i++) {
            wait(NULL);
        }

    //creo otro dos hijos
    pid_t p7, p8;
    p7 = fork();
    if (p7 == 0) {
        //soy p7
        printf("Soy p7 PID: %d, y mi padre es PPID: %d\n", getpid(), getppid());
        exit(0);
    }

    p8 = fork();
    if (p8 == 0) {
        //soy p8
        printf("Soy p8 PID: %d, y mi padre es PPID: %d\n", getpid(), getppid());
        exit(0);
    }

    for(int i = 0; i < 2; i++) {
        wait(NULL);
    }

    printf("Soy el padre y el grafo ha terminado.\n");

    return 0;
}