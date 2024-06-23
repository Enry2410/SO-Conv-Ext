//Implementación de las funciones CRUD para reseervas
#include "reserva.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Nodo* crear_reserva(int id, const char* cliente, const char* fecha_inicio, const char* fecha_fin, int habitacion) {
    Nodo* nueva_reserva = (Nodo*)malloc(sizeof(Nodo));
    nueva_reserva->reserva.id = id;
    strncpy(nueva_reserva->reserva.cliente, cliente, sizeof(nueva_reserva->reserva.cliente));
    strncpy(nueva_reserva->reserva.fecha_inicio, fecha_inicio, sizeof(nueva_reserva->reserva.fecha_inicio));
    strncpy(nueva_reserva->reserva.fecha_fin, fecha_fin, sizeof(nueva_reserva->reserva.fecha_fin));
    nueva_reserva->reserva.habitacion = habitacion;
    nueva_reserva->siguiente = NULL;
    return nueva_reserva;
}

void agregar_reserva(Nodo** head, Nodo* nueva_reserva) {
    nueva_reserva->siguiente = *head;
    *head = nueva_reserva;
}

void modificar_reserva(Nodo* head, int id, const char* nuevo_cliente, const char* nueva_fecha_inicio, const char* nueva_fecha_fin, int nueva_habitacion) {
    Nodo* reserva = buscar_reserva(head, id);
    if (reserva) {
        strncpy(reserva->reserva.cliente, nuevo_cliente, sizeof(reserva->reserva.cliente));
        strncpy(reserva->reserva.fecha_inicio, nueva_fecha_inicio, sizeof(reserva->reserva.fecha_inicio));
        strncpy(reserva->reserva.fecha_fin, nueva_fecha_fin, sizeof(reserva->reserva.fecha_fin));
        reserva->reserva.habitacion = nueva_habitacion;
    }
}

void eliminar_reserva(Nodo** head, int id) {
    Nodo* actual = *head;
    Nodo* previo = NULL;

    while (actual != NULL && actual->reserva.id != id) {
        previo = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        return;
    }

    if (previo == NULL) {
        *head = actual->siguiente;
    } else {
        previo->siguiente = actual->siguiente;
    }

    free(actual);
}

Nodo* buscar_reserva(Nodo* head, int id) {
    while (head != NULL) {
        if (head->reserva.id == id) {
            return head;
        }
        head = head->siguiente;
    }
    return NULL;
}

void listar_reservas(Nodo* head) {
    while (head != NULL) {
        printf("ID: %d, Cliente: %s, Fecha Inicio: %s, Fecha Fin: %s, Habitacion: %d\n",
               head->reserva.id, head->reserva.cliente, head->reserva.fecha_inicio, head->reserva.fecha_fin, head->reserva.habitacion);
        head = head->siguiente;
    }
}
