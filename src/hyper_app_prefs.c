#include <gtk/gtk.h>

#include "../include/hyper_app.h"
#include "../include/hyper_app_win.h"
#include "../include/hyper_app_prefs.h"


G_DEFINE_TYPE(HyperionAppPrefs, hyperion_app_prefs, GTK_TYPE_DIALOG)

static void hyperion_app_prefs_init(HyperionAppPrefs* prefs)
{
	gtk_widget_init_template(GTK_WIDGET(prefs));
	prefs->settings = g_settings_new("org.inc.hyperion");

	g_settings_bind(prefs->settings, "font", prefs->font, "font", G_SETTINGS_BIND_DEFAULT);
	g_settings_bind(prefs->settings, "transition", prefs->transition, "active-id", G_SETTINGS_BIND_DEFAULT);
}

static void hyperion_app_prefs_dispose(GObject* object)
{
	HyperionAppPrefs* prefs;
	prefs = HYPERION_APP_PREFS(object);
	g_clear_object(&prefs->settings);
	G_OBJECT_CLASS(hyperion_app_prefs_parent_class)->dispose(object);
}

static void hyperion_app_prefs_class_init(HyperionAppPrefsClass* class)
{
	G_OBJECT_CLASS(class)->dispose = hyperion_app_prefs_dispose;

	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/org/hyperion/editor/preferences.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppPrefs, font);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), HyperionAppPrefs, transition);
}

HyperionAppPrefs* hyperion_app_prefs_new(HyperionAppWindow* win)
{
	return g_object_new(HYPERION_APP_PREFS_TYPE, "transient-for", win, "use-header-bar", TRUE, NULL);
}
