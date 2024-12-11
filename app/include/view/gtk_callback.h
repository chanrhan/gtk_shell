#ifndef GTK_CALLBACK_H
#define GTK_CALLBACK_H

#include "view_common.h"

#define DECLARE_GTK_CALLBACK(str) int g_callback_##str(GtkWidget* widget, gpointer data)

// DECLARE_GTK_CALLBACK(mvdir);
DECLARE_GTK_CALLBACK(quit);
DECLARE_GTK_CALLBACK(mkdir_popup_open);
DECLARE_GTK_CALLBACK(dir_info_open);
DECLARE_GTK_CALLBACK(file_info_open);

// modal
DECLARE_GTK_CALLBACK(mkdir_popup_type_changed);
DECLARE_GTK_CALLBACK(mkdir_popup_submit);


DECLARE_GTK_CALLBACK(mvdir_toolbar);

// menu: file option
DECLARE_GTK_CALLBACK(file_copy);
DECLARE_GTK_CALLBACK(file_rename);
DECLARE_GTK_CALLBACK(file_modify);
DECLARE_GTK_CALLBACK(file_move);
DECLARE_GTK_CALLBACK(file_remove);
DECLARE_GTK_CALLBACK(file_execute);
DECLARE_GTK_CALLBACK(file_mklink);

// menu: dir option
DECLARE_GTK_CALLBACK(dir_remove);


int g_callback_item_clicked(GtkWidget* widget, GdkEventButton *event, gpointer data);

int gtk_exit(GtkWidget* widget, gpointer data);

gboolean on_enter_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);

gboolean on_leave_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);

void on_popup_close(GtkWidget *widget, gpointer user_data);

#endif