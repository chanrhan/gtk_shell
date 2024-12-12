#include "ipc.h"

int create_msg_q(const char* filename, int proj_id){
    
    key_t key = ftok(filename, proj_id);

    int msg_q_id = msgget(key, IPC_CREAT|0644);
    if(msg_q_id == -1){
        perror("msgget");
        exit(1);
    }

    return msg_q_id;
}
