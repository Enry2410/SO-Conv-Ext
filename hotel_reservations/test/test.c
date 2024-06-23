#include "reserva.h"
#include "shared_data.h"
#include "hilos.h"
#include "proceso_reserva.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

int test_main() {
    SharedData* shared_data = init_shared_data();

    int pipe_consulta_fd[2], pipe_actualiza_fd[2];

    pipe(pipe_consulta_fd);
    pipe(pipe_actualiza_fd);

    // Load reservations from file
    shared_data->reservas = cargar_reservas();

    int opcion;
    printf("Seleccione el modo de operación:\n");
    printf("1. Usar hilos\n");
    printf("2. Usar procesos\n");
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        // Create threads for consultations and updates
        pthread_t hilo1, hilo2;
        pthread_create(&hilo1, NULL, consultar_reservas_hilo, (void*)shared_data);

        Reserva reserva_actualizar = {1, "Nuevo Cliente Hilo", "2024-06-25", "2024-06-30", 102};
        void* args[] = {shared_data, &reserva_actualizar};
        pthread_create(&hilo2, NULL, actualizar_reservas_hilo, (void*)args);

        // Wait for threads to finish
        pthread_join(hilo1, NULL);
        pthread_join(hilo2, NULL);
    } else if (opcion == 2) {
        // Implementation with processes and pipes
        consultar_reservas_proceso(shared_data, pipe_consulta_fd);

        Reserva reserva_actualizar = {1, "Nuevo Cliente Proceso", "2024-06-25", "2024-06-30", 102};
        actualizar_reserva_proceso(shared_data, &reserva_actualizar, pipe_actualiza_fd);
    } else {
        printf("Opción inválida.\n");
    }

    // Save reservations to file
    guardar_reservas(shared_data->reservas);

    // Clean up shared data
    destroy_shared_data(shared_data);

    return 0;
}