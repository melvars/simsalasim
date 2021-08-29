#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kvm.h>
#include <log.h>

struct aalloc_info {
	void *actual_ptr;
};

static void *aalloc(int size, int align)
{
	int adjusted = align - 1;
	void *actual = calloc(1, sizeof(void *) + size + adjusted);
	struct aalloc_info *ai =
		(void *)(((uintptr_t)((long)actual + sizeof(void *) + adjusted) & ~adjusted) -
			 sizeof(void *));
	ai->actual_ptr = actual;
	return ((char *)ai) + sizeof(void *);
}

static void afree(void *ptr)
{
	struct aalloc_info *a = (void *)((char *)ptr - sizeof(void *));
	free(a->actual_ptr);
}

static void load_bios(const char *path)
{
	FILE *bios = fopen(path, "rb");
	if (!bios) {
		errln("Couldn't open '%s': %s", path, strerror(errno));
		exit(1);
	}
	fseek(bios, 0, SEEK_END);
	int size = ftell(bios);
	fseek(bios, 0, SEEK_SET);

	void *data = aalloc(size, 4096);
	if (fread(data, size, 1, bios) != 1) {
		errln("Couldn't read '%s': %s", path, strerror(errno));
		exit(1);
	}

	fclose(bios);

	int load = kvm_load(0, (long)data, size);
	if (load < 0) {
		errln("Couldn't load '%s'", path);
		exit(1);
	}

	afree(data);
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	int cpu = kvm_init();
	if (cpu < 0)
		exit(1);
	load_bios("build/test");
	kvm_exec();

	return 0;
}
