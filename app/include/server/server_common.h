#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <sys/times.h>
#include <limits.h>
#include "ipc.h"
#include "ipc_types.h"

#define MAX_CMD_SIZE 1024

// 최상위 디렉토리
extern const char* ROOT_DIR;

typedef int (*cmd_func_t)(req_msg_t req, res_msg_t* res);
typedef struct cmd_t
{
    int cmd;
    cmd_func_t cmd_func;
} cmd_t;

typedef struct pair_string_int_t{
    int type;
    char name;
} pair_string_int_t;

// 파일 종류 목록
extern pair_string_int_t type_list[];

extern int type_list_size;


// 구현 명령어 목록 
extern cmd_t cmd_list[];

extern int cmd_list_size;


int select_cmd(req_msg_t req, res_msg_t* res);

char* gettime_str(time_t timestamp);
struct tm* gettime(time_t timestamp);

#endif