#include "ipc_server.h"


int send_to_view(res_msg_t* res){
    res->mtype=1;
    printf("Send status: %d\n", res->status);
    if((msgsnd(res_msg_q_id, res, sizeof(res_msg_t), IPC_NOWAIT)) == -1){
        perror("msgsnd");
        return 1;
    }
    return 0;
}

// 클라이언트로부터 요청 기다리는 함수
// 읽어온 데이터의 크기를 반환 
int wait_rcv(req_msg_t* req){
    int len = msgrcv(req_msg_q_id, req, sizeof(req_msg_t), 0,0);
    printf("Received cmd: %d, pwd: %s\n", req->cmd, req->cwd);
    return len;
}