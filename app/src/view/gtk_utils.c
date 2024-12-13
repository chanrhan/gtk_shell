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
            link_file();
            break;
        default:
            break;
    }
    gtk_widget_hide(md_mkdir.window);
}

int make_dir(){
    int perm = 0;
    int file_type = 0;
    char* filename = gtk_entry_get_text(GTK_ENTRY(md_mkdir.inp_filename));
    if(filename[0] == '\0'){
        gtk_label_set_text(md_mkdir.inp_error_label, "invalid filename");
        return TRUE;
    }
    for(int i=0;i<9;++i){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(md_mkdir.perm_check_box[i]))){
            perm |= (1 << i);
        }
    }

    printf("type:%d,filename:%s,perm:%d,\n", file_type, filename, perm);

    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_MK_DIR;
    strncpy(req.args[0], filename, MAX_ARGV_SIZE);
    sprintf(req.args[1], "%d",perm);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        printf("mk: %s [%d] success!\n", cwd, res.data.file_len);
        show_dialog_text("Directory Create Success");
        update_file_list(res);
    }else{
        show_dialog_text("Directory Create Failed");
    }
}

int rename_file(){
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_RENAME;

    strncpy(req.args[0], file_list[selected_index].name, MAX_ARGV_SIZE);

    char* filename = gtk_entry_get_text(GTK_ENTRY(md_mkdir.inp_filename));
    strncpy(req.args[1], filename, MAX_ARGV_SIZE);
    printf("rename: %s->%s\n", req.args[0], req.args[1]);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        show_dialog_text("Rename Success");
        update_file_list(res);
    }else{
        show_dialog_text("Rename Failed");
    }
}

int remove_file(char* filename){
    // printf("rmdir: %s\n", filename);
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_RM_FILE;
    // strncpy(req.cwd, cwd, CWD_LEN);
    strncpy(req.args[0], filename, MAX_ARGV_SIZE);
    
    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        show_dialog_text("Remove File Success");
        update_file_list(res);
    }else{
        show_dialog_text("Remove File Failed");
    }
    gtk_widget_hide(ctxm_dir_option.menu);
}

int remove_dir(char* filename){
    // printf("rmdir: %s\n", filename);
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_RM_DIR;
    strncpy(req.args[0], filename, MAX_ARGV_SIZE);
    
    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        show_dialog_text("Remove Directory Success");
        update_file_list(res);
    }else{
        show_dialog_text("Remove Directory Failed");
    }
    gtk_widget_hide(ctxm_dir_option.menu);
}

int move_file(){
    printf("move\n");
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

    // char path[MAX_PATH_LEN];
    snprintf(copied_file.name, MAX_PATH_LEN, "%s/%s", cwd, org->name);
    // strncpy(copied_file.name, path, MAX_PATH_LEN);
    printf("copied name:%s, %s\n", copied_file.name, org->name);

    char text[64];
    snprintf(text, 64, "%4s%4d%4d%4s", copied_file.name, copied_file.type, copied_file.size, copied_file.mtime);

    copy_mode = 1;
    gtk_style_class_toggle(label_data[selected_index], "cut", TRUE);

    gtk_label_set_text(GTK_LABEL(copied_file_label), text);
    show_dialog_text("File Selected");
}

int copy_file(){
    printf("copy\n");
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

    // char path[MAX_PATH_LEN];
    snprintf(copied_file.name, MAX_PATH_LEN, "%s/%s", cwd, org->name);
    // strncpy(copied_file.name, path, MAX_PATH_LEN);
    printf("copied name:%s, %s\n", copied_file.name, org->name);

    char text[64];
    snprintf(text, 64, "%4s%4d%4d%4s", copied_file.name, copied_file.type, copied_file.size, copied_file.mtime);

    copy_mode = 0;
    gtk_label_set_text(GTK_LABEL(copied_file_label), text);
    show_dialog_text("File Copied");
}

int paste_file(){
    if(copy_mode == -1){
        return 1;
    }
    req_msg_t req;
    res_msg_t res;

    req.cmd = (copy_mode == 0) ? CMD_CP : CMD_MV;

    char* source = &copied_file.name;
    
    strncpy(req.args[0], source, MAX_ARGV_SIZE);
    // printf("args0:%s, source:%s, org:%s\n", req.args[0], source, copied_file.name);

    char* filename = strrchr(source, '/');
    if(filename == NULL){
        filename = source;
    }else{
        filename += 1;
    }
    char dest[MAX_PATH_LEN];
    snprintf(dest, MAX_PATH_LEN, "%s/%s", cwd, filename);
    
    // 폴더 복사할 경우, 해당 폴더를 폴더 내에 복사하는 상황 방지 
    if(copied_file.type ==4 && strncmp(source, dest, strlen(source)) == 0){
        printf("same directory");
        return 1;
    }
    
    if(copy_mode == 0){
        char* p = strrchr(dest, '.');
        if(p == NULL){
            p += strlen(dest);    
        }
        // printf("p:%s, len:%d\n", p, strlen(p));
        // snprintf(dest, strlen(dest)+strlen(p)+5, "");
        char* tmp = (char*)malloc(strlen(p)+6);
        snprintf(tmp, strlen(p)+6, "_copy%s", p);
        strncpy(p, tmp, strlen(p)+6);
        // printf("paste:%s | %s\n", p, source);
    }
    // printf("dest:%s,\n", dest);
    
    strncpy(req.args[1], dest, MAX_ARGV_SIZE);
    // printf("paste[%d]: %s->%s\n", copy_mode, req.args[0], req.args[1]);
    

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        if(copy_mode == 1){
            strcpy(copied_file.mtime, "");
            strcpy(copied_file.name, "");
            copied_file.size = -1;
            copied_file.type = -1;
        }
        show_dialog_text("Paste Success Success");
        update_file_list(res);
    }else{
        show_dialog_text("Paste Failed");
    }
    copy_mode = -1;
}



int execute_file(char* filename){

}

int link_file(){
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_MK_LN;

    strncpy(req.args[0], file_list[selected_index].name, MAX_ARGV_SIZE);

    char* filename = (char*)malloc(64);
    strcpy(filename, gtk_entry_get_text(GTK_ENTRY(md_mkdir.inp_filename)));

    char* p = strrchr(filename, '.');
    if(p == NULL){
        p = filename + strlen(filename);    
    }
    strcpy(p, ".ln");
    printf("link:%s\n", filename);
    
    strncpy(req.args[1], filename, MAX_ARGV_SIZE);
    printf("rename: %s->%s\n", req.args[0], req.args[1]);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        update_file_list(res);
    }
}

int search_file(){
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_SEARCH;

    char* text = gtk_entry_get_text(GTK_ENTRY(search_inp));
    strncpy(req.args[0], text, MAX_ARGV_SIZE);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0)
        update_current_working_directory(res.cwd);
        strncpy(found_filename, res.find, MAX_PATH_LEN);
        update_file_list(res);
    
}
