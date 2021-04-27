#ifndef GUI_H
#define GUI_H

#include <def.h>
#include <gtk/gtk.h>

int gui_init(int argc, char *argv[]);
void gui_show_warning(const char *text);
void gui_show_info(const char *text);

void gui_add_line_marker(u32 line_number, const char *message, const char *name, const char *category, const char *icon, GdkRGBA rgba);
void gui_remove_line_marker(const char *category);

void gui_unhighlight(void);
void gui_unhighlight_name(const char *tag_name);
void gui_highlight(u32 column, u32 line, u32 length, const char *color);

#endif
