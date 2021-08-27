#ifndef __HYPERION_APP_WIN_H
#define __HYPERION_APP_WIN_H

#include <gtk/gtk.h>
#include "hyper_app.h"


#define HYPERION_APP_WINDOW_TYPE (hyperion_app_window_get_type())
G_DECLARE_FINAL_TYPE(HyperionAppWindow, hyperion_app_window, HYPERION, APP_WINDOW, GtkApplicationWindow)


struct _HyperionAppWindow
{
    GtkApplicationWindow parent;

    GSettings* settings;
    GtkWidget* stack;
    GtkWidget* stack_switcher;
    GtkWidget* welcome_label;
    GtkWidget* box;
    // GtkWidget* header;
    GtkWidget* overlay;
    GtkWidget* gears;
    GtkWidget* search;
    GtkWidget* searchbar;
    GtkWidget* searchentry;
    GtkWidget* sidebar;
    GtkWidget* words;
};

//
HyperionAppWindow* hyperion_app_window_new(HyperionApp* app);

//
void hyperion_app_window_open(HyperionAppWindow* win, GFile* file);

//
void hyperion_app_window_close(HyperionAppWindow* win);


#endif /* __HYPERION_APP_WIN_H */