#ifndef __HYPERION_APP_EDITOR_H
#define __HYPERION_APP_EDITOR_H

#include <gtk/gtk.h>
#include <string.h>

#define HYPERION_APP_TYPE (hyperion_app_get_type())
G_DECLARE_FINAL_TYPE(HyperionApp, hyperion_app, HYPERION, APP, GtkApplication)


//
static void hyperion_app_activate(GApplication *app);

//
HyperionApp* hyperion_app_new(void);


#endif // !__HYPERION_APP_EDITOR_H