# Thiết lập biến
CC = gcc
CFLAGS = -Wall
TARGET = main
SOURCES = main.c feature_extraction.c distance.c io_handler.c 
OBJECTS = $(SOURCES:.c=.o)

# Mục tiêu mặc định
all: $(TARGET)

# Quy tắc để tạo tệp thực thi từ các tệp đối tượng
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Quy tắc để biên dịch tệp .c thành tệp .o
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Quy tắc để dọn dẹp các tệp đối tượng và tệp thực thi
clean:
	rm -f $(OBJECTS) $(TARGET)
