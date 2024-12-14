#ifndef VIEW_FUNC_H
#define VIEW_FUNC_H

#include "view_common.h"

gboolean on_realize(gpointer data);

int move_directory(char* append);

int move_full_directory(char* path);

int show_process();

void toggle_display_mode();

void get_time_format(char* time, char* out);


#endif

