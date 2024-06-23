#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
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
    printf("3. Probar funcionalidades\n");
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
        consultar_reservas_proceso(shared_data, pipe_consulta_fd);

        Reserva reserva_actualizar = {1, "Nuevo Cliente Proceso", "2024-06-25", "2024-06-30", 102};
        actualizar_reserva_proceso(shared_data, &reserva_actualizar, pipe_actualiza_fd);
    } else if (opcion == 3){
        void test_crear_reserva();
        printf("\n Función de crear exito\n");
        void test_modificar_reserva();
        printf("\n Funcion de modificar exito\n");
        void test_eliminar_reserva();
        printf("\n Función de agregar exito\n");
        printf("\n Funcion eliminar exito\n");

    } else {
        printf("Opción inválida.\n");
    }

    // Guardar reservas en el archivo
    guardar_reservas(shared_data->reservas);

    // Limpiar recursos compartidos
    destroy_shared_data(shared_data);

    return 0;
}

void test_crear_reserva(){
    Nodo* reserva = crear_reserva(1, "Juan Perez", "24/10/2020", "31/10/2020", 113);
    assert(reserva != NULL);
    assert(reserva->reserva.id == 1);
    assert(strcmp(reserva->reserva.cliente, "Juan Perez")==0);
    free(reserva);
}

void test_modificar_reserva(){
    Nodo* reserva = crear_reserva(1, "Juan Perez", "24/10/2020", "31/10/2020", 113);
    modificar_reserva(reserva, 1, "Juan Marquez", "24/10/2020", "31/10/2020", 114);
    assert(reserva != NULL);
    assert(reserva->reserva.id == 1);
    assert(strcmp(reserva->reserva.cliente, "Juan Marquez")==0);
    assert(reserva->reserva.habitacion == 114);
    free(reserva);
}

void test_eliminar_reserva(){
    Nodo* reserva = crear_reserva(1, "Juan Perez", "24/10/2020", "31/10/2020", 113);
    Nodo* nuevaReserva = crear_reserva(2, "Juan Marquez", "24/10/2020", "31/10/2020", 114);
    agregar_reserva(&reserva, nuevaReserva);
    
    
    eliminar_reserva(&reserva, 2);
    Nodo* compara = buscar_reserva(reserva, 2);
    assert(compara == NULL);
    while (reserva!= NULL) {

        Nodo* next = reserva->siguiente;

        free(reserva);

        reserva = next;
    }
    free(nuevaReserva);
    free(compara);
}