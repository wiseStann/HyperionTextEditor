#include <gtk/gtk.h>

#include "include/hyper_app.h"

int main (int argc, char *argv[])
{
	return g_application_run(G_APPLICATION(hyperion_app_new()), argc, argv);
}