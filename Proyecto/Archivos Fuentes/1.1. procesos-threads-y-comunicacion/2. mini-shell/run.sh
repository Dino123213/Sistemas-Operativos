#!/bin/bash

# Se cambia a la carpeta comandos
cd comandos

# Se compilan todos los archivos .c en la carpeta comandos
for archivo in *.c; do
    nombre=$(basename "$archivo" .c)
    echo "Compilando $archivo..."
    gcc -Wall "$archivo" -o "$nombre"
done

# Se vuelve a la carpeta principal /minishell
cd ..

# Se compila minishell.c
echo "Compilando minishell.c..."
gcc -Wall minishell.c -o minishell

# Se ejecuta el programa minishell
echo "Ejecutando minishell..."
./minishell
