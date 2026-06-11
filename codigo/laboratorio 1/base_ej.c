#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <ctype.h>  //para tolower()


#define NUM_PALABRAS 9   // cantidad de palabras a buscar

struct mensaje {
    long tipo; //tipo de mensaje
    char palabra[20]; //array para indicar que palabra encontré
    int iden_proceso; //ID del proceso que envía el mensaje
};


//función para convertir una cadena a minúsculas
void convertir_a_minusculas(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo.txt>\n", argv[0]);
        return 1;
    }

    // Palabras a buscar
    const char *palabras[NUM_PALABRAS] = {
        "milonga",
        "canción",
        "semilla",
        "canilla",
        "tango",
        "barrio",
        "popular",
        "ciudad",
        "magia"
    };

    //Sis2025op
    //tengo que crear procesos hijos que lean un texto y le comuniquen al padre que palabras encontro
    
    //tenemos que crear una cola de mensajes
    key_t clave = ftok(argv[1], 65); //generar una clave usando el archivo pasado como parámetro
    if (clave == -1) {
        perror("Error al generar clave");
        exit(1);
    }

    int msgid = msgget(clave, 0666 | IPC_CREAT); //obtengo la id de la cola de mensajes
    if (msgid == -1) {
        perror("Error al crear cola de mensajes");
        exit(1);
    }

    //struc del mensaje
    struct mensaje msg;

    //crear un hijo por cada palabra a buscar
    for(int i = 0; i < NUM_PALABRAS; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error al crear proceso hijo");
            exit(1);

        } else if (pid == 0) {
            //proceso hijo

            FILE *archivo = fopen(argv[1], "r"); //abro el archivo
            if (!archivo) {
                perror("Error al abrir el archivo");
                exit(1);
            }

            //tengo que recorrer el archivo y encontrar la palabra elegida
            char linea[256];
            while (fgets(linea, sizeof(linea), archivo)) { //leo linea por linea el archivo
                convertir_a_minusculas(linea);
                if (strstr(linea, palabras[i]) != NULL) { //si encuentro la palabra (sin importar mayúsculas/minúsculas)
                    msg.tipo = 1L; //tipo de mensaje
                    strcpy(msg.palabra, palabras[i]); //indico que se encontró la palabra
                    msg.iden_proceso = getpid(); //id del proceso hijo

                    //envio el mensaje
                    if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                        perror("Error al enviar mensaje");
                        fclose(archivo);
                        exit(1);
                    }

                }
                //sigo recorriendo por si la palabra aparece más de una vez
            }
            fclose(archivo); //termino de leer el archivo entonces lo cierro
            exit(0); //termino el proceso hijo
        }
    }

    //el padre tiene que quedarse activo esperando los mensajes de los hijos
    
    //espero a que todos los hijos terminen
    for (int i = 0; i < NUM_PALABRAS; i++) {
        wait(NULL); //espero a que terminen los hijos sin guardar el status
    }

    int cant_encontradas = 0; //el padre va a contar cuantas palabras se encontraron
    
    //recibo los mensajes (solo los que fueron enviados)
    while (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1L, IPC_NOWAIT) != -1) {
        cant_encontradas++;
        printf("El proceso con ID %d encontró la palabra: %s\n", msg.iden_proceso, msg.palabra);
    }
    
    printf("Total de palabras encontradas: %d\n", cant_encontradas); //imprimos por pantalla cuantas palabras se encontraron
    
    //eliminar la cola de mensajes
    msgctl(msgid, IPC_RMID, NULL);
    
    return 0;
}