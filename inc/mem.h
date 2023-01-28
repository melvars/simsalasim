#ifndef MEM_H
#define MEM_H

#include <stdio.h>
#include <stdint.h>

typedef uint64_t vaddr;

void *mem_alloc(size_t size, vaddr virt);
void *mem_phys(vaddr virt);
void mem_free_all(void);

#endif
