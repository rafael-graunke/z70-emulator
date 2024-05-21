CC=gcc
CFLAGS=

CFILES=cpu.c main.c
OBJECTS=cpu.o main.o

OUT_DIR=out
BINARY=bin

all: $(OUT_DIR) $(OUT_DIR)/$(BINARY)

$(OUT_DIR):
	mkdir $(OUT_DIR)

$(OUT_DIR)/$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(OUT_DIR) *.o
