#include "cmd.h"

int cmd_cat(req_msg_t req, res_msg_t* res){
    char dir[MAX_PATH_LEN];
    if(append_path(req.cwd, req.args[0], dir, 0) != 0){
        perror("append path: mkfile");
        return 1;
    }
    
    FILE *fp; 
    if((fp = fopen(dir, "r")) == NULL){
        perror("fopen");
        return 1;
    }
    snprintf(res->text_buf, MAX_TXB_SIZE, "%s", fp);

    return 0;

}