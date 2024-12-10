#include "ipc_server.h"
#include <sys/msg.h>

int send_to_view(res_msg_t* res){
    res->mtype=1;
    printf("Send status: %d\n", res->status);
    printf("Send byte: %d\n", sizeof(res_msg_t));

    // int msqid = 3407872; // 메시지 큐 ID를 여기에 설정
    // struct msqid_ds buf;
    // // struct msginfo* minfo;
    // // printf("msgmax:%d\n",minfo->msgmax);

    // // msgctl로 메시지 큐 상태 가져오기
    // if (msgctl(msqid, IPC_STAT, &buf) == -1) {
    //     perror("msgctl");
    //     return 1;
    // }

    // // 메시지 큐 상태 출력
    // printf("Current bytes in queue: %lu\n", buf.msg_cbytes); // 현재 사용된 바이트
    // printf("Number of messages: %lu\n", buf.msg_qnum);          // 메시지 수
    // printf("Maximum bytes allowed: %lu\n", buf.msg_qbytes);    // 최대 바이트 수
    // char* buf = (char*)malloc(1024);
    // int ret = (msgsnd(res_msg_q_id, buf, sizeof(res_msg_t), IPC_NOWAIT));
    // printf("---START---\n");
    // for(int i=0;buf[i]!='\0';++i){
    //     printf("%c",buf[i]);
    // }
    // printf("----END----\n");
    //  if(ret == -1){
    //     perror("msgsnd");
    //     return 1;
    // }
    // return 0;

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