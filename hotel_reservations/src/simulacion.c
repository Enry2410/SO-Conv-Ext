#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "reserva.h"
#include "shared_data.h"


void manejar_solicitud_reserva(Nodo** reservas, int id, const char* cliente, const char* fecha_inicio, const char* fecha_fin, int habitacion) {
    // Crear una nueva reserva y agregarla a la lista de reservas
    Nodo* nueva_reserva = crear_reserva(id, cliente, fecha_inicio, fecha_fin, habitacion);
    agregar_reserva(reservas, nueva_reserva);
}

int main() {
    SharedData* shared_data = init_shared_data();
    int num_solicitudes = 5;

    for (int i = 0; i < num_solicitudes; i++) {
        if (fork() == 0) {
            manejar_solicitud_reserva(&(shared_data->reservas), i + 1, "Cliente", "2024-06-01", "2024-06-10", 100 + i);
            exit(0);
        }
    }

    for (int i = 0; i < num_solicitudes; i++) {
        wait(NULL);
    }

    destroy_shared_data(shared_data);
    return 0;
}