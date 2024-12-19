#include "gtk_utils.h"
#include "gtk_draw.h"
#include "gtk_modal.h"
#include "ipc_view.h"
#include "view_func.h"

void set_perm(GtkWidget* widget[9], int mode){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[0]), (mode & 0001) ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[1]), (mode & 0002) ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[2]), (mode & 0004) ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[3]), (mode & 0010) ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[4]), (mode & 0020) ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[5]), (mode & 0040) ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[6]), (mode & 0100) ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[7]), (mode & 0200) ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget[8]), (mode & 0400) ? TRUE : FALSE);
}

int get_perm(GtkWidget* widget[9]){
    int perm = 0;
    for(int i=0;i<9;++i){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget[i]))){
            perm |= (1 << i);
        }
    }
    return perm;
}

int change_perm(){
    char* filename = gtk_label_get_text(GTK_LABEL(md_file_detail.filename_label));
    int perm = get_perm(md_file_detail.perm_check_box);
    

    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_CH_PERM;
    strncpy(req.args[0], filename, MAX_ARGV_SIZE);
    sprintf(req.args[1], "%o", perm);
    printf("change_perm:%s\n", req.args[1]);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        inform_dialog("Permission Update Success");
    }else{
        error_dialog("Permission Update Failed");
    }
    gtk_widget_hide(md_file_detail.window);
}

int open_file_detail(){
    printf("open file\n");
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_OPEN_FILE;
    strncpy(req.args[0], file_list[selected_index].name, MAX_ARGV_SIZE);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        gtk_widget_show_all(md_file_detail.window);
        file_info_t* file = &res.data.files[0];
        char birthtime[12];
        char mtime[12];
        get_time_format(file->birthtime, birthtime);
        get_time_format(file->mtime, mtime);
        
        gtk_label_set_text(GTK_LABEL(md_file_detail.filename_label), file_list[selected_index].name);
        gtk_label_set_text(GTK_LABEL(md_file_detail.birthtimetime_label), birthtime);
        gtk_label_set_text(GTK_LABEL(md_file_detail.mtime_label), mtime);
        char text[8];
        sprintf(text, "%d", file->size);
        gtk_label_set_text(GTK_LABEL(md_file_detail.size_label), text);

        int mode = file->perm;
        set_perm(md_file_detail.perm_check_box, mode);
    }
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
        case EDIT_MODE_FILE:
            open_text_editor();
            break;
        default:
            break;
    }
    
}

int open_text_editor(){
    char* filename = gtk_entry_get_text(GTK_ENTRY(md_mkdir.inp_filename));
    strncpy(md_text_editor.filename, filename, 64);
    gtk_widget_hide(md_mkdir.window);
    gtk_widget_show_all(md_text_editor.window);
}

int read_file(char* filename){
    strcpy(md_text_editor.filename, filename);
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_CAT;
    strncpy(req.args[0], filename, MAX_PATH_LEN);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        gtk_widget_show_all(md_text_editor.window);
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(md_text_editor.textview));
        gtk_text_buffer_set_text(buffer, res.text_buf, -1);
    }
}

int write_file(){
    char* filename = &md_text_editor.filename;
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *text;

    // GTK에서는 textview에서 buffer를 통해 전체 텍스트를 가져올 수 있다. 
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(md_text_editor.textview));
    gtk_text_buffer_get_bounds(buffer, &start, &end); 
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_MK_FILE;
    strncpy(req.args[0], filename, MAX_PATH_LEN);
    strncpy(req.text_buf, text, MAX_TXB_SIZE);

    printf("(%d)text:%s\n", strlen(req.text_buf), req.text_buf);


    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        inform_dialog("File Create Success");
        update_file_list(res);
    }else{
        error_dialog("File Create Failed");
    }
    strcpy(md_text_editor.filename, "");
    strcpy(md_mkdir.inp_filename, "");
    g_free(text); // gtk_text_view_get_buffer로 가져온 gchar 텍스트는 반드시! g_free로 해제시켜줘야함
    gtk_widget_hide(md_text_editor.window);
}


int make_dir(){
    int file_type = 0;
    char* filename = gtk_entry_get_text(GTK_ENTRY(md_mkdir.inp_filename));
    if(filename[0] == '\0'){
        gtk_label_set_text(md_mkdir.inp_error_label, "invalid filename");
        return TRUE;
    }
    // for(int i=0;i<9;++i){
    //     if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(md_mkdir.perm_check_box[i]))){
    //         perm |= (1 << i);
    //     }
    // }

    printf("type:%d,filename:%s\n", file_type, filename);

    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_MK_DIR;
    strncpy(req.args[0], filename, MAX_ARGV_SIZE);
    // sprintf(req.args[1], "%d",perm);

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        printf("mk: %s [%d] success!\n", cwd, res.data.file_len);
        gtk_entry_set_text(GTK_ENTRY(md_mkdir.inp_filename), "");
        inform_dialog("Directory Create Success");
        update_file_list(res);
    }else{
        error_dialog("Directory Create Failed");
    }
    gtk_widget_hide(md_mkdir.window);
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
        inform_dialog("Rename Success");
        gtk_entry_set_text(GTK_ENTRY(md_mkdir.inp_filename), "");
        update_file_list(res);
    }else{
        error_dialog("Rename Failed");
    }
    gtk_widget_hide(md_mkdir.window);
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
        inform_dialog("Remove File Success");
        update_file_list(res);
    }else{
        error_dialog("Remove File Failed");
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
        inform_dialog("Remove Directory Success");
        update_file_list(res);
    }else{
        error_dialog("Remove Directory Failed");
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

    set_clipboard(text);
    inform_dialog("File Selected");
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
    set_clipboard(text);
    inform_dialog("File Copied");
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

    // 파일명만 따로 분리
    char* filename = strrchr(source, '/');
    if(filename == NULL){
        filename = source;
    }else{
        filename += 1;
    }
    char dest[MAX_PATH_LEN];
    snprintf(dest, MAX_PATH_LEN, "%s/%s", cwd, filename);
    printf("paste:%s->%s\n", source, dest);
    
    // 폴더 복사할 경우, 해당 폴더를 폴더 내에 복사하는 상황 방지 
    if(copied_file.type ==4 && strncmp(source, dest, strlen(source)) == 0){
        error_dialog("Invalid Paste");
        return 1;
    }
    
    if(copied_file.type == 8 && copy_mode == 0){
        char* p = strrchr(dest, '.');
        if(p == NULL){
            p = dest + strlen(dest);    
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
        clear_clipboard();
        inform_dialog("Paste Success Success");
        update_file_list(res);
    }else{
        error_dialog("Paste Failed");
    }
    copy_mode = -1;
}



int execute_file(char* filename){
    printf("execute:%s\n", filename);
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_EXEC;

    strncpy(req.args[0], filename, MAX_ARGV_SIZE);


    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
         inform_dialog("File Execute Success");
        // update_file_list(res);
    }else{
         error_dialog("File Execute Failed");
    }
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
    if(len >= 0){
        update_current_working_directory(res.cwd);
        strncpy(found_filename, res.find, MAX_PATH_LEN);
        printf("found:%s\n", found_filename);
        update_file_list(res);
    }else{
        strcpy(found_filename, "");
    }
    
}
