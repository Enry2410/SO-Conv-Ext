//Añade reservas

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "reserva.h"
#include "shared_data.h"
#include "proceso_reserva.h"

void manejar_solicitud_reserva(Nodo** reservas, int id, const char* cliente, const char* fecha_inicio, const char* fecha_fin, int habitacion) {
    // Crear una nueva reserva y agregarla a la lista de reservas
    Nodo* nueva_reserva = crear_reserva(id, cliente, fecha_inicio, fecha_fin, habitacion);
    agregar_reserva(reservas, nueva_reserva);
}


void crear_procesos_para_reservas(SharedData* shared_data) {
    pid_t pid = fork();
    if (pid == 0) {
        sem_wait(&shared_data->semaforo);
        manejar_solicitud_reserva(&(shared_data->reservas), 1, "Cliente 1", "2024-06-01", "2024-06-10", 101);
        sem_post(&shared_data->semaforo);
        exit(0);
    } else if (pid > 0) {
        wait(NULL);
    }

    pid = fork();
    if (pid == 0) {
        sem_wait(&shared_data->semaforo);
        manejar_solicitud_reserva(&(shared_data->reservas), 2, "Cliente 2", "2024-06-05", "2024-06-15", 102);
        sem_post(&shared_data->semaforo);
        exit(0);
    } else if (pid > 0) {
        wait(NULL);
    }
}
