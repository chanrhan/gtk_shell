#ifndef GTK_UTILS_H
#define GTK_UTILS_H

#include "view_common.h"

int submit_file_info();

int make_dir();

int make_file();

int rename_file();

int remove_dir(char* filename);

int remove_file(char* filename);

int move_file(char* source, char* dest);

int copy_file();

int paste_file(char* path);

int execute_file(char* filename);

int link_file(char* path);

#endif
