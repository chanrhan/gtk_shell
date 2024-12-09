#ifndef IPC_H
#define IPC_H

#include "common.h"

extern int req_msg_q_id;
extern int res_msg_q_id;

int create_msg_q(const char* filename, int proj_id);


#endif
