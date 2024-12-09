#ifndef IPC_SERVER_H
#define IPC_SERVER_H

#include "server_common.h"

int send_to_view(res_msg_t* res);

// 클라이언트로부터 요청 기다리는 함수
// 읽어온 데이터의 크기를 반환 
int wait_rcv(req_msg_t* req);

#endif