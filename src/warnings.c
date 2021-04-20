#include <assert.h>
#include <def.h>
#include <gui.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <warnings.h>

#define WARNING_COUNT 1024 // Well, something went horribly wrong I guess
#define WARNING_LENGTH 64

struct warning {
	u8 exists;
	struct context *ctx;
	char text[WARNING_LENGTH];
};

static struct warning warnings[WARNING_COUNT] = { 0 };
static u32 warning_index = 0;
void warnings_add(struct context *ctx, const char *fmt, ...)
{
	assert(warning_index + 1 < WARNING_COUNT);

	warnings[warning_index].exists = 1;
	warnings[warning_index].ctx = ctx;

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(warnings[warning_index].text, WARNING_LENGTH, fmt, ap);
	va_end(ap);

	warning_index++;
}

// TODO: Print somewhere else (e.g. next to line)
void warnings_print(void)
{
	for (u32 i = 0; i < WARNING_COUNT; i++) {
		if (!warnings[i].exists)
			continue;

		/* gui_show_warning(warnings[i].text); */
		printf("Line %d:%d: %s\n", warnings[i].ctx->line, warnings[i].ctx->column,
		       warnings[i].text);
	}
}

u8 warnings_exist(void)
{
	return warning_index > 0;
}

void warnings_clear(void)
{
	if (warning_index)
		memset(warnings, 0, sizeof(warnings));
}
