#include "cmd.h"

int cmd_kill(req_msg_t req, res_msg_t* res){
    int pid, sig;
    char* _pid = &req.args[0];
    char* _sig = &req.args[0];
    if((pid = atoi(_pid)) != 0){
        if((sig = atoi(_sig)) == 0){
            sig = 15;
        }
        return kill(pid, sig);
    }
    return 1;
}