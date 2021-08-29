#ifndef LOG_H
#define LOG_H

void __logln(const char *func, const char *format, ...);
void __errln(const char *func, const char *format, ...);

#define logln(format, ...) __logln(__func__, format, ##__VA_ARGS__)
#define errln(format, ...) __errln(__func__, format, ##__VA_ARGS__)

#endif
