#ifndef VIEW_COMMON_H
#define VIEW_COMMON_H

#include <gtk/gtk.h>
#include "util.h"
#include "view_types.h"
#include "ipc.h"
#include "ipc_types.h"

extern int req_msg_q_id;
extern int res_msg_q_id;

extern char cwd[MAX_PATH_LEN];

extern file_info_t file_list[MAX_FILE_LIST_SIZE];
extern int file_list_size;
extern GtkWidget* label_data[MAX_FILE_LIST_SIZE];
extern GtkWidget* dir_text;
extern int selected_index; // selected file index
extern int edit_mode; // file info mode

extern int copy_mode; // copy or move

extern GtkWidget* copied_file_label;
extern GtkLabel* dialog_label;
extern GtkWidget* search_inp;
extern GtkWidget* path_tok_label[MAX_PATH_TOKEN];
extern char path_tok[32][MAX_PATH_LEN];
extern int path_tok_map[10];

extern char found_filename[MAX_PATH_LEN];

extern file_info_t copied_file;

// GtkWidget* mkdir_popup;


#endif
