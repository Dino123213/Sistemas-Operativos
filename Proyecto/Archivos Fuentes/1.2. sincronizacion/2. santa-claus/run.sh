#!/bin/bash

# Se compila el archivo main.c en un ejecutable llamado main
gcc -Wall main.c -o main -lpthread

# Se verifica si la compilación fue exitosa
if [ $? -eq 0 ]; then
    # Ejecutar el ejecutable main
    ./main
else
    echo "Error en la compilación de main.c"
fi
