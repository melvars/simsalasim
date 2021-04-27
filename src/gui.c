#include <def.h>
#include <gtksourceview/gtksource.h>
#include <gui.h>
#include <parser.h>
#include <stdio.h>
#include <string.h>
#include <syntax.h>

#define BUFFER_SIZE 1024

static GtkWidget *window;
static GtkWidget *text_view;
static char filename[1024] = { 0 };

static char *gui_text_buffer(void)
{
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

	gtk_text_buffer_get_bounds(buffer, &start, &end);
	gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

	return text; // g_free after use!
}

static void gui_fill_text_view(const char *text)
{
	GtkSourceBuffer *text_buffer = gtk_source_buffer_new(NULL);
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer), text, (gint)strlen(text));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(text_view), GTK_TEXT_BUFFER(text_buffer));
}

static void gui_fill_text_view_from_file(const char *path)
{
	char buf[BUFFER_SIZE] = { 0 };
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

	if (parse(buf, BUFFER_SIZE))
		gui_show_info("Compiled successfully!");

	g_free(buf);
}

static void gui_call_syntax_highlighter(void)
{
	char *buf = gui_text_buffer();

	syntax_highlight(buf, BUFFER_SIZE);

	g_free(buf);
}

static void gui_init_highlighter(void)
{
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	gtk_text_buffer_create_tag(buffer, "instr", "foreground", "#ff0000", NULL);
	gtk_text_buffer_create_tag(buffer, "regs", "foreground", "#00ff00", NULL);
}

void gui_unhighlight(void)
{
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

	gtk_text_buffer_get_bounds(buffer, &start, &end);
	gtk_text_buffer_remove_all_tags(buffer, &start, &end);
}

void gui_unhighlight_name(const char *tag_name)
{
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

	gtk_text_buffer_get_bounds(buffer, &start, &end);
	gtk_text_buffer_remove_tag_by_name(buffer, tag_name, &start, &end);
}

void gui_highlight(u32 column, u32 line, u32 length, const char *tag_name)
{
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

	gtk_text_buffer_get_iter_at_line_offset(buffer, &start, line, column);
	gtk_text_buffer_get_iter_at_line_offset(buffer, &end, line, column + length);

	gtk_text_buffer_apply_tag_by_name(buffer, tag_name, &start, &end);
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

static gchar *gui_show_tt(GtkSourceMarkAttributes *attributes, GtkSourceMark *mark,
			  const gchar *message)
{
	UNUSED(attributes);
	UNUSED(mark);
	return strdup(message);
}

void gui_add_line_marker(u32 line_number, const char *message, const char *name, const char *category,
			 const char *icon, GdkRGBA rgba)
{
	GtkSourceMarkAttributes *attributes = gtk_source_mark_attributes_new();
	gtk_source_mark_attributes_set_background(attributes, &rgba);
	gtk_source_mark_attributes_set_icon_name(attributes, icon);
	gtk_source_view_set_mark_attributes(GTK_SOURCE_VIEW(text_view), category, attributes, 10);

	GtkTextIter iter;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	gtk_text_buffer_get_iter_at_line(buffer, &iter, line_number);
	gtk_source_view_set_mark_attributes(GTK_SOURCE_VIEW(text_view), category, attributes, 10);
	gtk_source_buffer_create_source_mark(GTK_SOURCE_BUFFER(buffer), name, category, &iter);
	g_signal_connect(G_OBJECT(attributes), "query-tooltip-text", G_CALLBACK(gui_show_tt),
			 strdup(message));
}

void gui_remove_line_marker(const char *category) {
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

	gtk_text_buffer_get_bounds(buffer, &start, &end);
	gtk_source_buffer_remove_source_marks(GTK_SOURCE_BUFFER(buffer), &start, &end, category);
}

static u8 gui_save_file(char *fname, char *fdata)
{
	FILE *f = fopen(fname, "w");
	if (f == NULL)
		return 0;
	fputs(fdata, f);
	fclose(f);
	return 1;
}

static void gui_show_save_file_dialog(void)
{
	GtkWidget *dialog =
		gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window),
					    GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel",
					    GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

	gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);

	char *text;
	if (strlen(filename) == 0) {
		gtk_file_chooser_set_current_folder(chooser, g_get_home_dir());
		gtk_file_chooser_set_current_name(chooser, "new.asm");
		int res = gtk_dialog_run(GTK_DIALOG(dialog));
		if (res == GTK_RESPONSE_ACCEPT) {
			char *savefile = gtk_file_chooser_get_filename(chooser);
			strcpy(filename, savefile);
			g_free(savefile);
			text = gui_text_buffer();
			gtk_widget_destroy(dialog);
		} else {
			return;
		}
	} else {
		text = gui_text_buffer();
	}

	u8 ret = gui_save_file(filename, text);
	g_free(text);
	if (ret)
		gui_show_info("File saved!");
	else
		gui_show_warning("Could not save file!");
}

