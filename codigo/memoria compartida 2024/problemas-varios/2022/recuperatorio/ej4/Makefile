CC = gcc
CC = gcc
CFLAGS = -Wall -pthread

# Objetivos de la compilación
all: main prod cons

# Compilar el programa principal
main: main.o
	$(CC) $(CFLAGS) -o main main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Compilar el programa Prod
prod: Prod.o
	$(CC) $(CFLAGS) -o prod Prod.o

Prod.o: Prod.c PC.h
	$(CC) $(CFLAGS) -c Prod.c

# Compilar el programa Cons
cons: Cons.o
	$(CC) $(CFLAGS) -o cons Cons.o

Cons.o: Cons.c PC.h
	$(CC) $(CFLAGS) -c Cons.c

# Limpiar archivos objeto y ejecutables
clean:
	rm -f *.o main prod cons
