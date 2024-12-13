#include "view_func.h"
#include "gtk_draw.h"
#include "ipc_view.h"

int move_full_directory(char* path){
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
    send_wait_rcv(&req, &res);
    if(res.status != STATUS_OK){
        return 1;
    }
    
}
