// Función principal del programa

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "reserva.h"
#include "shared_data.h"
#include "proceso_reserva.h"

void crear_hilos_para_operaciones(SharedData* shared_data) {
    pthread_t hilo1, hilo2;
    Reserva reserva = {1, "Cliente Actualizado", "2024-06-05", "2024-06-15", 102};

    pthread_create(&hilo1, NULL, consultar_reservas, (void*)shared_data);
    void* args[2] = {shared_data, &reserva};
    pthread_create(&hilo2, NULL, actualizar_reserva, (void*)args);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
}

int main() {
    SharedData* shared_data = init_shared_data();

    // Cargar reservas desde el archivo
    shared_data->reservas = cargar_reservas();

    // Creación de procesos para reservas
    crear_procesos_para_reservas(shared_data);

    // Simulación de operaciones concurrentes con hilos
    crear_hilos_para_operaciones(shared_data);

    // Comunicación con pipes
    comunicacion_con_pipes(shared_data);

    // Comunicación con sockets
    comunicacion_con_sockets(shared_data);

    // Guardar reservas en el archivo
    guardar_reservas(shared_data->reservas);

    destroy_shared_data(shared_data);

    return 0;
}