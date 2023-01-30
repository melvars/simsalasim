# TODO: I think I'm using make wrong
SOURCEDIR = $(PWD)/src
TESTSDIR = $(PWD)/tests
INCDIR = $(PWD)/inc
BUILDDIR = $(PWD)/build
SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJS = $(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

CTESTS = $(wildcard $(TESTSDIR)/c/*.c)
CTESTSOBJS = $(patsubst $(TESTSDIR)/c/%.c, $(BUILDDIR)/test_c_%.o, $(CTESTS))
ASMTESTS = $(wildcard $(TESTSDIR)/asm/*.asm)
ASMTESTSOBJS = $(patsubst $(TESTSDIR)/asm/%.asm, $(BUILDDIR)/test_asm_%.o, $(ASMTESTS))

CC = gcc
AS = nasm
LD = ld
WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wwrite-strings -Wredundant-decls -Wnested-externs -Wformat=2 -Wmissing-declarations -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -Wswitch-default -Wswitch-enum -Wunreachable-code -Wundef -Wold-style-definition -Wvla -pedantic -Wno-pointer-arith
DEBUG = -fsanitize=undefined,address,leak -fstack-protector-strong -s -Og
CFLAGS = -Ofast $(shell pkg-config --cflags --libs gtk4) $(WARNINGS) -I$(INCDIR) #$(DEBUG)
ASFLAGS = -O2 -f elf64 # TODO: Use -O0

all: $(BUILDDIR)/out tests

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@$(CC) -c $< $(CFLAGS) -o $@

$(BUILDDIR)/out: $(OBJS)
	@$(CC) $^ $(CFLAGS) -o $@

tests: $(CTESTSOBJS) $(ASMTESTSOBJS)

$(ASMTESTSOBJS): $(ASMTESTS)
	@$(AS) $(ASFLAGS) $< -o $@
	@$(LD) $@ -o $(basename $@)

$(CTESTSOBJS): $(CTESTS)
	@$(CC) $< -o $(basename $@)

clean:
	@$(RM) -rf $(BUILDDIR)/*

run: all sync
	@$(BUILDDIR)/out $(BUILDDIR)/test_asm_yay

sync:
	@$(MAKE) $(BUILDDIR)/out --always-make --dry-run | grep -wE 'gcc|g\+\+' | grep -w '\-c' | jq -nR '[inputs|{directory:".", command:., file: match(" [^ ]+$$").string[1:]}]' >compile_commands.json
	@ctags -R --exclude=.git --exclude=build .

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(BUILDDIR)
	@$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: all out tests clean run sync
