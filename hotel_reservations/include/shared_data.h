//Estructuras y funciones relacionadas con la memoria compartida y los semaforos


#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <semaphore.h>
#include "reserva.h"

#define SHM_NAME "/hotel_reservas"

typedef struct {
    sem_t semaforo;
    Nodo* reservas;
} SharedData;

SharedData* init_shared_data();
void destroy_shared_data(SharedData* shared_data);
void guardar_reservas(Nodo* head);
Nodo* cargar_reservas();
void comunicacion_con_pipes(SharedData* shared_data);
void comunicacion_con_sockets(SharedData* shared_data);

#endif
