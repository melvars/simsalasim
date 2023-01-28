#include <stdint.h>
#include <unistd.h>

#include <cpu.h>
#include <mem.h>
#include <linux.h>
#include <log.h>

// TODO: Don't use unistd, emulate all syscalls for time-travel

static void call_write(void)
{
	uint64_t fd = *(uint64_t *)cpu_get_reg(RDI);
	char *buf = mem_phys(*(vaddr *)cpu_get_reg(RSI));
	uint64_t count = *(uint64_t *)cpu_get_reg(RDX);
	int ret = write(fd, buf, count);
	cpu_set_reg(RAX, ret);
}

static void call_exit(void)
{
	int64_t code = *(int64_t *)cpu_get_reg(RDI);
	logln("exiting with code %d", code);
}

int linux_call(void)
{
	uint64_t call = *(uint64_t *)cpu_get_reg(RAX);
	switch (call) {
	case 1:
		call_write();
		break;
	case 60:
		call_exit();
		return ERR;
	default:
		errln("invalid syscall %d", call);
		return ERR;
	}
	return OK;
}
