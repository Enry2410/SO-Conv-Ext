// Implementacion de las funciones relacionadas con la memoria compartida

#include "shared_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SHM_SIZE sizeof(SharedData)
#define PORT 8080

SharedData* init_shared_data() {
    key_t key = IPC_PRIVATE;
    int shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }
    SharedData* shared_data = (SharedData*)shmat(shm_id, NULL, 0);
    if (shared_data == (SharedData*)-1) {
        perror("shmat");
        exit(1);
    }
    sem_init(&shared_data->semaforo, 1, 1);
    shared_data->reservas = NULL; // Iniciar la lista de reservas como NULL
    return shared_data;
}

void destroy_shared_data(SharedData* shared_data) {
    // Liberar la lista de reservas
    Nodo* actual = shared_data->reservas;
    while (actual != NULL) {
        Nodo* temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    sem_destroy(&shared_data->semaforo);
    shmdt(shared_data);
}

void comunicacion_con_pipes(SharedData* shared_data) {
    int pipe_fd[2];
    pipe(pipe_fd);
    pid_t pid = fork();

    if (pid == 0) {
        // Proceso hijo
        close(pipe_fd[1]); // Cerrar  escritura

        char buffer[256];
        read(pipe_fd[0], buffer, sizeof(buffer));
        printf("Mensaje recibido en el proceso hijo: %s\n", buffer);

        close(pipe_fd[0]); // Cerrar  lectura
        exit(0);
    } else {
        // Proceso padre
        close(pipe_fd[0]); // Cerrar  lectura

        const char* mensaje = "Mensaje mandado por el proceso padre";
        write(pipe_fd[1], mensaje, strlen(mensaje) + 1);

        close(pipe_fd[1]); // Cerrar  escritura
        wait(NULL); // Esperar a que el proceso hijo termine
    }
}

void comunicacion_con_sockets(SharedData* shared_data) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hola desde el servidor";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    read(new_socket, buffer, 1024);
    printf("Mensaje recibido: %s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Mensaje enviado: %s\n", hello);

    close(new_socket);
    close(server_fd);
}

void guardar_reservas(Nodo* head) {
    FILE* file = fopen("reservas.txt", "w");
    if (file == NULL) {
        perror("Error al abrir el archivo para guardar");
        exit(EXIT_FAILURE);
    }
    while (head) {
        fprintf(file, "%d,%s,%s,%s,%d\n", head->reserva.id, head->reserva.cliente, 
            head->reserva.fecha_inicio, head->reserva.fecha_fin, head->reserva.habitacion);
        head = head->siguiente;
    }
    fclose(file);
}

Nodo* cargar_reservas() {
    FILE* file = fopen("reservas.txt", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo para cargar");
        exit(EXIT_FAILURE);
    }
    Nodo* head = NULL;
    Reserva r;
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%d\n", &r.id, r.cliente, r.fecha_inicio, 
        r.fecha_fin, &r.habitacion) != EOF) {
        Nodo* nuevo = crear_reserva(r.id, r.cliente, r.fecha_inicio, r.fecha_fin, r.habitacion);
        agregar_reserva(&head, nuevo);
    }
    fclose(file);
    return head;
}