#include "ipc_view.h"

int req_msg_q_id = 0;
int res_msg_q_id = 0;

int send_to_server(req_msg_t* req)
{
    strncpy(req->cwd, cwd, MAX_PATH_LEN);
    req->mtype=1;
    printf("msg_q_id:%d\n", req_msg_q_id);
    printf("Send cmd: %d\n", req->cmd);
    printf("Send byte: %d\n", sizeof(req_msg_t));
    // char* p = req;
    // printf("---START---\n");
    // for(int i=0;p[i]='\0';++i){
    //     printf("%c", p[i]);
    // }
    // printf("----END----\n");
    printf("file limit:%d\n", MAX_FILE_LIST_SIZE);
    if ((msgsnd(req_msg_q_id, req, sizeof(req_msg_t), IPC_NOWAIT)) == -1)
    {
        perror("msgsnd");
        exit(1);
    }
    printf("sended");

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
