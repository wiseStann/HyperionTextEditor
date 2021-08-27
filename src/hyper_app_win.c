#include <gtk/gtk.h>

#include "../include/hyper_app.h"
#include "../include/hyper_app_win.h"




/* TODO:
    make undo redo operations in menubar and add keyboard shortcuts
    make window appear on the center of the screen
    scrollable stackswitcher
    ability to open multiple files using GtkFileChooser
    change color of StackSwitcher tabs
    add functionality to window footer
*/





G_DEFINE_TYPE(HyperionAppWindow, hyperion_app_window, GTK_TYPE_APPLICATION_WINDOW)

static void search_text_changed(GtkEntry* entry, HyperionAppWindow* win)
{
    const char* text;
    GtkWidget* tab;
    GtkWidget* view;
    GtkTextBuffer* buffer;
    GtkTextIter start, match_start, match_end;

    text = gtk_editable_get_text(GTK_EDITABLE(entry));

    if (text[0] == '\0')
        return;

    tab = gtk_stack_get_visible_child(GTK_STACK(win->stack));
    view = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(tab));
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

    gtk_text_buffer_get_start_iter(buffer, &start);
    if (gtk_text_iter_forward_search(&start, text, GTK_TEXT_SEARCH_CASE_INSENSITIVE, &match_start, &match_end, NULL))
    {
        gtk_text_buffer_select_range(buffer, &match_start, &match_end);
        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(view), &match_start, 0.0, FALSE, 0.0, 0.0);
    }
}

static void find_word(GtkButton* button, HyperionAppWindow* win)
{
    const char* word;
    word = gtk_button_get_label(button);
    gtk_editable_set_text(GTK_EDITABLE(win->searchentry), word);
}

static void update_words(HyperionAppWindow* win)
{
    GHashTable* strings;
    GHashTableIter iter;
    GtkWidget* tab, *view, *row;
    GtkTextBuffer* buffer;
    GtkTextIter start, end;
    char* word, *key;
    GtkWidget* child;

    tab = gtk_stack_get_visible_child(GTK_STACK(win->stack));

    if (tab == NULL)
        return;

    view = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(tab));
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

    strings = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    gtk_text_buffer_get_start_iter(buffer, &start);
    while (!gtk_text_iter_is_end(&start))
    {
        while (!gtk_text_iter_starts_word(&start))
        {
            if (!gtk_text_iter_forward_char(&start))
                goto done;
        }
        end = start;
        if (!gtk_text_iter_forward_word_end(&end))
            goto done;
        word = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        g_hash_table_add(strings, g_utf8_strdown(word, -1));
        g_free(word);
        start = end;
    }

done:
    while ((child = gtk_widget_get_first_child(win->words)))
    gtk_list_box_remove(GTK_LIST_BOX(win->words), child);

    g_hash_table_iter_init(&iter, strings);
    while (g_hash_table_iter_next(&iter, (gpointer *)&key, NULL))
    {
        row = gtk_button_new_with_label(key);
        g_signal_connect(row, "clicked", G_CALLBACK(find_word), win);
        gtk_list_box_insert(GTK_LIST_BOX(win->words), row, -1);
    }

    g_hash_table_unref(strings);
}

static void visible_child_changed(GObject* stack, GParamSpec* pspec, HyperionAppWindow* win)
{
    if (gtk_widget_in_destruction(GTK_WIDGET(stack)))
        return;

    gtk_search_bar_set_search_mode(GTK_SEARCH_BAR(win->searchbar), FALSE);
    update_words(win);
}

static void words_changed(GObject* sidebar, GParamSpec* pspec, HyperionAppWindow* win)
{
    update_words(win);
}

