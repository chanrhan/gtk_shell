#include "cmd.h"

int cmd_ls(req_msg_t req, res_msg_t* res){
    if(append_path(req.cwd, req.args[0], &res->cwd, 0) != 0){
        printf("access failed!");
        return 1;
    }
    get_file_list(res->cwd, &res->data);

    return 0;
}