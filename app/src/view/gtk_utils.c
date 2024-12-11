#include "gtk_utils.h"
#include "gtk_draw.h"
#include "gtk_modals.h"
#include "ipc_view.h"
#include "view_func.h"

int make_dir(){
    int perm = 0;
    int file_type = 0;
    char* filename = gtk_entry_get_text(GTK_ENTRY(mkdir_modal.inp_filename));
    if(filename[0] == '\0'){
        gtk_label_set_text(mkdir_modal.inp_error_label, "invalid filename");
        return TRUE;
    }
    for(int i=0;i<9;++i){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(mkdir_modal.perm_check_box[i]))){
            perm |= (1 << i);
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
    printf("mk: %s [%d] success!\n", cwd, res.data.file_len);

    update_directory(res);
    gtk_widget_hide(mkdir_modal.window);
}

int rename_dir(){

}

int remove_dir(char* filename){
    printf("rmdir: %s\n", filename);
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_RM_DIR;
    strncpy(req.cwd, cwd, CWD_LEN);
    strncpy(req.args[0], filename, ARG_SIZE);
    
    printf("111\n");
    send_wait_rcv(&req, &res);
    printf("222\n");
    update_directory(res);
    printf("333\n");
    gtk_widget_hide(dir_menu_context.menu);
}

int move_file(char* source, char* dest){

}

int copy_file(char* source, char* dest){

}

int execute_file(char* filename){

}

int link_file(char* source, char* dest){

}


