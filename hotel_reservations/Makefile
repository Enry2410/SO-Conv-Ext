VPATH = src test

CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lrt -pthread

SRC = main.c reserva.c shared_data.c proceso_reserva.c hilos.c
OBJ = $(SRC:.c=.o)
TEST_SRC = test/test.c
TEST_OBJ = $(TEST_SRC:.c=.o)
EXEC = hotel_reservations test_program

all: $(EXEC)

hotel_reservations: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

test_program: $(TEST_OBJ) $(filter-out src/main.o, $(OBJ))
	$(CC) -Wl,-e,test_main -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(EXEC)

.PHONY: all clean
