#include "cmd.h"

int cmd_move(req_msg_t req, res_msg_t* res){
    char* source = req.args[0];
    char* dest = req.args[1];
    if(access(source, 0) == -1){
        perror("access: copy file");
        return 1;
    }

    // char path[MAX_PATH_LEN];
    // if(append_path(req.cwd, req.args[0], path, 0) != 0){
    //     perror("append_path (1)");
    //     return 1;
    // }
    // char new_filename[MAX_PATH_LEN];
    // if(append_path(req.cwd, req.args[1], new_filename, 1) != 0){
    //     perror("append_path (2)");
    //     return 1;
    // }
    printf("move: %s->%s\n", source, dest);
    int ret = rename(source, dest);
    get_file_list(req.cwd, &res->data);
    return ret;
}