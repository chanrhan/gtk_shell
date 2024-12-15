#include "cmd.h"

int cmd_ls(req_msg_t req, res_msg_t* res){
    if(append_path(req.cwd, req.args[0], &res->cwd, 0) != 0){
        printf("access failed!");
        return 1;
    }
    get_file_list(res->cwd, &res->data);

    return 0;
}

int cmd_ls_full(req_msg_t req, res_msg_t* res){
    // if(append_path(req.cwd, req.args[0], &res->cwd, 0) != 0){
    //     printf("access failed!");
    //     return 1;
    // }
    if(strncmp(req.args[0], ROOT_DIR, strlen(ROOT_DIR)) != 0){
        perror("Can't access");
        return 1;
    }
    if(access(req.args[0], 0) == -1){
        perror("access: ls_full");
        return 1;
    }
    strncpy(res->cwd, req.args[0], MAX_PATH_LEN);
    get_file_list(res->cwd, &res->data);

    return 0;
}