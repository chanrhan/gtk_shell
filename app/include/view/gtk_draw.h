#ifndef GTK_DRAW_H
#define GTK_DRAW_H

#include "gtk_utils.h"
#include "gtk_modals.h"

void set_text_current_dir(char* new_cwd);

void update_directory(res_msg_t res);

GtkWidget *create_gtk_main_window();

void css_init();

void build_layout(GtkWidget* window);

#endif