void css_set(GtkCssProvider* cssProvider, GtkWidget* widget)
{
    GtkStyleContext* context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void hyperion_app_window_init(HyperionAppWindow* win)
{
    GtkBuilder* builder;
    GMenuModel* menu;
    GAction* action;
    // PangoAttrList* attrs_list;
    // PangoAttribute* attribute;

    // This text was written through the Hyperion Text Editor

    gtk_widget_init_template(GTK_WIDGET(win));
    
    win->welcome_label = gtk_label_new("Welcome to the HYPERION Text Editor!\n\n"
                                       "New file:  <Ctrl> + N\n"
                                       "Open file:  <Ctrl> + O\n"
                                       "New window:  <Ctrl> + <Shift> + N");
    gtk_widget_set_name(win->welcome_label, "welcome_label");

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "./styles/application.css");
    css_set(cssProvider, GTK_WIDGET(win));
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    // attrs_list = pango_attr_list_new();
    // attribute = pango_attr_size_new(20 * PANGO_SCALE);
    // pango_attr_list_insert(attrs_list, attribute);
    // gtk_label_set_attributes(GTK_LABEL(win->welcome_label), attrs_list);

    // win->header = gtk_header_bar_new();

    win->overlay = gtk_overlay_new();
    gtk_overlay_set_child(GTK_OVERLAY(win->overlay), win->welcome_label);

    win->stack = gtk_stack_new();
    gtk_widget_set_name(win->stack, "win_stack");
    win->stack_switcher = gtk_stack_switcher_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(win->stack_switcher), GTK_STACK(win->stack));
    gtk_widget_set_hexpand(win->stack_switcher, TRUE);

    win->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(win->box), win->stack_switcher);
    gtk_box_append(GTK_BOX(win->box), win->stack);

    builder = gtk_builder_new_from_file("./ui/gears-menu.ui");
    menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(win->gears), menu);
    g_object_unref(builder);

    win->settings = g_settings_new("org.hyperion.editor");

    g_settings_bind(win->settings, "transition", win->stack, "transition-type", G_SETTINGS_BIND_DEFAULT);

    g_settings_bind(win->settings, "show-words", win->sidebar, "reveal-child", G_SETTINGS_BIND_DEFAULT);
    g_signal_connect(win->sidebar, "notify::reveal-child", G_CALLBACK(words_changed), win);

    g_object_bind_property(win->search, "active", win->searchbar, "search-mode-enabled", G_BINDING_BIDIRECTIONAL);

    action = g_settings_create_action(win->settings, "show-words");
    g_action_map_add_action(G_ACTION_MAP(win), action);
    g_object_unref(action);
}

static void hyperion_app_window_dispose(GObject* object)
{
    HyperionAppWindow* win;
    win = HYPERION_APP_WINDOW(object);
    g_clear_object(&win->settings);
    G_OBJECT_CLASS(hyperion_app_window_parent_class)->dispose(object);
}

static void hyperion_app_window_class_init(HyperionAppWindowClass* class)
{
    G_OBJECT_CLASS(class)->dispose = hyperion_app_window_dispose;

    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/org/hyperion/editor/window.ui");

    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppWindow, stack);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppWindow, gears);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppWindow, search);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppWindow, searchbar);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppWindow, searchentry);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppWindow, words);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppWindow, sidebar);

    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), search_text_changed);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), visible_child_changed);
}

HyperionAppWindow* hyperion_app_window_new(HyperionApp* app)
{
    HyperionAppWindow* window;

    window = g_object_new(HYPERION_APP_WINDOW_TYPE, "application", app, NULL);
	gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(window), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(window), 1500, 800);
    gtk_window_set_child(GTK_WINDOW(window), window->overlay);

    return window;
}

