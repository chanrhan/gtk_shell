#include "view.h"

char cwd[32] = "/tmp/test";
file_info_t file_list[FILE_DISPLAY_LIMIT];
GtkWidget* label_data[8];
GtkWidget* dir_text;

int main(int argc, char** argv){
    printf("START VIEW\n");
    req_msg_q_id = create_msg_q("keyfile", 1);
    res_msg_q_id = create_msg_q("keyfile", 2);
    gtk_init(&argc, &argv);
    css_init();
    GtkWidget *window = create_gtk_main_window();
    build_layout(window);

    g_signal_connect(window, "realize", G_CALLBACK(on_realize), NULL);
    g_idle_add(on_realize, NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
