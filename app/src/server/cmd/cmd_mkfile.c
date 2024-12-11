#include "cmd.h"

int cmd_mkfile(req_msg_t req, res_msg_t *res){
    char cwd[CWD_LEN];
    if(append_path(req.cwd, req.args[0], &cwd, 1) != 0){
        perror("validat_path");
        return 1;
    }

    int ret = mkdir(cwd, atoi(req.args[1]));
    get_ls(req.cwd, &res->data);
    return ret;

}