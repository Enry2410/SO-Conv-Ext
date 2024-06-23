#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "reserva.h"
#include "shared_data.h"
#include "proceso_reserva.h"

void* consultar_reservas_proceso(SharedData* shared_data, int pipe_fd[2]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo para consulta de reservas
        close(pipe_fd[0]); // Cerrar el extremo de lectura

        sem_wait(&shared_data->semaforo);
        Nodo* actual = shared_data->reservas;
        while (actual != NULL) {
            printf("Reserva ID: %d, Cliente: %s, Fecha Inicio: %s, Fecha Fin: %s, Habitacion: %d\n", 
                actual->reserva.id, actual->reserva.cliente, actual->reserva.fecha_inicio, 
                actual->reserva.fecha_fin, actual->reserva.habitacion);
            actual = actual->siguiente;
        }
        sem_post(&shared_data->semaforo);

        close(pipe_fd[1]); // Cerrar el extremo de escritura
        exit(0);
    } else if (pid > 0) {
        // Proceso padre
        close(pipe_fd[1]); // Cerrar el extremo de escritura

        // Esperar a que el proceso hijo termine
        wait(NULL);

        close(pipe_fd[0]); // Cerrar el extremo de lectura
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return NULL;
}

void* actualizar_reserva_proceso(SharedData* shared_data, Reserva* reserva, int pipe_fd[2]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo para actualización de reserva
        close(pipe_fd[0]); // Cerrar el extremo de lectura

        sem_wait(&shared_data->semaforo);
        modificar_reserva(shared_data->reservas, reserva->id, reserva->cliente, 
            reserva->fecha_inicio, reserva->fecha_fin, reserva->habitacion);
        sem_post(&shared_data->semaforo);

        close(pipe_fd[1]); // Cerrar el extremo de escritura
        exit(0);
    } else if (pid > 0) {
        // Proceso padre
        close(pipe_fd[1]); // Cerrar el extremo de escritura

        // Esperar a que el proceso hijo termine
        wait(NULL);

        close(pipe_fd[0]); // Cerrar el extremo de lectura
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return NULL;
}