# Makefile para el sistema Lectores-Escritores Modular

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lpthread

# Archivos objeto
TARGETS = main escritor lector

# Regla principal
all: $(TARGETS)

# Compilar main
main: main.c semaforos.h
	$(CC) $(CFLAGS) -o main main.c $(LDFLAGS)

# Compilar escritor
escritor: escritor.c semaforos.h
	$(CC) $(CFLAGS) -o escritor escritor.c $(LDFLAGS)

# Compilar lector
lector: lector.c semaforos.h
	$(CC) $(CFLAGS) -o lector lector.c $(LDFLAGS)

# Limpiar archivos compilados
clean:
	rm -f $(TARGETS)

# Ejecutar el programa
run: all
	./main

# Mostrar ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make all    - Compilar todos los archivos"
	@echo "  make main   - Compilar solo el controlador principal"
	@echo "  make escritor - Compilar solo el programa escritor"
	@echo "  make lector - Compilar solo el programa lector"
	@echo "  make run    - Compilar y ejecutar el sistema completo"
	@echo "  make clean  - Limpiar archivos compilados"
	@echo "  make help   - Mostrar esta ayuda"

.PHONY: all clean run help