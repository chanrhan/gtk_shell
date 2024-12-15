#ifndef IPC_VIEW_H
#define IPC_VIEW_H

#include "view_common.h"

int send_to_server(req_msg_t* req);

int wait_rcv(res_msg_t* res);

int send_wait_rcv(req_msg_t* req, res_msg_t* res);

#endif