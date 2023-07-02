CC = gcc
CFLAGS = -fopenmp
TARGET = align

all: $(TARGET)

$(TARGET): align.o
	$(CC) $(CFLAGS) align.o -o $(TARGET)

align.o: align.c
	$(CC) $(CFLAGS) -c align.c

clean:
	rm -f align.o $(TARGET)
