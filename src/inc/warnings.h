#ifndef WARNINGS_H
#define WARNINGS_H

#include <def.h>

struct context {
	u32 line;
	u32 column;
};

void warnings_print(void);
void warnings_add(struct context *ctx, const char *fmt, ...);
void warnings_clear(void);
void warnings_remove_marks(void);
u8 warnings_exist(void);

#endif
