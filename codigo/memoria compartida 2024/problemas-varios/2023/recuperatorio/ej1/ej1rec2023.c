#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ITER 125
#define BUFFER_SIZE 256
#define TIME 1

int pipe_par[2];
int pipe_impar[2];
int pipe_res[2];

int cant_palabras(char *buffer) {
    int count = 0;
    int in_word = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t') {
            if (in_word) {
                count++;
                in_word = 0;
            }
        } else {
            in_word = 1;
        }
    }
    if (in_word) {
        count++;
    }
    return count;
}

int cant_A(char *buffer) {
    int count = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == 'A') {
            count++;
        }
    }
    return count;
}

void par_impar(FILE *file) {
    close(pipe_par[0]);
    close(pipe_impar[0]);

    for (int i = 0; i < ITER; i++) {
        sleep(TIME);

        char buffer[BUFFER_SIZE];

        if (fgets(buffer, sizeof(buffer), file) != NULL) {

            printf("Lector lee linea %d:\n", i);

            if (i % 2 == 0) {
                write(pipe_par[1], buffer, sizeof(buffer));
            } else {
                write(pipe_impar[1], buffer, sizeof(buffer));
            }

        } else {
            printf("Fin de archivo\n");
            break;
        }
    }
    close(pipe_par[1]);
    close(pipe_impar[1]);
}

void pares() {
    close(pipe_par[1]);
    close(pipe_res[0]);
    char buffer_res[BUFFER_SIZE];

    for (int i = 0; i < ITER; i++) {
        sleep(TIME);

        char buffer[BUFFER_SIZE];
        read(pipe_par[0], buffer, sizeof(buffer));

        int cant_p = cant_palabras(buffer);
        
        sprintf(buffer_res, "proc1: tiene %d palabras \n", cant_p);

        write(pipe_res[1], buffer_res, sizeof(buffer));
    }
    close(pipe_par[0]);
    close(pipe_res[1]);
}

void impares() {
    close(pipe_impar[1]);
    close(pipe_res[0]);
    char buffer_res[BUFFER_SIZE];

    for (int i = 0; i < ITER; i++) {
        sleep(TIME);
        char buffer[BUFFER_SIZE];
        read(pipe_impar[0], buffer, sizeof(buffer));

        int cant_a = cant_A(buffer);
        
        sprintf(buffer_res, "proc2: tiene %d A's \n", cant_a);

        write(pipe_res[1], buffer_res, sizeof(buffer));
    }
    close(pipe_impar[0]);
    close(pipe_res[1]);
}

void main() {
    pid_t par_impar_t;
    pid_t par_t;
    pid_t impar_t;
    
    if (pipe(pipe_par) == -1 || pipe(pipe_impar) == -1 || pipe(pipe_res) == -1) {
        perror("Error al crear la pipe");
        exit(-1);
    }

    FILE *file;
    file = fopen("pares.txt", "r");

    if (file == NULL) {
		perror("Error al crear archivo");
        exit(-1);
	}

    par_t = fork();

    if (par_t == 0) { // proceso hijo para lineas pares
        pares();
        exit(0);
    } else if (par_t < 0) {
        perror("Error al crear el proceso hijo");
        exit(-1);
    }

    impar_t = fork();

    if (impar_t == 0) { // proceso hijo para lineas impares
        impares();
        exit(0);
    } else if (impar_t < 0) {
        perror("Error al crear el proceso hijo");
        exit(-1);
    }

    par_impar_t = fork();

    if (par_impar_t == 0) { // proceso para lineas pares e impares
        par_impar(file);
        exit(0);
    } else if (par_impar_t < 0) {
        perror("Error al crear el proceso hijo");
        exit(-1);
    } else {
        close(pipe_res[1]);
        char buffer_res[BUFFER_SIZE];
        for (int i = 0; i < ITER; i++) {
            read(pipe_res[0], buffer_res, sizeof(buffer_res));
            printf("RESULTADO: %s\n", buffer_res);
            printf(buffer_res);
        }
        close(pipe_res[0]);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    fclose(file);
}