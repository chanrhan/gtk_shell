#include "view_func.h"
#include "gtk_draw.h"
#include "ipc_view.h"

void get_time_format(char* time, char* out){
    // oo/oo oo:oo = 11
    char* buf[4];
    int i=0;
    char* p;
    p = time + strlen(time);
    while(p > time){
        p -= 2;
        buf[i] = (char*)malloc(3);
        strncpy(buf[i], p, 3);
        // printf("%s: %s\n",p, buf[i]);
        ++i;
        *p = '\0';
    }
    snprintf(out, 12, "%s/%s %s:%s", buf[3], buf[2], buf[1], buf[0]);
}

int move_full_directory(char* path){
    gtk_style_class_toggle(btn_ps, "process_mode", FALSE);

    printf("move cwd: %s\n", path);

    req_msg_t req;
    res_msg_t res;
    req.cmd = CMD_LS_FULL;
    strncpy(req.args[0], path, MAX_PATH_LEN);
    send_wait_rcv(&req, &res);
    if(res.status != STATUS_OK){
        return 1;
    }
    update_current_working_directory(res.cwd);
    update_file_list(res);

    return 0;
}

int move_directory(char* path){
    printf("move cwd: %s\n", cwd);

    req_msg_t req;
    res_msg_t res;
    req.cmd = CMD_LS;
    if(path != NULL){
        strncpy(req.args[0], path, 16);
    }else{
        req.args[0][0] = '\0';
    }   
    send_wait_rcv(&req, &res);
    if(res.status != STATUS_OK){
        return 1;
    }
    // printf("move: %s [%d] success!\n", cwd, res.data.file_len);


    update_current_working_directory(res.cwd);
    update_file_list(res);

    return 0;
}

int show_process(){
    req_msg_t req;
    res_msg_t res;

    req.cmd = CMD_PS;

    int len = send_wait_rcv(&req, &res);
    if(len >= 0){
        update_process_list(res);
    }
}

void toggle_display_mode(){
    if(display_mode == FILE_DISPLAY_MODE){
        display_mode = PROCESS_DISPLAY_MODE;
        gtk_style_class_toggle(btn_ps, "process_mode", TRUE);
        show_process();
    }else{
        display_mode = FILE_DISPLAY_MODE;
        gtk_style_class_toggle(btn_ps, "process_mode", FALSE);
        move_full_directory(cwd);
    }
}