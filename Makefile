CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = browser_history

all: $(TARGET)

$(TARGET): browser_history.c
	$(CC) $(CFLAGS) -o $(TARGET) browser_history.c

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all run clean
