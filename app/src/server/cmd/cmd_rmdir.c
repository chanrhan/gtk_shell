#include "cmd.h"

int cmd_rmdir(req_msg_t req, res_msg_t *res)
{
    char source[MAX_PATH_LEN];
    if(append_path(req.cwd, req.args[0], &source, 1) != 0){
        perror("validat_path");
        return 1;
    }

    int ret = rmdir(source);
    if(ret != 0){
        return 1;
    }
    get_file_list(req.cwd, &res->data);
    return ret;
}