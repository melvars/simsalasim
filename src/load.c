#include <elf.h>
#include <stdio.h>
#include <string.h>

#include <load.h>
#include <log.h>

vaddr load(const char *path)
{
	FILE *file = fopen(path, "rb");
	if (!file) {
		errln("can't read file %s.", path);
		return 0;
	}

	Elf64_Ehdr header = { 0 };
	fread(&header, sizeof(header), 1, file);
	if (memcmp(header.e_ident, ELFMAG, SELFMAG)) {
		errln("invalid ELF %s.", path);
		return 0;
	}

	for (int i = 0; i < header.e_phnum; i++) {
		int offset = header.e_phoff + header.e_phentsize * i;
		if (fseek(file, offset, SEEK_SET)) {
			errln("can't seek program to %x", offset);
			return 0;
		}

		Elf64_Phdr program = { 0 };
		if (fread(&program, sizeof(program), 1, file) != 1) {
			errln("invalid program size %d", i);
			return 0;
		}

		if (!program.p_vaddr || program.p_type != PT_LOAD)
			continue;

		void *addr = mem_alloc(program.p_memsz, program.p_vaddr);
		if (!addr) {
			errln("can't allocate %dB at %x", program.p_memsz,
			      program.p_vaddr);
			return 0;
		}
		if (fseek(file, program.p_offset, SEEK_SET)) {
			errln("can't seek program to %x", offset);
			return 0;
		}
		if (fread(addr, program.p_filesz, 1, file) != 1) {
			errln("invalid program size %d", i);
			return 0;
		}

		logln("loaded program %d", i);
	}

	fclose(file);
	return header.e_entry;
}
