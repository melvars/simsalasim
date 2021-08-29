#include <stdarg.h>
#include <stdio.h>

#include <log.h>

#define LOG_OUT stdout
#define ERR_OUT stderr

void __logln(const char *func, const char *format, ...)
{
	fprintf(LOG_OUT, "[LOG] %s: ", func);

	va_list ap;
	va_start(ap, format);
	vfprintf(LOG_OUT, format, ap);
	va_end(ap);

	fprintf(LOG_OUT, "\n");
}

void __errln(const char *func, const char *format, ...)
{
	fprintf(ERR_OUT, "[ERR] %s: ", func);

	va_list ap;
	va_start(ap, format);
	vfprintf(ERR_OUT, format, ap);
	va_end(ap);

	fprintf(ERR_OUT, "\n");
}
