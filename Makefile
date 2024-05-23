CC=gcc
CFLAGS=-lncurses -std=c99 -pedantic

CFILES=cpu.c screen.c main.c
OBJECTS=cpu.o screen.o main.o

OUT_DIR=out
BINARY=bin

all: $(OUT_DIR) $(OUT_DIR)/$(BINARY)

$(OUT_DIR):
	mkdir $(OUT_DIR)

$(OUT_DIR)/$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $^

clean:
	rm -rf $(OUT_DIR) *.o
