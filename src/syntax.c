#include <def.h>
#include <gui.h>
#include <lexer.h>
#include <stdio.h>
#include <syntax.h>

struct pos {
	u32 x;
	u32 y;
};

static void syntax_highlight_line(struct pos *pos, char *str, u32 size)
{
	/* printf("Got size %d '", size); */
	/* for (u32 i = 0; i < size; i++) { */
	/* 	printf("%c", str[i]); */
	/* } */
	/* printf("'\n"); */

	while (1) {
		struct token tok = token_resolve(str + pos->x, size - pos->x);

		if (tok.type == NEWLINE) {
			pos->x = 0;
			pos->y++;
			return;
		}

		if (tok.type == UNKNOWN)
			return;

		if (tok.type == SPACE) {
			pos->x += tok.length;
			continue;
		}

		/* token_print(&tok); */

		if (tok.type > INSTR_START && tok.type < INSTR_END)
			gui_highlight(pos->x, pos->y, tok.length, "instr");
		else if (tok.type > REGS_START && tok.type < REGS_END) {
			gui_highlight(pos->x, pos->y, tok.length, "regs");
		}

		pos->x += tok.length;
	}
}

void syntax_highlight(char *buf, u32 size)
{
	struct pos pos = { 0 };

	u32 diff = 0;
	char *start = buf;
	gui_unhighlight();
	for (u32 i = 0; i < size; i++) {
		// TODO: Fix highlighting of last line without \n
		if (buf[i] == '\0')
			break;

		if (buf[i] == '\n') {
			pos.x = 0;
			syntax_highlight_line(&pos, start, diff);
			start += diff + 1;
			diff = 0;
		} else {
			diff++;
		}
	}
}
