CC = g++
CFLAGS = -fopenmp
TARGET = align

all: $(TARGET)

$(TARGET): align.o
	$(CC) $(CFLAGS) align.o -o $(TARGET)

align.o: align.cpp
	$(CC) $(CFLAGS) -c align.cpp

clean:
	rm -f align.o $(TARGET)
