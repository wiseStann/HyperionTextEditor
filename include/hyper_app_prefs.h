#ifndef __HYPERION_APP_PREFS_H
#define __HYERION_APP_PREFS_H

#include <gtk/gtk.h>
#include "hyper_app_win.h"

#define HYPERION_APP_PREFS_TYPE (hyperion_app_prefs_get_type ())
G_DECLARE_FINAL_TYPE(HyperionAppPrefs, hyperion_app_prefs, HYPERION, APP_PREFS, GtkDialog)

struct _HyperionAppPrefs
{
	GtkDialog parent;
	
	GSettings* settings;
	GtkWidget* font;
	GtkWidget* transition;
};

HyperionAppPrefs* hyperion_app_prefs_new(HyperionAppWindow *win);


#endif /* __HYPERION_APP_PREFS_H */