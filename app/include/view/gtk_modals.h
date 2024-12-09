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

gtk_modal_mkdir mkdir_modal;

#endif