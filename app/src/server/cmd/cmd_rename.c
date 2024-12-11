#include "cmd.h"

int cmd_rename(req_msg_t req, res_msg_t *res)
{
    char path[CWD_LEN];
    if(validate_path(req.cwd, req.args[0], path, 0) != 0){
        perror("validat_path");
        return 1;
    }
    int ret = rename(path, req.args[1]);
    get_ls(req.cwd, &res->data);
    return ret;
}