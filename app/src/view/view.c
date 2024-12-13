#include "gtk_draw.h"
#include "view_func.h"


char cwd[MAX_PATH_LEN] = "/tmp/test";
file_info_t file_list[MAX_FILE_LIST_SIZE];
GtkWidget* label_data[MAX_FILE_LIST_SIZE];
GtkWidget* dir_text;

file_info_t copied_file;
GtkWidget* copied_file_label;
GtkLabel* dialog_label;
GtkWidget* search_inp;

GtkWidget* path_tok_label[MAX_PATH_TOKEN];
char path_tok[32][MAX_PATH_LEN];
int path_tok_map[10];

char found_filename[MAX_PATH_LEN];

int selected_index = 0;
int edit_mode = 0; 
int copy_mode = -1;
int file_list_size = 0;

int display_mode = DISPLAY_FILE;

void init(){
    
}

int main(int argc, char** argv){
    

    printf("START VIEW\n");
    req_msg_q_id = create_msg_q("keyfile", MSG_Q_REQ_PROJ_ID);
    res_msg_q_id = create_msg_q("keyfile", MSG_Q_RES_PROJ_ID);
    gtk_init(&argc, &argv);
    css_init();
    GtkWidget *window = create_gtk_main_window();
    build_layout(window);

    init();
    g_idle_add(on_realize, NULL);

    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}
