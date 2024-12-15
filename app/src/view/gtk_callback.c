#include "gtk_callback.h"
#include "gtk_modal.h"
#include "gtk_utils.h"
#include "view_func.h"
#include "ipc_view.h"
#include "gtk_draw.h"

int get_clicked_index(GtkWidget* widget){
    int *index = (int *)g_object_get_data(G_OBJECT(widget), "index");
    return *index;
}

file_info_t get_clicked_file_info(GtkWidget* widget){
    int *index = (int *)g_object_get_data(G_OBJECT(widget), "index");
    return file_list[*index];
}

int g_callback_change_display_mode(GtkWidget* widget, gpointer data){
    toggle_display_mode();
}

gboolean on_enter_notify_path_tok(GtkWidget *widget, GdkEventCrossing *event, gpointer data){
    if(display_mode == FILE_DISPLAY_MODE){
        gtk_widget_set_state_flags(widget, GTK_STATE_FLAG_PRELIGHT, TRUE);
    }
}

gboolean on_leave_notify_path_tok(GtkWidget *widget, GdkEventCrossing *event, gpointer data){
    if(display_mode == FILE_DISPLAY_MODE){
        gtk_widget_unset_state_flags(widget, GTK_STATE_FLAG_PRELIGHT);
    }
}

// hover in
gboolean on_enter_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer data) {
    int index = get_clicked_index(widget);
    if(index >= file_list_size){
        return TRUE;
    }
    if(selected_index != -1){
    }
        gtk_widget_set_state_flags(widget, GTK_STATE_FLAG_PRELIGHT, TRUE);
    return FALSE;
}

// hover out
gboolean on_leave_notify(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data) {
    gtk_widget_unset_state_flags(widget, GTK_STATE_FLAG_PRELIGHT);
    return FALSE;
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
    int index = get_clicked_index(widget);
    char* filename = file_list[index].name;
    // printf("clicked:%d\n", index);
    
    if(display_mode == FILE_DISPLAY_MODE){
        if(event->type  == GDK_BUTTON_PRESS && event->button == 3){
            printf("right click\n");
            GtkMenu* target_menu_context;
            if(index >= file_list_size){ // background
                selected_index = -1;
                gtk_widget_set_sensitive(ctxm_bg_option.item_paste, (copy_mode == -1) ? FALSE : TRUE);
                target_menu_context = ctxm_bg_option.menu;
            }else{
                int type = file_list[index].type;
                if(type == 4){
                    target_menu_context = ctxm_dir_option.menu;
                }else{
                    target_menu_context = ctxm_file_option.menu;
                }
                selected_index = index;
            }
            gtk_menu_popup_at_pointer(target_menu_context, (GdkEvent *)event);
        }else{
            //  printf("111:%s\n", filename);
            if(filename[0] != '\0'){
            //  printf("222\n");
                switch (file_list[index].type)
                {
                case 4:
            //  printf("333\n");
                    move_directory(filename);
                    break;
                case 8:
                    read_file(filename);
                    break;
                default:
                    break;
                }
            }
            
            selected_index = -1;
        }
    }else{
        // DISPLAY PROCESS MODE

    }
    

    return TRUE;
}

int g_callback_mkdir_popup_open(GtkWidget* widget, gpointer data){
    edit_mode = EDIT_MODE_MAKE;
    gtk_window_set_title(GTK_WINDOW(md_mkdir.window), "CREATE DIRECTORY");
    gtk_widget_show_all(md_mkdir.window);

    return TRUE; 
}

// modify 
int g_callback_file_info_open(GtkWidget* widget, gpointer data){
    printf("read\n");
    edit_mode = EDIT_MODE_FILE;
    // selected_index = get_clicked_index(widget);
    gtk_widget_show_all(md_mkdir.window);
}

int g_callback_open_file_detail(GtkWidget* widget, gpointer data){
    open_file_detail();
}

int g_callback_submit_change_perm(GtkWidget* widget, gpointer data){
    change_perm();
}

int g_callback_dir_info_open(GtkWidget* widget, gpointer data){
    gtk_widget_show_all(md_mkdir.window);
}

int g_callback_open_mkfile_modal(GtkWidget* widget, gpointer data){
    open_text_editor();
}

int g_callback_submit_mkfile(GtkWidget* widget, gpointer data){
    write_file();
}


int g_callback_mkdir_popup_submit(GtkWidget* widget, gpointer data){
    submit_file_info();
    return TRUE;
}

int g_callback_mvdir(GtkWidget* widget, gpointer data){
    display_mode = FILE_DISPLAY_MODE;
    char* str = (char*)data;
    printf("mv:%s\n", str);
    move_full_directory(str);
}

char* gettok(int index){
    int i = path_tok_map[index];
    return path_tok[i];
}

int g_callback_mvdir_tok(GtkWidget* widget, gpointer data){
    display_mode = FILE_DISPLAY_MODE;
    int index = get_clicked_index(widget);
    int i=0;
    char mv_path[MAX_PATH_LEN];
    char* p = mv_path;
    char* tok;
    while(strcmp((tok = gettok(i)), "") != 0 && i <= index){
        strcpy(p, "/");
        p += 1;
        strcpy(p, tok);
        printf("p:%s\n", p);
        p += strlen(tok);
        // printf(" pp:%s\n", p);
        ++i;
    }
    move_full_directory(mv_path);
}

gboolean on_realize(gpointer data){
    // printf("realize\n");
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



void on_popup_close(GtkWidget *widget, gpointer data) {
    GtkWidget *popup = GTK_WIDGET(data);
    gtk_widget_hide(popup);  // gtk_widget_destroy 시 위젯이 완전히 삭제되기 때문에 hide 사용 
}

// menu callback: file option
int g_callback_file_rename(GtkWidget* widget, gpointer data){
    edit_mode = EDIT_MODE_RENAME;
    gtk_entry_set_text(GTK_ENTRY(md_mkdir.inp_filename), file_list[selected_index].name);
    gtk_window_set_title(GTK_WINDOW(md_mkdir.window), "RENAME FILE");
    gtk_widget_show_all(md_mkdir.window);
}

int g_callback_file_modify(GtkWidget* widget, gpointer data){
    
}

int g_callback_file_move(GtkWidget* widget, gpointer data){
    move_file();
}

int g_callback_file_copy(GtkWidget* widget, gpointer data){
    copy_file();
}

int g_callback_file_paste(GtkWidget* widget, gpointer data){
    paste_file();
}

int g_callback_file_execute(GtkWidget* widget, gpointer data){
    execute_file(file_list[selected_index].name);
}
int g_callback_file_remove(GtkWidget* widget, gpointer data){
    remove_file(file_list[selected_index].name);
}

int g_callback_file_mklink(GtkWidget* widget, gpointer data){
    edit_mode = EDIT_MODE_LINK;
    gtk_entry_set_text(GTK_ENTRY(md_mkdir.inp_filename), file_list[selected_index].name);
    gtk_window_set_title(GTK_WINDOW(md_mkdir.window), "MAKE LINK FILE");
    gtk_widget_show_all(md_mkdir.window);
}

// menu callback: dir
int g_callback_dir_remove(GtkWidget* widget, gpointer data){
    remove_dir(file_list[selected_index].name);
}

int g_callback_search_file(GtkWidget* widget, gpointer data){
    search_file();
}

int g_callback_search_inp_changed(GtkWidget* widget, gpointer data){
    strcpy(found_filename, "");
}

int g_callback_clear_clipboard(GtkWidget* widget, gpointer data){
    clear_clipboard();
}