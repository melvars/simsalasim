#ifndef WARNINGS_H
#define WARNINGS_H

#include <def.h>

void warnings_print(void);
void warnings_add(u32 line, const char *fmt, ...);
void warnings_clear(void);
u8 warnings_exist(void);

#endif
