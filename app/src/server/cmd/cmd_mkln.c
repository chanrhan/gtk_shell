#include "cmd.h"

int cmd_mkln(req_msg_t req, res_msg_t *res){
    char source[CWD_LEN];
    if(validate_path(req.cwd, req.args[0], &source, 1) != 0){
        perror("validat_path");
        return 1;
    }

    char dest[CWD_LEN];
    if(validate_path(req.cwd, req.args[1], &dest, 1) != 0){
        perror("validat_path");
        return 1;
    }

    int option_s = 0;
    int ret = 0;
    for(int i=2;i<4;++i){
        if(req.args[i][0] == 's'){
            option_s =1;
        }
    }
    if(option_s == 1){
        ret = symlink(source, dest); 
    }else{
        ret = link(source, dest);
    }

    get_ls(req.cwd, &res->data);
    return ret;
}