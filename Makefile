CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC = main.c cli.c storage.c
OBJ = $(SRC:.c=.o)
TARGET = brag

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
