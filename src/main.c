#include <cpu.h>
#include <mem.h>
#include <log.h>
#include <gui.h>

static struct cpu_interface cpu = {
	.reg_names = gui_reg_names,
	.reg_update = gui_reg_update,
	.instr_done = gui_instr_done,
};

static struct gui_interface gui = {
	.step_next = cpu_next,
	.step_prev = cpu_prev,
};

int main(int argc, char *argv[])
{
	if (argc != 2) {
		logln("invalid arguments");
		return 1;
	}

	cpu_register_interface(&cpu);
	cpu_exec(argv[1]);

	/* while (cpu_next() == OK) */
	/* 	; */

	gui_register_interface(&gui);
	gui_init();

	cpu_destroy();
	mem_destroy();
	return 0;
}
