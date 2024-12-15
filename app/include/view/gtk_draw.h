#ifndef GTK_DRAW_H
#define GTK_DRAW_H

#include "view_common.h"

void set_clipboard(char* text);
void clear_clipboard();

void update_current_working_directory(char* new_cwd);

void update_file_list(res_msg_t res);

void update_process_list(res_msg_t res);

void show_dialog_text(char* msg);

GtkWidget *create_gtk_main_window();

void css_init();

void css_reload();

void build_layout(GtkWidget* window);

void gtk_style_class_toggle(GtkWidget* widget, char* classname, gboolean flag);

void inform_dialog(char* msg);
void error_dialog(char* msg);

#endif
