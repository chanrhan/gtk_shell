#ifndef GTK_MODAL_H
#define GTK_MODAL_H

#include "view_common.h"

typedef struct gtk_modal_file_info
{
    int type;
    char filename_error[32];
    GtkWidget* window;
    GtkWidget* select_type;
    GtkWidget* inp_filename;
    GtkWidget* check_box_symbol;
    GtkWidget* inp_error_label;
    GtkWidget* perm_check_box[9];
    GtkWidget* btn_close;
    GtkWidget* btn_submit;
} gtk_modal_file_info;

typedef struct gtk_modal_file_detail
{
    int type;
    GtkWidget* window;
    GtkWidget* inp_filename;
    GtkWidget* check_box_symbol;
    GtkWidget* inp_error_label;
    GtkWidget* perm_check_box[9];
    GtkWidget* btn_close;
} gtk_modal_file_detail;

typedef struct gtk_context_menu_file_option{
    GtkMenu* menu;
    GtkWidget* item_rename;
    GtkWidget* item_modify;
    GtkWidget* item_move;
    GtkWidget* item_remove;
    GtkWidget* item_copy;
    GtkWidget* item_execute;
    GtkWidget* item_mklink;
} gtk_context_menu_file_option;

typedef struct gtk_context_menu_dir_option{
    GtkMenu* menu;
    GtkWidget* item_rename;
    GtkWidget* item_move;
    GtkWidget* item_remove;
    GtkWidget* item_copy;
} gtk_context_menu_dir_option;

typedef struct gtk_context_menu_bg_option{
    GtkMenu* menu;
    GtkWidget* item_paste;
    GtkWidget* item_create_dir;
    GtkWidget* item_create_file;
    GtkWidget* item_open_terminal;
} gtk_context_menu_bg_option;

typedef struct gtk_modal_text_editor{
    GtkWidget* window;
    char filename[MAX_PATH_LEN];
    GtkWidget* textview;
}gtk_modal_text_editor;

gtk_modal_file_info md_mkdir;
gtk_modal_file_detail md_file_detail;
gtk_context_menu_file_option ctxm_file_option;
gtk_context_menu_dir_option ctxm_dir_option;
gtk_context_menu_bg_option ctxm_bg_option;
gtk_modal_text_editor md_text_editor;

void init_modal(GtkWidget* parent);

#endif