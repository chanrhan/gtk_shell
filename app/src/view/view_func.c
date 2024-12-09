#include "view_func.h"

int move_directory(char* append){
    // printf("cwd: %s\n", cwd);

    req_msg_t req;
    res_msg_t res;
    req.cmd = CMD_LS;
    if(append != NULL){
        strncpy(req.args[0], append, 16);
    }else{
        req.args[0][0] = '\0';
    }
    if(send_wait_rcv(&req, &res) != 0){
        return 1;
    }
    printf("move: %s [%d] success!\n", cwd, res.data.file_len);

    update_directory(res);

    set_text_current_dir(res.cwd);

    return 0;
}

int g_callback_quit(GtkWidget* widget, gpointer data){
    req_msg_t req;
    req.cmd = CMD_QUIT;
    // printf("send..\n");
    send_to_server(&req);
    gtk_exit(widget, data);
    return 0;
}

int g_callback_mvdir(GtkWidget* widget, gpointer data){
    int *index = (int *)g_object_get_data(G_OBJECT(widget), "index");
    // printf("click index:%d\n", *index);
    char* append = file_list[*index].name;
    // printf("%d-> append:%s,\n", *index, append);
    move_directory(append);

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
    int perm = 0;
    int file_type = 0;
    char* filename = gtk_entry_get_text(GTK_ENTRY(mkdir_modal.inp_filename));
    if(filename[0] == '\0'){
        gtk_label_set_text(mkdir_modal.inp_error_label, "invalid filename");
        return TRUE;
    }

    int bit = 0;
    for(int i=0;i<9;++i){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(mkdir_modal.perm_check_box[i]))){
            bit |= (1 << (i+1)%3);
        }
        if((i+1)%3 == 0){
            perm += bit * pow(10, i/3);
        }
    }

    char* text = (char*)gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(GTK_COMBO_BOX(mkdir_modal.select_type)));
    if(strcmp(text, "directory") == 0){
        file_type = 0;
    }else if(strcmp(text, "file") == 0){
        file_type = 1;
    }else if(strcmp(text, "link") == 0){
        file_type = 2;
    }else{
        perror("WRONG VALUE");
        exit(1);
    }

    printf("type:%d,filename:%s,perm:%d,\n", file_type, filename, perm);

    req_msg_t req;
    res_msg_t res;

    switch (file_type)
    {
        case 0:
            req.cmd = CMD_MK_DIR;
            break;
        case 1:
            req.cmd = CMD_MK_FILE;
            break;
        case 2:
            req.cmd = CMD_MK_LN;
            break;
        default:
            break;
    }
    strncpy(req.args[0], filename, ARG_SIZE);
    sprintf(req.args[1], "%d",perm);

    send_wait_rcv(&req, &res);
    if(res.status != 0){
        // gtk_label_set_text(mkdir_modal.inp_error_label, res.ERROR);
        return TRUE;
    }
    printf("mk: %s [%d] success!\n", cwd, res.data.file_len);

        update_directory(res);
        gtk_widget_hide(mkdir_modal.window);

    return TRUE;
}

gboolean on_realize(gpointer data){
    move_directory(NULL);
    return FALSE;
}