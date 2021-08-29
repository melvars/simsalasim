#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

#include <log.h>

#define LOG_OUT stdout
#define ERR_OUT stderr

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void __logln(const char *func, const char *format, ...)
{
	pthread_mutex_lock(&mutex);

	fprintf(LOG_OUT, "[LOG] %s: ", func);

	va_list ap;
	va_start(ap, format);
	vfprintf(LOG_OUT, format, ap);
	va_end(ap);

	fprintf(LOG_OUT, "\n");

	pthread_mutex_unlock(&mutex);
}

void __errln(const char *func, const char *format, ...)
{
	pthread_mutex_lock(&mutex);

	fprintf(ERR_OUT, "[ERR] %s: ", func);

	va_list ap;
	va_start(ap, format);
	vfprintf(ERR_OUT, format, ap);
	va_end(ap);

	fprintf(ERR_OUT, "\n");

	pthread_mutex_unlock(&mutex);
}
