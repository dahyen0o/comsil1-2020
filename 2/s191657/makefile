OBJECTS = main.o page.o print.o
SRC = main.c page.c print.c

CC=g++
CFLAGS = -c
TARGET = main

$(TARGET) : $(OBJECTS) head.h
	$(CC) -o $(TARGET) $(OBJECTS)

clean:
	rm $(OBJECTS) $(TARGET)
