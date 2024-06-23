#ifndef PROCESO_RESERVA_H
#define PROCESO_RESERVA_H

#include "shared_data.h"

void* consultar_reservas_proceso(SharedData* shared_data, int* pipe_fd[2]);
void* actualizar_reserva_proceso(SharedData* shared_data, Reserva* reserva, int* pipe_fd[2]);

#endif
