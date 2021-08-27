#include <gtk/gtk.h>

#include "../include/hyper_app.h"
#include "../include/hyper_app_win.h"
#include "../include/hyper_app_prefs.h"


struct _HyperionApp
{
    GtkApplication parent;
};

G_DEFINE_TYPE(HyperionApp, hyperion_app, GTK_TYPE_APPLICATION);

static void hyperion_app_init(HyperionApp* app)
{
}

//////////////////////////////////////////////////////////////////////////////

void open_file_response_handler(GtkDialog *dialog, gint response_id, GtkWindow* window)
{
	if (response_id == GTK_RESPONSE_OK) {
		GFile* file;
		GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
		file = gtk_file_chooser_get_file(chooser);
        hyperion_app_window_open(HYPERION_APP_WINDOW(window), file);
	}
    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void open_file(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
	GtkWindow* window;
	GtkWidget* dialog;
	GApplication* app = user_data;

	window = gtk_application_get_active_window(GTK_APPLICATION(app));
	GtkFileChooserAction act = GTK_FILE_CHOOSER_ACTION_OPEN;
	dialog = gtk_file_chooser_dialog_new("Choose a File", GTK_WINDOW(window), act,
										"_Cancel", GTK_RESPONSE_CANCEL,
										"_Open", GTK_RESPONSE_OK, NULL);
	
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
	gtk_widget_show(dialog);

	g_signal_connect(dialog, "response", G_CALLBACK(open_file_response_handler), GTK_WINDOW(window));
}

static void create_file(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    GApplication* app = user_data;
}

static void create_window(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    GApplication* app = user_data;
    hyperion_app_activate(app);
}

static void save_file(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    GApplication* app = user_data;
	HyperionAppWindow* window;
    GtkWidget* current_file, *file_text_view;
    GtkTextBuffer* buffer;
    GtkTextIter start_iter, end_iter;
    char* contents;
    const char* visible_filename;

	window = HYPERION_APP_WINDOW(gtk_application_get_active_window(GTK_APPLICATION(app)));
    current_file = gtk_stack_get_visible_child(GTK_STACK(window->stack));
    visible_filename = gtk_stack_get_visible_child_name(GTK_STACK(window->stack));
    file_text_view = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(current_file));
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(file_text_view));

    gtk_text_buffer_get_start_iter(buffer, &start_iter);
    gtk_text_buffer_get_end_iter(buffer, &end_iter);

    contents = gtk_text_buffer_get_text(buffer, &start_iter, &end_iter, TRUE);
    g_file_set_contents(visible_filename, contents, strlen(contents), NULL);
}

