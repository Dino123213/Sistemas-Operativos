#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUEUE 10

// Estructura para mensajes
struct msg_buffer {
    long tipo;
    char texto[100];
};

// Cola de mensajes compartida entre hilos (sin protección)
struct {
    struct msg_buffer mensajes[MAX_QUEUE];
    int frente;      // Índice de lectura
    int final;       // Índice de escritura
    int count;       // Cantidad de mensajes
} cola_mensajes;

// Inicializar la cola
void init_cola() {
    cola_mensajes.frente = 0;
    cola_mensajes.final = 0;
    cola_mensajes.count = 0;
}

// Enviar mensaje (sin protección)
void enviar_mensaje(struct msg_buffer *mensaje) {
    if (cola_mensajes.count < MAX_QUEUE) {
        cola_mensajes.mensajes[cola_mensajes.final] = *mensaje;
        cola_mensajes.final = (cola_mensajes.final + 1) % MAX_QUEUE;
        cola_mensajes.count++;
    }
}

// Recibir mensaje (sin protección)
int recibir_mensaje(struct msg_buffer *mensaje) {
    if (cola_mensajes.count > 0) {
        *mensaje = cola_mensajes.mensajes[cola_mensajes.frente];
        cola_mensajes.frente = (cola_mensajes.frente + 1) % MAX_QUEUE;
        cola_mensajes.count--;
        return 1;  // Éxito
    }
    return 0;  // No hay mensajes
}

// Hilo A
void* hiloA(void* arg) {
    struct msg_buffer mensaje;
    
    // Esperar activamente hasta recibir mensaje
    while (recibir_mensaje(&mensaje) == 0) {
        usleep(1000);  // Esperar 1ms
    }
    
    printf("[HiloA] Recibido: tipo=%ld, texto=%s\n", mensaje.tipo, mensaje.texto);
    
    // Modificar y reenviar
    mensaje.tipo = 2;
    strcpy(mensaje.texto, "Respuesta desde HiloA");
    
    enviar_mensaje(&mensaje);
    printf("[HiloA] Mensaje enviado de vuelta\n");
    
    pthread_exit(NULL);
}

int main() {
    pthread_t tidA;
    struct msg_buffer mensaje;
    
    // Inicializar cola de mensajes
    init_cola();
    
    // Crear hilo A
    if (pthread_create(&tidA, NULL, hiloA, NULL) != 0) {
        perror("Error creando hilo A");
        exit(1);
    }
    
    // Hilo principal
    printf("[Main] Enviando mensaje...\n");
    
    // Crear y enviar mensaje
    strcpy(mensaje.texto, "Hola desde el hilo principal");
    mensaje.tipo = 1;
    
    enviar_mensaje(&mensaje);
    printf("[Main] Mensaje enviado: %s\n", mensaje.texto);
    
    // Esperar activamente a recibir respuesta
    while (recibir_mensaje(&mensaje) == 0) {
        usleep(1000);  // Esperar 1ms
    }
    
    printf("[Main] Recibido respuesta: tipo=%ld, texto=%s\n", mensaje.tipo, mensaje.texto);
    
    // Esperar a que termine el hilo A
    pthread_join(tidA, NULL);
    
    printf("[Main] Programa terminado\n");
    
    return 0;
}