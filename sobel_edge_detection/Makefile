CC = gcc
CFLAGS = -Wall
OBJ = main.o sobel.o image.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

main.o: main.c sobel.h image.h
	$(CC) $(CFLAGS) -c main.c

sobel.o: sobel.c sobel.h
	$(CC) $(CFLAGS) -c sobel.c

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c

clean:
	rm -f $(OBJ) $(TARGET)
