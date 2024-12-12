#include "cmd.h"

int cmd_rmfile(req_msg_t req, res_msg_t* res){
    char* target;
    if(append_path(req.cwd, req.args[0], target, 0) != 0){
        perror("append path");
    }

    int ret = remove(target);
    get_file_list(req.cwd, &res->data);
    return ret;
}