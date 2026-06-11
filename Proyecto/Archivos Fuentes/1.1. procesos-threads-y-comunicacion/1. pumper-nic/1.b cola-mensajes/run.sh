#!/bin/bash

# Se verifica si el directorio "ejecutables" existe, si no, lo crea
mkdir -p ejecutables

# Se compila cada archivo .c en un ejecutable en el directorio "ejecutables"
for file in *.c; do
    gcc -Wall "$file" -o "ejecutables/${file%.c}"
done

# Se ejectuta el ejecutable "main" desde el directorio "ejecutables"
./ejecutables/main

