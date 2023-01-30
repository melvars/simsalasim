#include <stdlib.h>

#include <cpu.h>
#include <load.h>
#include <log.h>
#include <linux.h>
#include <mem.h>

#define MOD() ((modrm & 0xc0) >> 6)
#define RM() (modrm & 0x07)
#define DISP32() (U32())
#define DISP8() (S8())
#define REG() ((modrm & 0x38) >> 3)
#define ADDR()                                                                 \
	__extension__({                                                        \
		uint32_t _ret = 0;                                             \
		if (MOD() == 0) {                                              \
			if (RM() == 4)                                         \
				errln("TODO.");                                \
			else if (RM() == 5)                                    \
				_ret = DISP32();                               \
			else                                                   \
				_ret = GET_R32(RM());                          \
		} else if (MOD() == 1) {                                       \
			if (RM() == 4)                                         \
				errln("TODO.");                                \
			else                                                   \
				_ret = GET_R32(RM()) + DISP8();                \
		} else {                                                       \
			if (RM() == 4)                                         \
				errln("TODO.");                                \
			else                                                   \
				_ret = GET_R32(RM()) + DISP32();               \
		}                                                              \
		_ret;                                                          \
	})

#define U8() (*(uint8_t *)mem_phys(rip++))
#define S8() (*(int8_t *)mem_phys(rip++))
#define U16()                                                                  \
	__extension__({                                                        \
		uint16_t ret = *(uint16_t *)mem_phys(rip);                     \
		rip += 2;                                                      \
		ret;                                                           \
	})
#define S16()                                                                  \
	__extension__({                                                        \
		int16_t ret = *(int16_t *)mem_phys(rip);                       \
		rip += 2;                                                      \
		ret;                                                           \
	})
#define U32()                                                                  \
	__extension__({                                                        \
		uint32_t ret = *(uint32_t *)mem_phys(rip);                     \
		rip += 4;                                                      \
		ret;                                                           \
	})
#define S32()                                                                  \
	__extension__({                                                        \
		int32_t ret = *(int32_t *)mem_phys(rip);                       \
		rip += 4;                                                      \
		ret;                                                           \
	})
#define U64()                                                                  \
	__extension__({                                                        \
		uint64_t ret = *(uint64_t *)mem_phys(rip);                     \
		rip += 8;                                                      \
		ret;                                                           \
	})
#define S64()                                                                  \
	__extension__({                                                        \
		int64_t ret = *(int64_t *)mem_phys(rip);                       \
		rip += 8;                                                      \
		ret;                                                           \
	})

#define GET_R8(r) (r < 8 ? regs[r] & 0xff : (regs[r - 8] >> 8) & 0xff)
#define GET_R32(r) ((uint32_t)regs[r])
#define GET_R64(r) ((uint64_t)regs[r])
#define SET_R8(r, v)                                                           \
	__extension__({                                                        \
		if (r < 8)                                                     \
			regs[r] = (regs[r] & 0xffffff00) | (uint32_t)v;        \
		else                                                           \
			regs[r - 4] = (regs[r] = (regs[r] & 0xffff00ff) |      \
						 ((uint32_t)v << 8))           \
	})
#define SET_R32(r, v) (regs[r] = v)
#define SET_R64(r, v) (regs[r] = v)

#define GET_RM8() (MOD() == 3 ? GET_R8(RM()) : *(uint8_t *)mem_phys(ADDR()))
#define GET_RM32() (MOD() == 3 ? GET_R32(RM()) : *(uint32_t *)mem_phys(ADDR()))

static struct cpu_interface *interface;

struct rip_history {
	uint64_t rip;
	struct rip_history *prev;
};

static uint64_t rip = 0;
static err (*pos_instructions[256])(void);
static err (*neg_instructions[256])(void);
static uint64_t regs[REGISTERS_COUNT] = { 0 };
static struct rip_history *rip_history;

static char instruction_response_factory[256] = { 0 };

static const char *register_names[REGISTERS_COUNT] = {
	"RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI",
	"R8",  "R9",  "R10", "R11", "R12", "R13", "R14", "R15",
};

