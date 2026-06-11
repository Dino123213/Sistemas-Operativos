#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define CELESTE "\x1b[1;36m" 
#define AMARILLO "\x1b[33m"
#define RESET "\x1b[0m"       

#define MAX_CHAR 256

void ejecutarComando(char *comando, char *args[2]); 
void limpiarBuffer(const char* str);

int main() {
    int salir = 0;

    char entrada[MAX_CHAR]; 
    char *comando; 
    char *args[2]; 

    const char* usuario = getenv("USER"); 

    printf(AMARILLO "\n --------------- Minishell - Comision 16 ---------------\n\n" RESET);

    while (!salir) {
        printf(CELESTE "%s :> " RESET, usuario);
        fgets(entrada, sizeof(entrada), stdin);

        if (strlen(entrada) == MAX_CHAR + 1 && entrada[MAX_CHAR] != '\n') {
            printf("ERROR: El comando ingresado es demasiado largo. Caracteres permitidos: %d\n", MAX_CHAR);
            limpiarBuffer(entrada);
            continue;
        }
        entrada[strlen(entrada) - 1] = '\0'; // Remueve salto de línea al final del comando

        if (strlen(entrada) == 0) continue; // Caso donde se ingresa solo un Enter

        comando = strtok(entrada, " ");
        args[0] = strtok(NULL, " ");
        args[1] = strtok(NULL, " ");

        if (comando == NULL) {
            continue;
        }

        if ((strcmp(comando, "exit") == 0) && (args[0] == NULL)) {
            break;
        }

        ejecutarComando(comando, args);
    }

    printf(AMARILLO "\n --------------- Sesión finalizada. ---------------\n" RESET);

    return 0;
}

void ejecutarComando(char *comando, char *args[2]) {
    char ruta_comando[256];
    snprintf(ruta_comando, sizeof(ruta_comando), "./comandos/%s", comando);
    printf("\nEjecutando comando: %s\n", ruta_comando);

    char *full_arg[] = {ruta_comando, args[0], args[1]};

    pid_t pid = fork();

    if (pid == 0) {
        if (execv(ruta_comando, full_arg) == -1) {
            perror("Error al ejecutar el comando.");
            exit(1);
        }
    } else if (pid == -1) {
        perror("Error al crear el proceso hijo.");
        exit(1);
    }

    waitpid(pid, NULL, 0);
}

void limpiarBuffer(const char* str){
    if (str[strlen(str) - 1] != '\n') {
        while (getchar() != '\n');
    }
}