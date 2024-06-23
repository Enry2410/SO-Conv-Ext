//Estructura y funciones relacionadas con las reservass

#ifndef RESERVA_H
#define RESERVA_H

#define MAX_CLIENTE 50
#define MAX_FECHA 10

// Estructura para contener datos "Reserva"
typedef struct {
    int id;
    char cliente[MAX_CLIENTE];
    char fecha_inicio[MAX_FECHA];
    char fecha_fin[MAX_FECHA];
    int habitacion;
} Reserva;

// Objeto con puntero para formar una lista enlazada
typedef struct Nodo {
    Reserva reserva;
    struct Nodo* siguiente;
} Nodo;

Nodo* crear_reserva(int id, const char* cliente, const char* fecha_inicio, // Funcion para crear reserva
            const char* fecha_fin, int habitacion);
void agregar_reserva(Nodo** head, Nodo* nueva_reserva); // Función para añadir una reserva a la lista
void modificar_reserva(Nodo* head, int id, const char* nuevo_cliente, 
            const char* nueva_fecha_inicio, const char* nueva_fecha_fin, int nueva_habitacion);
void eliminar_reserva(Nodo** head, int id); // Función para eliminar una reserva por su id
Nodo* buscar_reserva(Nodo* head, int id); // Función para consultar una reserva por su id
void listar_reservas(Nodo* head); //Función para mostrar todas las reservas

#endif