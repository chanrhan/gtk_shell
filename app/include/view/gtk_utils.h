#ifndef GTK_UTILS_H
#define GTK_UTILS_H

#include "view_common.h"

int make_dir();

int rename_dir();

int remove_dir(char* filename);

int move_file(char* source, char* dest);

int copy_file(char* source, char* dest);

int execute_file(char* filename);

int link_file(char* source, char* dest);

#endif
