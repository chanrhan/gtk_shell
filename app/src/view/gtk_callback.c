#include "gtk_callback.h"
#include "gtk_modals.h"
#include "gtk_utils.h"
#include "view_func.h"
#include "ipc_view.h"

int get_clicked_index(GtkWidget* widget, gpointer data){
    int *index = (int *)g_object_get_data(G_OBJECT(widget), "index");
    return *index;
}

file_info_t get_clicked_file_info(GtkWidget* widget, gpointer data){
    int *index = (int *)g_object_get_data(G_OBJECT(widget), "index");
    return file_list[*index];
}

int g_callback_quit(GtkWidget* widget, gpointer data){
    req_msg_t req;
    req.cmd = CMD_QUIT;
    // printf("send..\n");
    send_to_server(&req);
    gtk_exit(widget, data);
    return 0;
}

int g_callback_item_clicked(GtkWidget* widget, GdkEventButton *event, gpointer data){
    int index = get_clicked_index(widget, data);
    char* filename = file_list[index].name;
    if(event->type  == GDK_BUTTON_PRESS && event->button == 3){
        printf("right click\n");
        int type = file_list[index].type;
        GtkMenu* target_menu_context;
        if(type == 4){
            target_menu_context = dir_menu_context.menu;
        }else{
            target_menu_context = file_menu_context.menu;
        }
        selected_index = index;
        gtk_menu_popup_at_pointer(target_menu_context, (GdkEvent *)event);
    }else{
        move_directory(filename);
    }

    return TRUE;
}

int g_callback_mkdir_popup_open(GtkWidget* widget, gpointer data){
    // printf("menu open\n");
    int bit[9] = {1,1,1,1,1,1,1,1,1};
    for(int i=0;i<9;++i){
        if(bit[i] == 1){
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mkdir_modal.perm_check_box[i]), TRUE);
        }
    }
    gtk_widget_show_all(mkdir_modal.window);

    return TRUE; 
}


int g_callback_mkdir_popup_submit(GtkWidget* widget, gpointer data){
    make_dir();
    return TRUE;
}

int g_callback_mvdir_toolbar(GtkWidget* widget, gpointer data){
    
}

gboolean on_realize(gpointer data){
    printf("realize\n");
    move_directory(NULL);
    return FALSE;
}


int gtk_exit(GtkWidget* widget, gpointer data){
    printf("QUIT\n");
    // msgctl(req_msg_q_id, IPC_RMID, (struct msqid_ds*)NULL);
    msgctl(res_msg_q_id, IPC_RMID, (struct msqid_ds*)NULL);

    gtk_main_quit();

    return 0;
}

gboolean on_enter_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data) {
    gtk_widget_set_state_flags(widget, GTK_STATE_FLAG_PRELIGHT, TRUE);
    return FALSE;
}

gboolean on_leave_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data) {
    gtk_widget_unset_state_flags(widget, GTK_STATE_FLAG_PRELIGHT);
    return FALSE;
}

void on_popup_close(GtkWidget *widget, gpointer data) {
    GtkWidget *popup = GTK_WIDGET(data);
    gtk_widget_hide(popup);  // gtk_widget_destroy 시 위젯이 완전히 삭제되기 때문에 hide 사용 
}

// menu callback: file option
int g_callback_file_rename(GtkWidget* widget, gpointer data){

}

int g_callback_file_modify(GtkWidget* widget, gpointer data){
    
}

int g_callback_file_move(GtkWidget* widget, gpointer data){
    
}

int g_callback_file_copy(GtkWidget* widget, gpointer data){
    
}

int g_callback_file_execute(GtkWidget* widget, gpointer data){
    
}
int g_callback_file_remove(GtkWidget* widget, gpointer data){
    
}

int g_callback_file_mklink(GtkWidget* widget, gpointer data){
    
}

// menu callback: dir
int g_callback_dir_rename(GtkWidget* widget, gpointer data){
    
}

int g_callback_dir_move(GtkWidget* widget, gpointer data){
    
}

int g_callback_dir_copy(GtkWidget* widget, gpointer data){
    
}

int g_callback_dir_remove(GtkWidget* widget, gpointer data){
    remove_dir(file_list[selected_index].name);
}