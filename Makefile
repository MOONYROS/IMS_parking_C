CC = gcc
CFLAGS = -Wall -g
TARGET = main
SOURCE = main.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

pack:
	zip T8_xlukas15.zip dokumentace.pdf main.c Makefile
