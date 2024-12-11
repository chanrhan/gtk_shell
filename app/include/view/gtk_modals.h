#ifndef GTK_MODALS_H
#define GTK_MODALS_H

#include "view_common.h"

typedef struct gtk_modal_mkdir
{
    int type;
    char filename_error[32];
    GtkWidget* window;
    GtkWidget* select_type;
    GtkWidget* inp_filename;
    GtkWidget* inp_error_label;
    GtkWidget* perm_check_box[9];
    GtkWidget* btn_close;
    GtkWidget* btn_submit;
} gtk_modal_mkdir;

typedef struct gtk_file_menu_context{
    GtkMenu* menu;
    GtkWidget* item_rename;
    GtkWidget* item_modify;
    GtkWidget* item_move;
    GtkWidget* item_remove;
    GtkWidget* item_copy;
    GtkWidget* item_execute;
    GtkWidget* item_mklink;
} gtk_file_menu_context;

typedef struct gtk_dir_menu_context{
    GtkMenu* menu;
    GtkWidget* item_rename;
    GtkWidget* item_move;
    GtkWidget* item_remove;
    GtkWidget* item_copy;
} gtk_dir_menu_context;

gtk_modal_mkdir mkdir_modal;
gtk_file_menu_context file_menu_context;
gtk_dir_menu_context dir_menu_context;

void init_modal(GtkWidget* parent);

#endif