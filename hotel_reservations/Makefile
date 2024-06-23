CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lrt -pthread

SRC = src/main.c src/reserva.c src/shared_data.c src/proceso_reserva.c src/hilos.c
OBJ = $(SRC:.c=.o)
EXEC = hotel_reservations

all: $(EXEC)

hotel_reservations: src/main.o src/reserva.o src/shared_data.o src/proceso_reserva.o src/hilos.o
    $(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJ) $(EXEC)

.PHONY: all clean