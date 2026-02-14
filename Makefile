CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = brag
PREFIX = /usr/local/bin

# Build the project
all:
	$(CC) $(CFLAGS) -o $(TARGET) main.c cli.c storage.c

# Build, copy the binary to PATH and clean up (requires sudo)
install: all
	cp $(TARGET) $(PREFIX)/$(TARGET)
	rm -f $(TARGET)

# Remove the binary from PATH (requires sudo)
uninstall:
	rm -f $(PREFIX)/$(TARGET)

# Remove the local binary
clean:
	rm -f $(TARGET)

.PHONY: all install uninstall clean
