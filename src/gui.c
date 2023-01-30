#include <gtk/gtk.h>

#include <gui.h>
#include <log.h>

enum stepper_response_type {
	STEP_PREV,
	STEP_NEXT,
};

static struct gui_interface *interface;
static GtkWindow *main;
static GtkStringList *execution_list;

// TODO: Either beautify or use InfoBar
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

static void stepper_handle_response(err ret)
{
	if (ret == END || ret == ERR) {
		error_dialog(main,
			     ret == END ? "Reached end" : "An error occured");
	}
}

static void stepper_next(void)
{
	stepper_handle_response(interface->step_next());
}

static void stepper_prev(void)
{
	stepper_handle_response(interface->step_prev());
}

static void init_menu(void)
{
	GtkWidget *header = gtk_header_bar_new();

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_add_css_class(box, "linked");

	GtkWidget *prev = gtk_button_new_from_icon_name("go-previous-symbolic");
	gtk_box_append(GTK_BOX(box), prev);

	GtkWidget *next = gtk_button_new_from_icon_name("go-next-symbolic");
	gtk_box_append(GTK_BOX(box), next);

	g_signal_connect(prev, "clicked", G_CALLBACK(stepper_prev), prev);
	g_signal_connect(next, "clicked", G_CALLBACK(stepper_next), next);

	gtk_header_bar_pack_end(GTK_HEADER_BAR(header), box);
	gtk_window_set_titlebar(main, header);
}

static void setup_execution_list(GtkSignalListItemFactory *self,
				 GtkListItem *item, gpointer data)
{
	(void)self;
	(void)data;
	GtkWidget *test = gtk_label_new(0);
	gtk_label_set_xalign(GTK_LABEL(test), 0);
	gtk_list_item_set_child(item, test);
}

static void bind_execution_list(GtkSignalListItemFactory *self,
				GtkListItem *item, gpointer data)
{
	(void)self;
	(void)data;
	GtkWidget *test = gtk_list_item_get_child(item);
	GtkStringObject *str = gtk_list_item_get_item(item);
	gtk_label_set_text(GTK_LABEL(test), gtk_string_object_get_string(str));
}

static GtkWidget *init_execution_list(void)
{
	const char *array[] = { NULL };
	execution_list = gtk_string_list_new((const char *const *)array);
	GtkNoSelection *model =
		gtk_no_selection_new(G_LIST_MODEL(execution_list));

	GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(setup_execution_list), 0);
	g_signal_connect(factory, "bind", G_CALLBACK(bind_execution_list), 0);
	return gtk_list_view_new(GTK_SELECTION_MODEL(model), factory);
}

static void init_main_window(GtkApplication *app)
{
	main = GTK_WINDOW(gtk_application_window_new(app));
	gtk_window_set_title(main, "Simsalasim");
	gtk_window_set_default_size(main, 200, 200);

	GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_window_set_child(main, paned);

	GtkWidget *left = init_execution_list();
	gtk_paned_set_start_child(GTK_PANED(paned), left);

	GtkWidget *right = gtk_label_new("end");
	gtk_paned_set_end_child(GTK_PANED(paned), right);
}

static void activate(GtkApplication *app, gpointer user_data)
{
	(void)user_data;
	init_main_window(app);
	init_menu();
	gtk_widget_show(GTK_WIDGET(main));
}

void gui_reg_names(const char *names[], int n)
{
	(void)names;
	(void)n;
}

void gui_reg_update(int reg, uint64_t value)
{
	(void)reg;
	(void)value;
}

static int execution_list_length = 0;
void gui_instr_push(char *instr)
{
	gtk_string_list_take(execution_list, instr);
	execution_list_length++;
}

void gui_instr_pop(void)
{
	gtk_string_list_remove(execution_list, --execution_list_length);
}

void gui_register_interface(struct gui_interface *gui)
{
	interface = gui;
}

void gui_init(void)
{
	GtkApplication *app = gtk_application_new("de.melvars.simsalasim",
						  G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_application_run(G_APPLICATION(app), 0, 0);
	g_object_unref(app);
}
