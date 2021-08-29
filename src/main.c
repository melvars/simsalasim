#include <cpu.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	int cpu = cpu_init();
	if (cpu < 0)
		exit(1);
	cpu_run();
	cpu_destroy();

	return 0;
}
