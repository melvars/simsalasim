#include <gtk/gtk.h>

#include <gui.h>
#include <log.h>

enum stepper_response_type {
	STEP_PREV,
	STEP_NEXT,
};

static struct gui_interface *interface;

static void error_dialog(GtkWindow *parent, const char *msg)
{
	GtkWidget *dialog = gtk_dialog_new_with_buttons(
		msg, parent, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		"OK", GTK_RESPONSE_CLOSE, NULL);
	GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	GtkWidget *label = gtk_label_new(msg);
	g_signal_connect_swapped(dialog, "response",
				 G_CALLBACK(gtk_window_destroy), dialog);

	gtk_box_append(GTK_BOX(content), label);
	gtk_widget_show(dialog);
}

static void stepper_response(GtkWidget *widget, enum stepper_response_type type)
{
	err ret = OK;

	switch (type) {
	case STEP_PREV:
		ret = interface->step_prev();
		break;
	case STEP_NEXT:
		ret = interface->step_next();
		break;
	default:
		break;
	}

	if (ret == END || ret == ERR) {
		error_dialog(GTK_WINDOW(widget),
			     ret == END ? "Reached end" : "An error occured");
	}
}

static void init_stepper_dialog(GtkWindow *parent)
{
	GtkWidget *stepper = gtk_dialog_new_with_buttons(
		"Stepper", parent,
		GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "Previous",
		1, "Next", 2, NULL);

	GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(stepper));
	GtkWidget *label = gtk_label_new("Stepper");

	g_signal_connect_swapped(stepper, "response",
				 G_CALLBACK(stepper_response), stepper);

	gtk_box_append(GTK_BOX(content), label);
	gtk_widget_show(stepper);
}

static void init_execution_window(GtkApplication *app)
{
	GtkWidget *execution = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(execution), "Execution");
	gtk_window_set_default_size(GTK_WINDOW(execution), 200, 200);
	gtk_widget_show(execution);

	init_stepper_dialog(GTK_WINDOW(execution));
}

static void activate(GtkApplication *app, gpointer user_data)
{
	(void)user_data;
	init_execution_window(app);
}

void gui_reg_names(const char *names, int n)
{
	(void)names;
	(void)n;
}

void gui_reg_update(int reg, uint64_t value)
{
	(void)reg;
	(void)value;
}

void gui_instr_done(char *instr)
{
	(void)instr;
}

void gui_register_interface(struct gui_interface *gui)
{
	interface = gui;
}

void gui_init(void)
{
	g_object_set(gtk_settings_get_default(),
		     "gtk-application-prefer-dark-theme", TRUE, NULL);
	GtkApplication *app = gtk_application_new("de.melvars.simsalasim",
						  G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_application_run(G_APPLICATION(app), 0, 0);
	g_object_unref(app);
}