static void gui_show_open_file_dialog(void)
{
	GtkWidget *dialog =
		gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(window),
					    GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel",
					    GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

	int res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		gtk_file_chooser_set_current_folder(chooser, g_get_home_dir());
		char *openfile = gtk_file_chooser_get_filename(chooser);
		gui_fill_text_view_from_file(openfile);
		strcpy(filename, openfile);
		g_free(openfile);
	}

	gtk_widget_destroy(dialog);
}

static void gui_show_new_file_dialog(void)
{
	gui_fill_text_view("");
	memset(filename, 0, sizeof(filename));
}

static gboolean gui_key_release_handler(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	UNUSED(widget);
	UNUSED(event);
	UNUSED(data);

	gui_call_syntax_highlighter();

	return FALSE;
}

static void gui_add_menu_item(const char *name, GtkWidget *parent, GtkAccelGroup *accel_group,
			      GdkModifierType mask_key, guint key, void (*callback)(void))
{
	GtkWidget *menu_item = gtk_menu_item_new_with_label(name);
	g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(callback), NULL);
	gtk_widget_add_accelerator(menu_item, "activate", accel_group, key, mask_key,
				   GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(parent), menu_item);
}

static void gui_activate(GtkApplication *app, gpointer data)
{
	UNUSED(data);

	// Initialize window
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "simsalasim");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

	// Key press listener
	g_signal_connect(window, "key_release_event", G_CALLBACK(gui_key_release_handler), NULL);

	// Main container
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), box);

	// Keyboard shortcut map init
	GtkAccelGroup *accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	// Very cool menu bar
	GtkWidget *menu_bar = gtk_menu_bar_new();
	// File menu
	GtkWidget *file_menu = gtk_menu_new();
	GtkWidget *file_tab = gtk_menu_item_new_with_label("File");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_tab), file_menu);

	gui_add_menu_item("New", file_menu, accel_group, GDK_CONTROL_MASK, GDK_KEY_N,
			  gui_show_new_file_dialog);
	gui_add_menu_item("Open", file_menu, accel_group, GDK_CONTROL_MASK, GDK_KEY_O,
			  gui_show_open_file_dialog);
	gui_add_menu_item("Save", file_menu, accel_group, GDK_CONTROL_MASK, GDK_KEY_S,
			  gui_show_save_file_dialog);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_tab);

	GtkWidget *run_menu = gtk_menu_new();
	GtkWidget *run_tab = gtk_menu_item_new_with_label("Run");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(run_tab), run_menu);

	gui_add_menu_item("Compile", run_menu, accel_group, GDK_CONTROL_MASK, GDK_KEY_Return,
			  gui_call_parser);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), run_tab);

	gtk_container_add(GTK_CONTAINER(box), menu_bar);

	// Strange text view
	text_view = gtk_source_view_new();
	gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(text_view), TRUE);
	gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(text_view), TRUE);
	gtk_source_view_set_show_line_marks(GTK_SOURCE_VIEW(text_view), TRUE);
	gtk_box_pack_end(GTK_BOX(box), text_view, TRUE, TRUE, 0);

	// Font
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(
		provider, "textview { font-family: Monospace; font-size: 11pt; }", -1, NULL);
	gtk_style_context_add_provider(gtk_widget_get_style_context(text_view),
				       GTK_STYLE_PROVIDER(provider),
				       GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	g_object_unref(provider);

	// Show all widgets!
	gtk_widget_show_all(window);

	// Only for testing purposes
	gui_fill_text_view_from_file("test.asm");

	gui_init_highlighter();
	gui_call_syntax_highlighter();
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
