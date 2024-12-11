#include "cmd.h"

int cmd_mkdir(req_msg_t req, res_msg_t *res)
{
    char cwd[CWD_LEN];
    // printf("cwd:%s,mk:%s,mode:%d\n", req.cwd, req.args[0], (mode_t)atoi(req.args[1]));
    if(validate_path(req.cwd, req.args[0], &cwd, 1) != 0){
        perror("validat_path");
        return 1;
    }

    printf("validate cwd:%s\n", cwd);
    int ret = mkdir(cwd, atoi(req.args[1]));
    if(ret != 0){
        perror("mkdir: ");
        // char* err = strerror(errno);
        // printf("err:%s|\n",err);
    }
    get_ls(req.cwd, &res->data);
    return ret;
}