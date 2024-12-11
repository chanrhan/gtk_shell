#include "view_func.h"
#include "gtk_draw.h"
#include "ipc_view.h"

int move_directory(char* append){
    printf("move cwd: %s\n", cwd);

    req_msg_t req;
    res_msg_t res;
    req.cmd = CMD_LS;
    if(append != NULL){
        strncpy(req.args[0], append, 16);
    }else{
        req.args[0][0] = '\0';
    }
    send_wait_rcv(&req, &res);
    if(res.status != 0){
        return 1;
    }
    printf("move: %s [%d] success!\n", cwd, res.data.file_len);

    update_directory(res);

    set_text_current_dir(res.cwd);

    return 0;
}
