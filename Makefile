CC = gcc
CFLAGS = -Wall -O2 -D_XOPEN_SOURCE=500
TARGET = gpio_control

all: $(TARGET)

$(TARGET): main.c gpio.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c gpio.c

clean:
	rm -f $(TARGET)

