#include <stdio.h>
#include <stdlib.h>

#include <mem.h>
#include <log.h>

// TODO: Better data structure
struct memory {
	size_t size;
	vaddr virt; // for vm
	void *physical; // actually alloced
	struct memory *next;
};

static struct memory *memory = 0;
static struct memory *head = 0;

static uint64_t zero = 0;

void *mem_alloc(size_t size, vaddr virt)
{
	if (head) {
		memory->next = malloc(sizeof(*head));
		memory = memory->next;
	} else {
		head = malloc(sizeof(*head));
		memory = head;
	}
	memory->size = size;
	memory->virt = virt;
	memory->physical = calloc(1, size);
	memory->next = 0;
	return memory->physical;
}

void *mem_phys(vaddr virt)
{
	struct memory *iterator = head;
	while (iterator) {
		if (virt >= iterator->virt &&
		    virt < iterator->virt + iterator->size)
			return iterator->physical + (virt - iterator->virt);
		iterator = iterator->next;
	}
	errln("illegal memory access %x", virt);
	return &zero;
}

void mem_free_all(void)
{
	struct memory *iterator = head;
	while (iterator) {
		free(iterator->physical);
		struct memory *temp = iterator->next;
		free(iterator);
		iterator = temp;
	}
}
