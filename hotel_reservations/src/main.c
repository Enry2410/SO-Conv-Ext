#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "reserva.h"
#include "shared_data.h"
#include "proceso_reserva.h"
#include "hilos.h"

int main() {
    SharedData* shared_data = init_shared_data();

    int pipe_consulta_fd[2], pipe_actualiza_fd[2];

    // Cargar reservas desde el archivo
    shared_data->reservas = cargar_reservas();

    int opcion;
    printf("Seleccione el modo de operación:\n");
    printf("1. Usar hilos\n");
    printf("2. Usar procesos\n");
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        // Crear hilos para consultas y actualizaciones
        pthread_t hilo1, hilo2;
        pthread_create(&hilo1, NULL, consultar_reservas_hilo, (void*)shared_data);

        Reserva reserva_actualizar = {1, "Nuevo Cliente Hilo", "2024-06-25", "2024-06-30", 102};
        void* args[] = {shared_data, &reserva_actualizar};
        pthread_create(&hilo2, NULL, actualizar_reservas_hilo, (void*)args);

        // Esperar a que los hilos terminen
        pthread_join(hilo1, NULL);
        pthread_join(hilo2, NULL);
    } else if (opcion == 2) {
        // Implementación con procesos y pipes
        consultar_reservas_proceso_pipe(shared_data, pipe_consulta_fd);

        Reserva reserva_actualizar = {1, "Nuevo Cliente Proceso", "2024-06-25", "2024-06-30", 102};
        actualizar_reserva_proceso_pipe(shared_data, &reserva_actualizar, pipe_actualiza_fd);
    } else {
        printf("Opción inválida.\n");
    }

    // Guardar reservas en el archivo
    guardar_reservas(shared_data->reservas);

    // Limpiar recursos compartidos
    destroy_shared_data(shared_data);

    return 0;
}
