#ifndef VIEW_TYPES_H
#define VIEW_TYPES_H

#define EDIT_MODE_MAKE 0
#define EDIT_MODE_RENAME 1
#define EDIT_MODE_LINK 2
#define EDIT_MODE_FILE 3

#define MAX_PATH_TOKEN 10

#define FILE_DISPLAY_MODE 150
#define PROCESS_DISPLAY_MODE 151

#define CLASS_INFORM "inform"
#define CLASS_ERROR "error"

typedef struct g_signal_data_t{
    int number;
} g_signal_data_t;

#endif