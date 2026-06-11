#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("ERROR: Se deben ingresar al menos dos argumentos (nombre de archivo y permisos).\n");
        return 1;
    }

    char* nombre = argv[1];
    char* modo = argv[2];

    int permisos = strtol(modo, NULL, 10);

    if (permisos < 1 || permisos > 777) {
        printf("ERROR: Los permisos deben ser un valor entre 001 y 777.\n");
        return 1;
    }

    if (strpbrk(modo, "89") != NULL) {
        printf("ERROR: Los permisos contienen caracteres inválidos ('8' o '9').\n");
        return 1;
    }

    if (chmod(nombre, permisos) == 0) {
        printf("\nSe han modificado los permisos correctamente.\n");
    } else {
        printf("\nERROR: El archivo no es válido o no se puede modificar.\n");
    }

    return 0;
}