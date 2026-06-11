# Sistema Lectores-Escritores Modular

Este proyecto implementa el problema clásico de lectores-escritores usando procesos separados y memoria compartida con semáforos POSIX.

## Estructura del proyecto

```
LEProcesos_Modular/
├── semaforos.h     # Definiciones compartidas y estructura de semáforos
├── main.c          # Controlador principal que crea los procesos
├── escritor.c      # Programa para procesos escritores
├── lector.c        # Programa para procesos lectores
├── Makefile        # Script de compilación
└── README.md       # Este archivo
```

## Funcionamiento

- **main.c**: Inicializa la memoria compartida, crea los semáforos y lanza los procesos escritores y lectores usando `execl()`.
- **escritor.c**: Programa independiente que implementa la lógica de escritor.
- **lector.c**: Programa independiente que implementa la lógica de lector.
- **semaforos.h**: Define la estructura de semáforos y constantes compartidas.

## Compilación y ejecución

### Compilar todo:
```bash
make all
```

### Ejecutar el sistema completo:
```bash
make run
```

### Compilar por separado:
```bash
make main      # Solo el controlador
make escritor  # Solo el programa escritor
make lector    # Solo el programa lector
```

### Limpiar archivos compilados:
```bash
make clean
```

## Parámetros configurables

En `semaforos.h` puedes modificar:
- `ESCRITORES`: Número de procesos escritores (default: 3)
- `LECTORES`: Número de procesos lectores (default: 3)  
- `ITER`: Iteraciones por proceso (default: 10)

## Funcionamiento de los semáforos

1. **wsem**: Semáforo binario (inicial=1) que controla el acceso exclusivo de escritores
2. **rmutex**: Mutex (inicial=1) que protege el contador de lectores
3. **rsem**: Contador (inicial=0) que lleva la cuenta de lectores activos

## Protocolo

### Escritores:
- Esperan acceso exclusivo (`sem_wait(wsem)`)
- Escriben durante 2 segundos
- Liberan acceso (`sem_post(wsem)`)

### Lectores:
- Primer lector bloquea escritores
- Múltiples lectores pueden leer simultáneamente
- Último lector desbloquea escritores

## Archivos necesarios

El programa necesita que exista el archivo `LEArchivo.c` en el directorio para generar la clave de memoria compartida con `ftok()`.