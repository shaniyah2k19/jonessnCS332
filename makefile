CC = gcc
CFLAGS = -Wall -Wextra
TARGET = traverse

all: $(TARGET)

$(TARGET): jonessn_HW02.c
	$(CC) $(CFLAGS) -o $(TARGET) jonessn_HW02.c

run: $(TARGET)
	./$(TARGET)
	
clean:
	rm -f $(TARGET)