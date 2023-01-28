#include <cpu.h>
#include <mem.h>
#include <log.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	if (argc != 2) {
		logln("invalid arguments");
		return 1;
	}

	cpu_exec(argv[1]);

	mem_free_all();
	return 0;
}
