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

// GtkWidget* mkdir_popup;


#endif
