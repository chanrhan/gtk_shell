#ifndef VIEW_COMMON_H
#define VIEW_COMMON_H

#include <gtk/gtk.h>
#include "view_types.h"
#include "ipc.h"
#include "ipc_types.h"

extern char cwd[32];

extern file_info_t file_list[FILE_DISPLAY_LIMIT];
extern GtkWidget* label_data[8];
extern GtkWidget* dir_text;
extern int selected_index; // selected file index
extern int edit_mode;

extern GtkWidget* copied_file_label;

extern file_info_t copied_file;

// GtkWidget* mkdir_popup;


#endif