void saveas_file_response_handler(GtkDialog *dialog, gint response_id, GtkWindow* window)
{
    if (response_id == GTK_RESPONSE_OK) {
		GFile* file;
		g_print("Hi");
	}
    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void saveas_file(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    GtkWindow* window;
    GtkWidget* dialog;
	GApplication* app = user_data;
    window = gtk_application_get_active_window(GTK_APPLICATION(app));

	GtkFileChooserAction act = GTK_FILE_CHOOSER_ACTION_SAVE;
	dialog = gtk_file_chooser_dialog_new("Save the File", GTK_WINDOW(window), act,
										"_Cancel", GTK_RESPONSE_CANCEL,
										"_Save", GTK_RESPONSE_OK, NULL);
	
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

	gtk_widget_show(dialog);

	g_signal_connect(dialog, "response", G_CALLBACK(saveas_file_response_handler), GTK_WINDOW(window));
}

static void close_editor(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    GtkWindow* window;
    GApplication* app = user_data;
    window = gtk_application_get_active_window(GTK_APPLICATION(app));
    hyperion_app_window_close(HYPERION_APP_WINDOW(window));
}

static void open_preferences(GSimpleAction* action, GVariant* parameter, gpointer app)
{
    HyperionAppPrefs* prefs;
    GtkWindow* win;

    win = gtk_application_get_active_window(GTK_APPLICATION(app));
    prefs = hyperion_app_prefs_new(HYPERION_APP_WINDOW(win));
    gtk_widget_show(GTK_WIDGET(prefs));
}

static void close_window(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    GtkWindow* window;
    GApplication* app = user_data;
    window = gtk_application_get_active_window(GTK_APPLICATION(app));
    gtk_window_destroy(window);
}

static void quit_window(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    GList* list, *next;
	GtkWindow* win;

	list = gtk_application_get_windows(GTK_APPLICATION(g_application_get_default()));
	while (list) {
		win = list->data;
		next = list->next;
		gtk_window_destroy(GTK_WINDOW(win));
		list = next;
	}
}

////////////////////////////////////////////////////////////////////////

static void file_undo(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    
}

static void file_rendo(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{

}

////////////////////////////////////////////////////////////////////////

static void show_documentation(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{

}

static void show_keyboard_shortcuts(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{

}

static void about_hyperion(GSimpleAction* action, GVariant* parameter, gpointer user_data)
{
    GtkWindow* window;
    GApplication* app = user_data;
    window = gtk_application_get_active_window(GTK_APPLICATION(app));
}

/////////////////////////////////////////////////////////////////////////

static GActionEntry app_entries[] = {
    { "open", open_file, NULL, NULL, NULL},
	{ "newfile", create_file, NULL, NULL, NULL},
	{ "newwindow", create_window, NULL, NULL, NULL},
	{ "save", save_file, NULL, NULL, NULL},
	{ "saveas", saveas_file, NULL, NULL, NULL},
    { "closeeditor", close_editor, NULL, NULL, NULL},
    { "winclose", close_window, NULL, NULL, NULL},
    { "quit", quit_window, NULL, NULL, NULL },

    { "undo", file_undo, NULL, NULL, NULL },
    { "rendo", file_rendo, NULL, NULL, NULL },

    { "docs", show_documentation, NULL, NULL, NULL },
    { "keyshortcuts", show_keyboard_shortcuts, NULL, NULL, NULL },
    { "about", about_hyperion, NULL, NULL, NULL },

    { "preferences", open_preferences, NULL, NULL, NULL },
};

static void hyperion_app_startup(GApplication* app) {
    const char* quit_accels[2] = { "<Ctrl>Q", NULL };
    const char* open_accels[2] = { "<Ctrl>O", NULL };
    const char* create_file_accels[2] = { "<Ctrl>N", NULL };
    const char* create_window_accels[2] = { "<Ctrl><Shift>N", NULL };
    const char* save_accels[2] = { "<Ctrl>S", NULL };
    const char* save_as_accels[2] = { "<Ctrl><Shift>S", NULL };
    const char* close_editor_accels[2] = { "<Ctrl>W", NULL };
    const char* close_window_accels[2] = { "<Alt>F4", NULL };

    const char* undo_accels[2] = { "<Ctrl>Z", NULL };
    const char* rendo_accels[2] = { "<Ctrl>Y", NULL };

    const char* show_documentation_accels[2] = { "<Ctrl><Shift>D", NULL };
    const char* show_keyshortcuts_accels[2] = { "<Ctrl><Shift>K", NULL };


    G_APPLICATION_CLASS(hyperion_app_parent_class)->startup(app);
    g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), app);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.quit", quit_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.open", open_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.newfile", create_file_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.newwindow", create_window_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.save", save_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.saveas", save_as_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.closeeditor", close_editor_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.winclose", close_window_accels);

    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.undo", undo_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.rendo", rendo_accels);

    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.docs", show_documentation_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.keyshortcuts", show_keyshortcuts_accels);


    GtkBuilder* builder = gtk_builder_new_from_file("./ui/menubar.ui");
    GMenuModel* menubar = G_MENU_MODEL(gtk_builder_get_object(builder, "menubar"));
	gtk_application_set_menubar(GTK_APPLICATION(app), menubar);
    g_object_unref(builder);
}

static void hyperion_app_activate(GApplication *app)
{
    HyperionAppWindow* win;
    win = hyperion_app_window_new(HYPERION_APP(app));
    gtk_widget_show(GTK_WIDGET(win));
}

static void hyperion_app_open(GApplication* app, GFile** files, int n_files, const char* hint)
{
    GList* windows;
    HyperionAppWindow* win;
    int i;

    windows = gtk_application_get_windows(GTK_APPLICATION(app));
    if (windows)
        win = HYPERION_APP_WINDOW(windows->data);
    else
        win = hyperion_app_window_new(HYPERION_APP(app));
    
    gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(win), TRUE);

    for (i = 0; i < n_files; i++)
        hyperion_app_window_open(win, files[i]);

    gtk_widget_show(GTK_WIDGET(win));
}

static void hyperion_app_class_init(HyperionAppClass* class)
{
    G_APPLICATION_CLASS(class)->startup = hyperion_app_startup;
    G_APPLICATION_CLASS(class)->activate = hyperion_app_activate;
    G_APPLICATION_CLASS(class)->open = hyperion_app_open;
}


HyperionApp* hyperion_app_new(void)
{
    return g_object_new(HYPERION_APP_TYPE, "application-id", "org.hyperion.editor", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}