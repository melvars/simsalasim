#ifndef GUI_H
#define GUI_H

#include <def.h>

int gui_init(int argc, char *argv[]);
void gui_show_warning(const char *text);
void gui_show_info(const char *text);
void gui_highlight(u32 column, u32 line, u32 length, const char *color);

#endif
