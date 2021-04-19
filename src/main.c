#include <gui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_help(void)
{
	printf("Help\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc == 2 && (strcmp(argv[1], "--help") == 0))
		print_help();

	return gui_init(argc, argv);
}
