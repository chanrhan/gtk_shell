#include "server_common.h"

const char* ROOT_DIR = "/tmp/test";

cmd_t cmd_list[] = {
    {CMD_LS, cmd_ls},
    // {CMD_ACCESS, cmd_cd},
    {CMD_MK_DIR, cmd_mkdir},
    {CMD_RM_DIR, cmd_rmdir},
    {CMD_RENAME, cmd_rename}
};

int cmd_list_size = sizeof(cmd_list) / sizeof(cmd_t);

pair_string_int_t type_list[]={
    {S_IFSOCK, 's'},
    {S_IFLNK, 'l'},
    {S_IFREG, 'r'},
    {S_IFBLK, 'b'},
    {S_IFDIR, 'd'},
    {S_IFCHR, 'c'},
    {S_IFIFO, 'f'}
};

int type_list_size = sizeof(type_list) / sizeof(pair_string_int_t);


int select_cmd(req_msg_t req, res_msg_t* res){
    for(int i=0;i<cmd_list_size;++i){
        if(req.cmd == cmd_list[i].cmd){
            return cmd_list[i].cmd_func(req, res);
        }
    }
    return 1;
}

struct tm* gettime(time_t timestamp){
	struct tm* tmp;
	tmp = localtime(&timestamp);
	return tmp;
}

char* gettime_str(time_t timestamp){
	struct tm* tmp;
	tmp = localtime(&timestamp);
	char* str = (char*)malloc(9);
	sprintf(str, "%.2d%.2d%.2d%.2d", tmp->tm_mon+1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min);
	return str;
}