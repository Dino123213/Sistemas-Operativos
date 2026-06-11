# Nombre del compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g

# Nombre del compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g

# Archivos fuente
SRC_TAXI = taxi.c
SRC_PASAJEROS = pasajeros.c
SRC_AUTO = auto.c
SRC_COMUN = estructura.h

# Archivos objeto generados
OBJ_TAXI = taxi.o
OBJ_PASAJEROS = pasajeros.o
OBJ_AUTO = auto.o

# Nombre de los ejecutables
# Nombre del compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g

# Archivos fuente
SRC_AUTO = auto.c
SRC_PASAJERO = pasajero.c  # Asegúrate de que sea 'pasajero.c'
SRC_MAIN = main.c
SRC_COMUN = estructura.c

# Archivos objeto generados
OBJ_AUTO = auto.o
OBJ_PASAJERO = pasajero.o  # Asegúrate de que sea 'pasajero.o'
OBJ_MAIN = main.o
OBJ_COMUN = estructura.o

# Nombre del compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g

# Archivos fuente
SRC_TAXI = taxi.c
SRC_AUTO = auto.c
SRC_PASAJERO = pasajero.c

# Archivos objeto generados
OBJ_TAXI = taxi.o
OBJ_AUTO = auto.o
OBJ_PASAJERO = pasajero.o

# Nombre de los ejecutables
EXEC_TAXI = taxi
EXEC_AUTO = auto
EXEC_PASAJERO = pasajero

# Genera todos los ejecutables
all: $(EXEC_TAXI) $(EXEC_AUTO) $(EXEC_PASAJERO)

# Compilar y enlazar el programa taxi
$(EXEC_TAXI): $(OBJ_TAXI)
	$(CC) $(CFLAGS) -o $(EXEC_TAXI) $(OBJ_TAXI)

# Compilar y enlazar el programa auto
$(EXEC_AUTO): $(OBJ_AUTO)
	$(CC) $(CFLAGS) -o $(EXEC_AUTO) $(OBJ_AUTO)

# Compilar y enlazar el programa pasajero
$(EXEC_PASAJERO): $(OBJ_PASAJERO)
	$(CC) $(CFLAGS) -o $(EXEC_PASAJERO) $(OBJ_PASAJERO)

# Generar los archivos objeto
%.o: %.c estructura.h
	$(CC) $(CFLAGS) -c $<

# Limpiar archivos objeto y ejecutables
clean:
	rm -f *.o $(EXEC_TAXI) $(EXEC_AUTO) $(EXEC_PASAJERO)
