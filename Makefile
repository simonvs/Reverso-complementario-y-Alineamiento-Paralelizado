CC = gcc
CFLAGS = -fopenmp
TARGET = reverse_complement

all: $(TARGET)

$(TARGET): reverse_complement.o
	$(CC) $(CFLAGS) reverse_complement.o -o $(TARGET)

reverse_complement.o: reverse_complement.c
	$(CC) $(CFLAGS) -c reverse_complement.c

clean:
	rm -f reverse_complement.o $(TARGET)
