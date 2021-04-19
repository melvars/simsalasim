#include <def.h>
#include <parser.h>
#include <stdio.h>
#include <string.h>

#define PEEK(hay, needle) (strcmp(hay, needle) == 0)

u8 parse(char *buf, u32 size)
{
	u32 line = 0;

	for (u32 i = 0; i < size; i++) {
		/* printf("'%c'\n", buf[i]); */
		if (buf[i] == '\0')
			break;

		if (buf[i] == '\n') {
			line++;
			continue;
		}
	}

	return 1;
}