void message_dialog_new(GtkWindow* win, const char* title, const char* message)
{
    GtkWidget* dialog, *grid, *msg_label, *ok_label, *button;
    PangoAttrList* attrs_list;
    PangoAttribute* attribute;

    dialog = gtk_dialog_new();

    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(win));
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_widget_set_size_request(dialog, 400, 150);
    
    msg_label = gtk_label_new(message);
    ok_label = gtk_label_new("OK");

    attrs_list = pango_attr_list_new();
    attribute = pango_attr_size_new(13 * PANGO_SCALE);
    pango_attr_list_insert(attrs_list, attribute);
    gtk_label_set_attributes(GTK_LABEL(msg_label), attrs_list);
    gtk_label_set_attributes(GTK_LABEL(ok_label), attrs_list);

    button = gtk_button_new();
    gtk_button_set_child(GTK_BUTTON(button), ok_label);

    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), msg_label, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    gtk_window_set_child(GTK_WINDOW(dialog), grid);

    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), dialog);

    gtk_widget_show(GTK_WIDGET(dialog));
}

const char* stack_get_last_item_name(HyperionAppWindow* win)
{
    GtkSelectionModel* pages;
    guint pages_number;
    GtkStackPage* last_added_page;
    const char* last_added_page_name;

    pages = gtk_stack_get_pages(GTK_STACK(win->stack));
    pages_number = g_list_model_get_n_items(G_LIST_MODEL(pages));
    last_added_page = g_list_model_get_item(G_LIST_MODEL(pages), pages_number - 1);

    if (last_added_page) {
        last_added_page_name = gtk_stack_page_get_name(last_added_page);
        return last_added_page_name;
    }
    return NULL;
}

void hyperion_app_window_open(HyperionAppWindow* win, GFile* file)
{
    char* basename, *path, *contents;
    GtkWidget* scrolled, *view;
    gsize length;
    GtkTextBuffer* buffer;
    GtkTextTag* tag;
    GtkTextIter start_iter, end_iter;

    basename = g_file_get_basename(file);
    path = g_file_get_path(file);

    gtk_overlay_add_overlay(GTK_OVERLAY(win->overlay), win->box);

    if (g_file_load_contents(file, NULL, &contents, &length, NULL, NULL))
    {
        if (g_utf8_validate(contents, length, NULL)) {
            scrolled = gtk_scrolled_window_new();

            gtk_widget_set_hexpand(scrolled, TRUE);
            gtk_widget_set_vexpand(scrolled, TRUE);
            
            view = gtk_text_view_new();
            gtk_text_view_set_editable(GTK_TEXT_VIEW(view), TRUE);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), TRUE);

            gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 10);
            gtk_text_view_set_top_margin(GTK_TEXT_VIEW(view), 10);

            gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), view);
            gtk_widget_set_parent(scrolled, win->stack);
            gtk_stack_add_titled(GTK_STACK(win->stack), scrolled, path, basename);

            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
            gtk_text_buffer_set_text(buffer, contents, length);
            g_free(contents);

            tag = gtk_text_buffer_create_tag(buffer, NULL, NULL);
            g_settings_bind(win->settings, "font", tag, "font", G_SETTINGS_BIND_DEFAULT);

            gtk_text_buffer_get_start_iter(buffer, &start_iter);
            gtk_text_buffer_get_end_iter(buffer, &end_iter);
            gtk_text_buffer_apply_tag(buffer, tag, &start_iter, &end_iter);

            gtk_stack_set_visible_child_name(GTK_STACK(win->stack), path);
        } else {
            message_dialog_new(GTK_WINDOW(win), "File Opening Error", "Can't open the file: the file is not UTF-8 encoded");
        }
    }
    gtk_widget_set_sensitive(win->search, TRUE);
    update_words(win);

    g_free(basename);
}

void hyperion_app_window_close(HyperionAppWindow* win)
{
    GtkWidget* visible_file;
    const char* last_added_page_name;
    
    visible_file = gtk_stack_get_visible_child(GTK_STACK(win->stack));
    if (visible_file) {
        gtk_widget_set_parent(visible_file, win->stack);
        gtk_stack_remove(GTK_STACK(win->stack), visible_file);
    }
    
    last_added_page_name = stack_get_last_item_name(win);
    if (last_added_page_name)
        gtk_stack_set_visible_child_name(GTK_STACK(win->stack), last_added_page_name);
}