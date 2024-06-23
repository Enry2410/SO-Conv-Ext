//Estructura y funciones relacionadas con las reservas

#ifndef RESERVA_H
#define RESERVA_H

#define MAX_CLIENTE 50
#define MAX_FECHA 10

typedef struct {
    int id;
    char cliente[MAX_CLIENTE];
    char fecha_inicio[MAX_FECHA];
    char fecha_fin[MAX_FECHA];
    int habitacion;
} Reserva;

typedef struct Nodo {
    Reserva reserva;
    struct Nodo* siguiente;
} Nodo;

Nodo* crear_reserva(int id, const char* cliente, const char* fecha_inicio, const char* fecha_fin, int habitacion);
void agregar_reserva(Nodo** head, Nodo* nueva_reserva);
void modificar_reserva(Nodo* head, int id, const char* nuevo_cliente, const char* nueva_fecha_inicio, const char* nueva_fecha_fin, int nueva_habitacion);
void eliminar_reserva(Nodo** head, int id);
Nodo* buscar_reserva(Nodo* head, int id);
void listar_reservas(Nodo* head);

#endif