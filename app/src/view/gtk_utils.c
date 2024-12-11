#include "gtk_utils.h"
#include "gtk_draw.h"
#include "gtk_modal.h"
#include "ipc_view.h"
#include "view_func.h"

int make_file(){

}

int submit_file_info(){
    switch (edit_mode)
    {
        case EDIT_MODE_MAKE:
            make_dir();
            break;
        case EDIT_MODE_RENAME:
            rename_file();
            break;
        case EDIT_MODE_LINK:
            // link_file();
            break;
        default:
            break;
    }
    gtk_widget_hide(mkdir_modal.window);
}

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

    // char* text = (char*)gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(GTK_COMBO_BOX(mkdir_modal.select_type)));
    // if(strcmp(text, "directory") == 0){
    //     file_type = 0;
    // }else if(strcmp(text, "file") == 0){
    //     file_type = 1;
    // }else if(strcmp(text, "link") == 0){
    //     file_type = 2;
    // }else{
    //     perror("WRONG VALUE");
    //     exit(1);
    // }

    printf("type:%d,filename:%s,perm:%d,\n", file_type, filename, perm);

    req_msg_t req;
    res_msg_t res;

    // switch (file_type)
    // {
    //     case 0:
    //         req.cmd = CMD_MK_DIR;
    //         break;
    //     case 1:
    //         req.cmd = CMD_MK_FILE;
    //         break;
    //     case 2:
    //         req.cmd = CMD_MK_LN;
    //         break;
    //     default:
    //         break;
    // }
    req.cmd = CMD_MK_DIR;
    strncpy(req.args[0], filename, ARG_SIZE);
    sprintf(req.args[1], "%d",perm);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        printf("mk: %s [%d] success!\n", cwd, res.data.file_len);
        update_directory(res);
    }
    
}

int rename_file(){
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_RENAME;

    strncpy(req.args[0], file_list[selected_index].name, ARG_SIZE);

    char* filename = gtk_entry_get_text(GTK_ENTRY(mkdir_modal.inp_filename));
    strncpy(req.args[1], filename, ARG_SIZE);
    printf("rename: %s->%s\n", req.args[0], req.args[1]);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        update_directory(res);
    }
}

int remove_file(char* filename){
    // printf("rmdir: %s\n", filename);
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_RM_FILE;
    // strncpy(req.cwd, cwd, CWD_LEN);
    strncpy(req.args[0], filename, ARG_SIZE);
    
    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        update_directory(res);
    }
    gtk_widget_hide(dir_menu_context.menu);
}

int remove_dir(char* filename){
    // printf("rmdir: %s\n", filename);
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_RM_DIR;
    strncpy(req.args[0], filename, ARG_SIZE);
    
    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        update_directory(res);
    }
    gtk_widget_hide(dir_menu_context.menu);
}

int move_file(char* source, char* dest){

}

int copy_file(){
    if(selected_index < 0){
        return 1;
    }
    file_info_t* org = &file_list[selected_index];
    if(strcmp(org->name, "..") == 0){
        return 0;
    }

    strncpy(copied_file.mtime, org->mtime, 9);
    
    copied_file.size = org->size;
    copied_file.type = org->type;

    char path[CWD_LEN];
    snprintf(path, CWD_LEN, "%s/%s", cwd, org->name);
    strncpy(copied_file.name, path, 16);

    char text[64];
    snprintf(text, 64, "%4s%4d%4d%4s", path, copied_file.type, copied_file.size, copied_file.mtime);

    gtk_label_set_text(GTK_LABEL(copied_file_label), text);
}

int paste_file(char* path){

}



int execute_file(char* filename){

}

int link_file(char* path){
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_RM_DIR;
    strncpy(req.args[0], path, ARG_SIZE);
    
    send_wait_rcv(&req, &res);
    update_directory(res);
    gtk_widget_hide(dir_menu_context.menu);
}


