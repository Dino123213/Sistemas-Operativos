#!/bin/bash

gcc -o codigo1 codigo1.c
gcc -o codigo2 codigo2.c 

# Ejecutar codigo1 en segundo plano
./codigo1 &

# Guardar el PID del proceso en una variable
PID=$!

# Esperar un poco para asegurarse que el proceso se haya ejecutado
sleep 2

echo
echo "Árbol de procesos:"
pstree -p $PID
