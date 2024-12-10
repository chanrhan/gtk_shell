#ifndef GTK_UTILS_H
#define GTK_UTILS_H

#include "view_common.h"

#define DECLARE_GTK_CALLBACK(str) int g_callback_##str(GtkWidget* widget, gpointer data)

DECLARE_GTK_CALLBACK(mvdir);
DECLARE_GTK_CALLBACK(quit);
DECLARE_GTK_CALLBACK(mkdir_popup_open);
DECLARE_GTK_CALLBACK(mkdir_popup_type_changed);
DECLARE_GTK_CALLBACK(mkdir_popup_submit);
DECLARE_GTK_CALLBACK(mvdir_toolbar);

int gtk_exit(GtkWidget* widget, gpointer data);

gboolean on_enter_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);

gboolean on_leave_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);

void on_popup_close(GtkWidget *widget, gpointer user_data);

#endif
