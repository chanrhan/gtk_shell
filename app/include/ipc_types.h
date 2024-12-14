#ifndef IPC_TYPES_H
#define IPC_TYPES_H

#define MSG_Q_REQ_PROJ_ID 101
#define MSG_Q_RES_PROJ_ID 201

#define DISPLAY_FILE 150
#define DISPLAY_PROCESS 151

#define CMD_LS 1 // ls
#define CMD_LS_FULL 2 // entire directory
#define CMD_PS 3

#define CMD_MK_DIR 10 // \ud3f4\ub354 \uc0dd\uc131
#define CMD_MK_FILE 11 // \ud30c\uc77c \uc0dd\uc131 (cat)
#define CMD_MK_LN 12  // \ub9c1\ud06c \ud30c\uc77c \uc0dd\uc131 

#define CMD_MV 20 // 이동 
#define CMD_RENAME 21 // 이름, 접근 권한 수정 (한번에)
#define CMD_CP 22 // 복사 
#define CMD_RM_DIR 23 // 폴더 삭제 
#define CMD_RM_FILE 24 // 파일 삭제
#define CMD_CH_PERM 25 // 파일 권한 수정 
#define CMD_CAT 26
#define CMD_OPEN_FILE 27 // 파일 정보 열기

#define CMD_EXEC 29 // 파일 실행 
#define CMD_SEARCH 30

#define CMD_KILL 40

#define CMD_QUIT 90

#define STATUS_OK 0
#define STATUS_FAIL 1

#define MSG_MAX_SIZE 32
#define MAX_FILE_LIST_SIZE 16

#define MSG_END '\0'
#define MSG_TRUE '0'
#define MSG_FALSE '1'

#define FILE_END -1
#define MAX_PATH_LEN 64

#define MAX_ARGV_SIZE 64
// #define MAX_FILENAME_SIZE 64
#define MTIME_LEN 9

#define MAX_TXB_SIZE 1

// extern long SYSTEM_MSGMAX;

typedef struct file_info_t{
    int type;
    int size;
    int perm;
    char name[MAX_PATH_LEN];
    char mtime[MTIME_LEN];
    char birthtime[MTIME_LEN];
} file_info_t;

typedef struct msg_data_t{
    int file_len;
    file_info_t files[MAX_FILE_LIST_SIZE];
} msg_data_t;

typedef struct req_msg_t{
    long mtype;
    int cmd;
    char cwd[MAX_PATH_LEN];
    char args[4][MAX_ARGV_SIZE];
    char text_buf[MAX_TXB_SIZE];
} req_msg_t;

typedef struct res_msg_t{
    long mtype;
    int status;
    int error_code;
    char cwd[MAX_PATH_LEN];
    char find[MAX_PATH_LEN];
    msg_data_t data;
    char text_buf[MAX_TXB_SIZE];
} res_msg_t;

#endif
