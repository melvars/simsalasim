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
WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wwrite-strings -Wredundant-decls -Wnested-externs -Wformat=2 -Wmissing-declarations -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -Wswitch-default -Wswitch-enum -Wlogical-op -Wunreachable-code -Wundef -Wold-style-definition -Wvla -pedantic -Wno-pointer-arith
DEBUG = -fsanitize=undefined -fsanitize=address -fsanitize=leak -fstack-protector-strong -s
CFLAGS = -Ofast $(WARNINGS) -I$(INCDIR) $(DEBUG)
ASFLAGS = -O2 -f elf64 # TODO: Use -O0

all: out tests

out: $(OBJS)
	@$(CC) -o $(BUILDDIR)/out $(CFLAGS) $^

tests: $(CTESTSOBJS) $(ASMTESTSOBJS)

$(BUILDDIR)/test_asm_%.o: $(TESTSDIR)/asm/%.asm
	@$(AS) $(ASFLAGS) $< -o $@
	@$(LD) $@ -o $(basename $@)

$(BUILDDIR)/test_c_%.o: $(TESTSDIR)/c/%.c
	@$(CC) $< -o $(basename $@)

clean:
	@$(RM) -rf $(BUILDDIR)/*

run: all sync
	@$(BUILDDIR)/out $(BUILDDIR)/test_asm_yay

sync:
	@make --always-make --dry-run | grep -wE 'gcc|g\+\+' | grep -w '\-c' | jq -nR '[inputs|{directory:".", command:., file: match(" [^ ]+$$").string[1:]}]' >compile_commands.json
	@ctags -R --exclude=.git --exclude=build .

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(BUILDDIR)
	@$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: all out tests clean run sync
