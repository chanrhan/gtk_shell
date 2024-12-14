#include "cmd.h"

int cmd_rmfile(req_msg_t req, res_msg_t* res){
    char path[MAX_PATH_LEN];
    if(append_path(req.cwd, req.args[0], path, 0) != 0){
        perror("append path");
    }

    int ret = remove(path);
    if(ret != 0){
        return 1;
    }
    get_file_list(req.cwd, &res->data);
    return ret;
}