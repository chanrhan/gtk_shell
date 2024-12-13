#ifndef CMD_H
#define CMD_H

#include "ipc_server.h"

#define DECLARE_CMDFUNC(str) int cmd_##str(req_msg_t req, res_msg_t* res)

// 명령어 구현 함수 선언
DECLARE_CMDFUNC(ls); 
DECLARE_CMDFUNC(ls_full); 

DECLARE_CMDFUNC(mkdir); // MK_DIR
DECLARE_CMDFUNC(mkfile); // MK_FILE
DECLARE_CMDFUNC(mkln); // MK_LN

DECLARE_CMDFUNC(rename); // RENAME
DECLARE_CMDFUNC(rmdir); // RM_DIR
DECLARE_CMDFUNC(rmfile); // RM_FILE

DECLARE_CMDFUNC(move); // MK_MV
DECLARE_CMDFUNC(copy); // MK_CP

DECLARE_CMDFUNC(execute); // MK_EXEC

DECLARE_CMDFUNC(ps); // MK_PS 
DECLARE_CMDFUNC(kill); // MK_KILL

DECLARE_CMDFUNC(search); // SEARCH
DECLARE_CMDFUNC(cat); // SEARCH

DECLARE_CMDFUNC(open_detail); // SEARCH



// 구현 명령어 목록 
extern cmd_t cmd_list[];

extern int cmd_list_size;

void get_file_list(char *pwd, msg_data_t *data);

int append_path(char *org_cwd, char *append, char* cwd, int mode);

#endif