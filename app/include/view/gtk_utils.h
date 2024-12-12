#ifndef GTK_UTILS_H
#define GTK_UTILS_H

#include "view_common.h"

int search_file();

int submit_file_info();

int make_dir();

int make_file();

int rename_file();

int remove_dir(char* filename);

int remove_file(char* filename);

int move_file();

int copy_file();

int paste_file();

int execute_file(char* filename);

int link_file();

#endif
