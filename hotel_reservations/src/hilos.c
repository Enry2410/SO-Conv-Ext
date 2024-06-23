#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "reserva.h"
#include "shared_data.h"

void* consultar_reservas(void* arg) {
    SharedData* shared_data = (SharedData*)arg;

    sem_wait(&shared_data->semaforo);
    Nodo* actual = shared_data->reservas;
    while (actual != NULL) {
        printf("Reserva ID: %d, Cliente: %s, Fecha Inicio: %s, Fecha Fin: %s, Habitación: %d\n",
               actual->reserva.id, actual->reserva.cliente, actual->reserva.fecha_inicio,
               actual->reserva.fecha_fin, actual->reserva.habitacion);
        actual = actual->siguiente;
    }
    sem_post(&shared_data->semaforo);

    return NULL;
}

void* actualizar_reserva(void* arg) {
    void** args = (void**)arg;
    SharedData* shared_data = (SharedData*)args[0];
    Reserva* nueva_reserva = (Reserva*)args[1];

    sem_wait(&shared_data->semaforo);
    Nodo* actual = shared_data->reservas;
    while (actual != NULL) {
        if (actual->reserva.id == nueva_reserva->id) {
            strcpy(actual->reserva.cliente, nueva_reserva->cliente);
            strcpy(actual->reserva.fecha_inicio, nueva_reserva->fecha_inicio);
            strcpy(actual->reserva.fecha_fin, nueva_reserva->fecha_fin);
            actual->reserva.habitacion = nueva_reserva->habitacion;
            break;
        }
        actual = actual->siguiente;
    }
    sem_post(&shared_data->semaforo);

    return NULL;
}
