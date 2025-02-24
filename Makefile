CC = gcc -Wall

CFLAGS = -std=c17 -Wall -Wextra -pedantic

SRC = ansi/ansi_escapes.c main.c
OBJ = $(SRC:.c=.o)
TARGET = BlackJack

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
