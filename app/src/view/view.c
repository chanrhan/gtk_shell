#include "gtk_draw.h"
#include "view_func.h"


char cwd[32] = "/tmp/test";
file_info_t file_list[FILE_DISPLAY_LIMIT];
GtkWidget* label_data[8];
GtkWidget* dir_text;

int selected_index = 0;

int main(int argc, char** argv){
    printf("START VIEW\n");
    req_msg_q_id = create_msg_q("keyfile", MSG_Q_REQ_PROJ_ID);
    res_msg_q_id = create_msg_q("keyfile", MSG_Q_RES_PROJ_ID);
    gtk_init(&argc, &argv);
    css_init();
    GtkWidget *window = create_gtk_main_window();
    build_layout(window);

    g_idle_add(on_realize, NULL);

    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}
