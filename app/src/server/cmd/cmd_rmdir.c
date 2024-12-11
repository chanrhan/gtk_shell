#include "cmd.h"

int cmd_rmdir(req_msg_t req, res_msg_t *res)
{
    char source[CWD_LEN];
    if(validate_path(req.cwd, req.args[0], &source, 1) != 0){
        perror("validat_path");
        return 1;
    }

    int ret = rmdir(source);
    get_ls(req.cwd, &res->data);
    return ret;
}