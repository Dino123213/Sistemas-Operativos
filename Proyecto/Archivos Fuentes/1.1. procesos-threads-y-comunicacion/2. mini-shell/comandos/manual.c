#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ROJO "\x1b[31m"
#define RESET "\x1b[0m" 

void mostrar_comandos() {
    printf(ROJO " --------------------+ Manual de Comandos +-------------------- \n" RESET);
    printf("1 - crdir [ruta/nombre]: Crea un directorio con el nombre especificado en la ruta\n");
    printf("2 - rmdir [ruta/nombre]: Elimina el directorio especificado en la ruta\n");
    printf("3 - crfile [ruta/nombre]: Crea un archivo de texto con el nombre especificado en la ruta\n");
    printf("4 - list [ruta/nombre]: Lista el contenido del directorio correspondiente al nombre especificado en la ruta\n");
    printf("5 - show [ruta/nombre.txt]: Muestra el contenido del archivo correspondiente al nombre especificado en la ruta\n");
    printf("6 - modp [ruta/nombre]: Modifica los permisos del archivo correspondiente al nombre especificado en la ruta\n");
    printf("7 - manual: Muestra los comandos implementados en la shell\n");
    printf("8 - exit: Cierra la shell\n");
    printf(ROJO " -------------------------------------------------------------- \n" RESET);
}

void ayuda_crdir() {
    printf(ROJO " ══════════ Uso del comando CRDIR  ══════════\n" RESET);
    printf("Este comando crea uno o más directorios con los nombres especificados.\n");
    printf("Uso: crdir 'ruta/nombre' ...\n");
    printf("Para crear más de un directorio, usa: 'crdir ruta1/nombre1 ruta2/nombre2 ...'.\n");
    printf(ROJO " ═════════════════════════════════════════════════\n" RESET);
}

void ayuda_rmdir() {
    printf(ROJO " ══════════ Uso del comando RMDIR ══════════\n" RESET);
    printf("Este comando elimina uno o más directorios con los nombres especificados.\n");
    printf("Uso: rmdir 'ruta/nombre' ...\n");
    printf("Para eliminar más de un directorio: 'rmdir ruta1/nombre1 ruta2/nombre2 ...'.\n");
    printf(ROJO " ═════════════════════════════════════════════════\n" RESET);
}

void ayuda_crfile() {
    printf(ROJO " ══════════ Uso del comando CRFILE ══════════\n" RESET);
    printf("Este comando crea uno o más archivos con los nombres especificados.\n");
    printf("Uso: crfile 'ruta/nombre' ...\n");
    printf("Para crear más de un archivo: 'crfile ruta1/nombre1 ruta2/nombre2 ...'.\n");
    printf(ROJO " ═════════════════════════════════════════════════\n" RESET);
}

void ayuda_list() {
    printf(ROJO " ══════════ Uso del comando LIST ══════════\n" RESET);
    printf("Este comando lista el contenido de uno o más directorios especificados.\n");
    printf("Uso: list 'ruta/nombre' ...\n");
    printf("Para listar más de un directorio: 'list ruta1/nombre1 ruta2/nombre2 ...'.\n");
    printf(ROJO " ═════════════════════════════════════════════════\n" RESET);
}

void ayuda_show() {
    printf(ROJO " ══════════ Uso del comando SHOW ══════════\n" RESET);
    printf("Este comando muestra el contenido de uno o más archivos de texto especificados.\n");
    printf("Uso: show 'ruta/nombre.txt' ...\n");
    printf("Para mostrar más de un archivo: 'show ruta1/nombre1.txt ruta2/nombre2.txt ...'.\n");
    printf(ROJO " ═════════════════════════════════════════════════\n" RESET);
}

void ayuda_modp() {
      printf(ROJO " ══════════ Uso del comando MODP ══════════\n" RESET);
    printf("Este comando modifica los permisos del archivo especificado.\n");
    printf("Uso: modp 'ruta/nombre' ...\n");
    printf("Permisos:\n");
    printf("0: Ningún permiso\n");
    printf("1: Ejecución\n");
    printf("2: Escritura\n");
    printf("3: Escritura y Ejecución\n");
    printf("4: Lectura\n");
    printf("5: Lectura y Ejecución\n");
    printf("6: Lectura y Escritura\n");
    printf("7: Lectura, Escritura y Ejecución\n");
    printf("\nLas combinaciones de permisos se deben ingresar en formato UGO (Usuario, Grupo, Otros), por ejemplo:\n");
    printf("  '751' asigna:\n");
    printf("    - 7 (Lectura, Escritura y Ejecución) para el Usuario (U)\n");
    printf("    - 5 (Lectura y Ejecución) para el Grupo (G)\n");
    printf("    - 1 (Ejecutar) para Otros (O)\n");
    printf("\nNota: El comando puede aceptar múltiples archivos si se especifican como parámetros.\n");
    printf(ROJO " ═════════════════════════════════════════════════\n" RESET);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        mostrar_comandos();
    } else if (!strcmp(argv[1], "crdir")) {
        ayuda_crdir();
    } else if (!strcmp(argv[1], "rmdir")) {
        ayuda_rmdir();
    } else if (!strcmp(argv[1], "crfile")) {
        ayuda_crfile();
    } else if (!strcmp(argv[1], "list")) {
        ayuda_list();
    } else if (!strcmp(argv[1], "show")) {
        ayuda_show();
    } else if (!strcmp(argv[1], "modp")) {
        ayuda_modp();
    } else {
        printf("No se reconoce el comando.\n");
    }

    return 0;
}
