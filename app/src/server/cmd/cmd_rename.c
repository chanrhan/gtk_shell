#include "cmd.h"

int cmd_rename(req_msg_t req, res_msg_t *res)
{
    char path[CWD_LEN];
    if(append_path(req.cwd, req.args[0], path, 0) != 0){
        perror("append_path (1)");
        return 1;
    }
    char new_filename[CWD_LEN];
    if(append_path(req.cwd, req.args[1], new_filename, 1) != 0){
        perror("append_path (2)");
        return 1;
    }
    printf("rename: %s->%s\n", path, new_filename);
    int ret = rename(path, new_filename);
    get_ls(req.cwd, &res->data);
    return ret;
}