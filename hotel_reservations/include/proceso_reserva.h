#ifndef PROCESO_RESERVA_H
#define PROCESO_RESERVA_H

#include "shared_data.h"

// Función para manejar una solicitud de reserva.
void manejar_solicitud_reserva(Nodo** reservas, int id, const char* cliente, const char* fecha_inicio, const char* fecha_fin, int habitacion);

// Función para crear procesos y manejar solicitudes de reservas.
void crear_procesos_para_reservas(SharedData* shared_data);

#endif