static void print_state(void)
{
	logln("rip=%x, rax=%x, rcx=%x, rdx=%x, rbx=%x, rsp=%x, rbp=%x, rsi=%x, rdi=%x,\nr8=%x, r9=%x, r10=%x, r11=%x, r12=%x, r13=%x, r14=%x, r15=%x",
	      rip, regs[RAX], regs[RCX], regs[RDX], regs[RBX], regs[RSP],
	      regs[RBP], regs[RSI], regs[RSI], regs[RDI], regs[R8], regs[R9],
	      regs[R10], regs[R11], regs[R12], regs[R13], regs[R14], regs[R15]);
}

static err pos_unknown_instruction(void)
{
	errln("unknown positive instruction at rip=%d", rip);
	return ERR;
}

static err neg_unknown_instruction(void)
{
	errln("unknown negative instruction at rip=%d", rip);
	return ERR;
}

static err pos_opcode(void)
{
	uint8_t op = U8();
	// TODO: is_*, j*
	if (op == 0x05) {
		sprintf(instruction_response_factory, "syscall");
		return linux_call();
	}
	return ERR;
}

static err pos_mov_r32_rm32(void)
{
	uint8_t modrm = U8();
	uint8_t reg = REG();
	uint32_t val = GET_RM32();
	sprintf(instruction_response_factory, "mov %s, %x", register_names[reg],
		val);
	return OK;
}

static err pos_mov_r32_imm32(void)
{
	rip--;
	uint8_t reg = U8() - 0xb8;
	uint32_t val = U32();
	SET_R32(reg, val);
	sprintf(instruction_response_factory, "mov %s, %#x",
		register_names[reg], val);
	return OK;
}

static err pos_nop(void)
{
	sprintf(instruction_response_factory, "nop");
	return OK;
}

static err pos_int(void)
{
	uint8_t op = U8();
	if (op == 0x80) {
		// Linux syscall
		// TODO: Support legacy syscalls (numbers are different)
		return ERR;
	}
	return ERR;
}

static void initialize(void)
{
	interface->reg_names(register_names, REGISTERS_COUNT);
	for (int i = 0; i < 256; i++) {
		pos_instructions[i] = pos_unknown_instruction;
		neg_instructions[i] = neg_unknown_instruction;
	}
	pos_instructions[0x0f] = pos_opcode;
	pos_instructions[0x8b] = pos_mov_r32_rm32;
	pos_instructions[0x90] = pos_nop;
	pos_instructions[0xcd] = pos_int;
	/* for (int i = 0; i < 8; i++) */
	/* 	pos_instructions[0xb0+i] = pos_mov_r8_imm8; */
	for (int i = 0; i < 8; i++)
		pos_instructions[0xb8 + i] = pos_mov_r32_imm32;
}

void *cpu_get_reg(uint8_t reg)
{
	return &regs[reg];
}

// TODO: Different signature?
void cpu_set_reg(uint8_t reg, uint64_t val)
{
	regs[reg] = val;
}

void cpu_register_interface(struct cpu_interface *cpu)
{
	interface = cpu;
}

void cpu_exec(const char *path)
{
	initialize();

	vaddr addr = load(path);
	if (!addr) {
		errln("invalid entry address");
		return;
	}
	rip = addr;

	rip_history = malloc(sizeof(*rip_history));
	rip_history->rip = rip;
	rip_history->prev = 0;
}

err cpu_next(void)
{
	uint8_t instr = U8();
	if (!instr)
		return END;
	logln("%x", instr);
	err ret = pos_instructions[instr]();
	interface->instr_push(instruction_response_factory);
	print_state();

	struct rip_history *next = malloc(sizeof(*next));
	next->rip = rip;
	next->prev = rip_history;
	rip_history = next;
	return ret;
}

err cpu_prev(void)
{
	struct rip_history *prev = rip_history->prev;
	if (!prev)
		return END;
	rip = prev->rip;
	uint8_t instr = U8();
	if (!instr)
		return ERR;
	err ret = neg_instructions[instr]();
	interface->instr_pop();
	print_state();

	free(rip_history);
	rip_history = prev;
	return ret;
}

void cpu_destroy(void)
{
	while (rip_history) {
		struct rip_history *temp = rip_history->prev;
		free(rip_history);
		rip_history = temp;
	}
}
