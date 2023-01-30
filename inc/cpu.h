#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#include <err.h>

struct cpu_interface {
	void (*reg_names)(const char *names[], int n);
	void (*reg_update)(int reg, uint64_t val);
	void (*instr_push)(char *instr);
	void (*instr_pop)(void);
};

enum registers {
	RAX,
	RCX,
	RDX,
	RBX,
	RSP,
	RBP,
	RSI,
	RDI,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
	REGISTERS_COUNT,
	AL = RAX,
	CL = RCX,
	DL = RDX,
	BL = RBX,
	SPL = RSP,
	BPL = RBP,
	SIL = RSI,
	DIL = RDI,
	R8B = R8,
	R9B = R9,
	R10B = R10,
	R11B = R11,
	R12B = R12,
	R13B = R13,
	R14B = R14,
	R15B = R15,
	AH = AL + 4,
	CH = CL + 4,
	DH = DL + 4,
	BH = BL + 4,
	SPH = SPL + 4,
	BPH = BPL + 4,
	SIH = SIL + 4,
	DIH = DIL + 4
};

err cpu_next(void);
err cpu_prev(void);

void *cpu_get_reg(uint8_t reg);
void cpu_set_reg(uint8_t reg, uint64_t val);
void cpu_register_interface(struct cpu_interface *cpu);
void cpu_exec(const char *path);
void cpu_destroy(void);

#endif
