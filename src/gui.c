#include <def.h>
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include <gui.h>
#include <parser.h>
#include <stdio.h>
#include <string.h>

// Main window
static GtkWidget *window;
static GtkWidget *text_view;

static char *gui_text_buffer(void)
{
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

	gtk_text_buffer_get_bounds(buffer, &start, &end);
	gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

	return text; // g_free after use!
}

static void gui_fill_text_view(const char *path)
{
	char buf[1024] = { 0 };
	FILE *test = fopen(path, "r");

	if (!test) {
		gui_show_warning(strerror(errno));
		return;
	}

	fread(buf, sizeof(buf), sizeof(buf[0]), test);
	fclose(test);

	GtkSourceBuffer *text_buffer = gtk_source_buffer_new(NULL);
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer), buf, strnlen(buf, sizeof(buf)));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(text_view), GTK_TEXT_BUFFER(text_buffer));
}

static void gui_call_parser(void)
{
	char *buf = gui_text_buffer();

	// TODO: Get actual buffer size
	if (parse(buf, 1024))
		gui_show_info("Compiled successfully!");

	g_free(buf);
}

void gui_show_warning(const char *text)
{
	if (!text)
		return;

	GtkWidget *warning = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
						    GTK_BUTTONS_OK, "Warning");

	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(warning), "%s\n", text);
	int response = gtk_dialog_run(GTK_DIALOG(warning));
	printf("%d\n", response);
	gtk_widget_destroy(warning);
}

void gui_show_info(const char *text)
{
	if (!text)
		return;

	GtkWidget *info = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
						 GTK_BUTTONS_OK, "Information");

	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(info), "%s\n", text);
	int response = gtk_dialog_run(GTK_DIALOG(info));
	printf("%d\n", response);
	gtk_widget_destroy(info);
}

static gboolean gui_key_press_handler(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	UNUSED(widget);
	UNUSED(data);

	if (event->state & GDK_CONTROL_MASK) {
		if (event->keyval == 's')
			gui_show_warning("Saving is not yet supported");
		else if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter)
			gui_call_parser();

		return TRUE;
	}

	gui_call_syntax_highlighter();

	return FALSE;
}

static void gui_activate(GtkApplication *app, gpointer data)
{
	UNUSED(data);

	// Initialize window
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "simsalasim");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

	// Key press listener
	g_signal_connect(window, "key_press_event", G_CALLBACK(gui_key_press_handler), NULL);

	// Main container
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
	gtk_container_add(GTK_CONTAINER(window), box);

	// Very cool menu bar
	GtkWidget *menu_bar = gtk_menu_bar_new();
	GtkWidget *file_menu = gtk_menu_new();
	GtkWidget *file_tab = gtk_menu_item_new_with_label("File");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_tab), file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_tab);
	gtk_container_add(GTK_CONTAINER(box), menu_bar);

	// Strange text view
	text_view = gtk_source_view_new();
	gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(text_view), TRUE);
	gtk_box_pack_end(GTK_BOX(box), text_view, TRUE, TRUE, 0);

	gtk_widget_show_all(window);

	// Only for testing purposes
	gui_fill_text_view("test.asm");
}

int gui_init(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", TRUE, NULL);

	GtkApplication *app =
		gtk_application_new("de.melvars.simsalasim", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(gui_activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
