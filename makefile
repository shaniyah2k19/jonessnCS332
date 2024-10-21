CC = gcc
CFLAGS = -Wall -Werror -std=c11
TARGET = jonessn_HW03

all: $(TARGET)

$(TARGET): jonessn_HW03.c
	$(CC) $(CFLAGS) -o $(TARGET) jonessn_HW03.c
	
clean:
	rm -f $(TARGET)