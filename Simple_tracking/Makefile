TARGET = main

CC = gcc

SRCS = main.c image.c

HEADERS = image.h

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -g

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
