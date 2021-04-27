SOURCEDIR = src
BUILDDIR = build
SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJS = $(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

CC = gcc
WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wwrite-strings -Wredundant-decls -Wnested-externs -Wformat=2 -Wmissing-declarations -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -Wswitch-default -Wswitch-enum -Wlogical-op -Wunreachable-code -Wundef -Wold-style-definition -Wvla -pedantic
DEBUG = -fsanitize=undefined -ggdb3 -g3 -g -s -Og
CFLAGS = -Ofast $(WARNINGS) -I$(SOURCEDIR)/inc/ $(shell pkg-config --cflags --libs gtk+-3.0 gtksourceview-4) $(DEBUG)

all: $(OBJS)
	@$(CC) -o ./$(BUILDDIR)/out $^ $(CFLAGS)

clean:
	@$(RM) -rf $(BUILDDIR)

run: clean all
	@./$(BUILDDIR)/out

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(BUILDDIR)
	@$(CC) -c -o $@ $< $(CFLAGS)
