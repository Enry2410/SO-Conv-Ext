#ifndef SHARED_DATA_H
#define SHARED_DATA_H

//Estructuras y funciones relacionadas con la memoria compartida y los semaforos

#include <semaphore.h>
#include "reserva.h"

#define SHM_NAME "/hotel_reservas"

typedef struct {
    Nodo* head;
    sem_t sem;
} SharedData;

// Inicializar memoria compartida
SharedData* init_shared_data();

// Destruir memoria compartida
void destroy_shared_data(SharedData* shared_data);

// Comunicación con pipes
void comunicacion_con_pipes(SharedData* shared_data);

// Comunicación con sockets
void comunicacion_con_sockets(SharedData* shared_data);

// Guardar reservas en archivo
void guardar_reservas(Nodo* head);

// Cargar reservas desde archivo
Nodo* cargar_reservas();

#endif
