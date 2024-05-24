C := gcc
CFLAGS := -lncurses -std=c99 -pedantic

SRCDIR := src
INCLUDEDIR := include
OBJDIR := obj

SOURCES := $(shell find $(SRCDIR) -name "*.c")
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
INCLUDES := $(addprefix -I,$(INCLUDEDIR))

BINARY := z70emu

all: $(OBJDIR) $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(INCLUDES) -c -o $@ $^

clean:
	rm -rf $(OBJDIR) $(BINARY)

