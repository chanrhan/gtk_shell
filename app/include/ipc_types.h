#ifndef IPC_TYPES_H
#define IPC_TYPES_H

#define CMD_LS 1 // ls
#define CMD_PS 

#define CMD_MK_DIR 10 // \ud3f4\ub354 \uc0dd\uc131
#define CMD_MK_FILE 11 // \ud30c\uc77c \uc0dd\uc131 (cat)
#define CMD_MK_LN 12  // \ub9c1\ud06c \ud30c\uc77c \uc0dd\uc131 

#define CMD_MV 20
#define CMD_RENAME 21
#define CMD_CP 22
#define CMD_RM_DIR 23
#define CMD_RM_FILE 24
#define CMD_CH_FILE 25
#define CMD_EXEC 26

#define CMD_KILL 40

#define CMD_QUIT 90

#define STATUS_OK 0
#define STATUS_FAIL 1

#define MSG_MAX_SIZE 32
#define FILE_DISPLAY_LIMIT 8

#define MSG_END '\0'
#define MSG_TRUE '0'
#define MSG_FALSE '1'

#define FILE_END -1
#define CWD_LEN 32

#define ARG_SIZE 16

typedef struct file_info_t{
    int type;
    char name[16];
    char atime[12];
    int size;
} file_info_t;

typedef struct msg_data_t{
    int file_len;
    file_info_t files[FILE_DISPLAY_LIMIT];
} msg_data_t;

typedef struct req_msg_t{
    long mtype;
    int cmd;
    char cwd[32];
    char args[4][16];
} req_msg_t;

typedef struct res_msg_t{
    long mtype;
    int status;
    // int error_code;
    char cwd[32];
    msg_data_t data;
} res_msg_t;

#endif
