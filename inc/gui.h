#ifndef GUI_H
#define GUI_H

#include <err.h>
#include <stdint.h>

struct gui_interface {
	err (*step_next)(void);
	err (*step_prev)(void);
};

void gui_reg_names(const char *names, int n);
void gui_reg_update(int reg, uint64_t value);
void gui_instr_done(char *instr);

void gui_register_interface(struct gui_interface *gui);
void gui_init(void);

#endif
