#include "cmd.h"

int cmd_chmod(req_msg_t req, res_msg_t* res){
    printf("change_perm\n");
    char path[MAX_PATH_LEN];
    if(append_path(req.cwd, req.args[0], path, 0) != 0){
        perror("append path: target");
        return 1;
    }
    const char* perm = req.args[1];
    printf("perm:%s\n", req.args[1]);

    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        perror("undefined file name");
        return -1;
    }
    mode_t mode = statbuf.st_mode;

    if (atoi(perm) != 0) {
        mode = strtol(perm, NULL, 8);  // 8진수 권한으로 변경
    } else {
        return 1;
    }

    return chmod(path, mode);
}