#include "ipc_view.h"

int send_to_server(req_msg_t* req)
{
    strncpy(req->cwd, cwd, CWD_LEN);
    req->mtype=1;
    printf("Send cmd: %d\n", req->cmd);
    // char* p = req;
    // printf("---START---\n");
    // for(int i=0;p[i]='\0';++i){
    //     printf("%c", p[i]);
    // }
    // printf("----END----\n");

    if ((msgsnd(req_msg_q_id, req, sizeof(req_msg_t), IPC_NOWAIT)) == -1)
    {
        perror("msgsnd");
        exit(1);
    }

    return 0;
}

int wait_rcv(res_msg_t* res)
{
    int len = msgrcv(res_msg_q_id, res, sizeof(res_msg_t), 0, 0);
    printf("Received status: %d\n", res->status);
    if(res->status != STATUS_OK){
        perror("status error");
        return -1;
    }
    return len;
}

int send_wait_rcv(req_msg_t* req, res_msg_t* res){
    send_to_server(req);
    return wait_rcv(res);
}
