#include "cmd.h"

int cmd_open_detail(req_msg_t req, res_msg_t* res){
    char source[MAX_PATH_LEN];
    if(append_path(req.cwd, req.args[0], source, 0) != 0){
        perror("append path: mkfile");
        return 1;
    }
    printf("open_detail\n");
    file_info_t* file_detail = &res->data.files[0];

    // stat 명령어를 통해 파일의 상세 정보 가져와서 출력
    struct stat statbuf;
    stat(source, &statbuf);

    unsigned int mode = statbuf.st_mode;

    char kind = 0;

    for (int i = 0; i < type_list_size; ++i)
    {
        if ((mode & S_IFMT) == type_list[i].type)
        {
            kind = type_list[i].name;
            break;
        }
    }

    // 권한 비트마스크를 통해 파일 권한 문자열(rwxrwxrwx) 생성
    char *perm = (char *)malloc(11);

    snprintf(perm, 11, "%c%c%c%c%c%c%c%c%c%c",
                kind,
                (mode & 0400) ? 'r' : '-', // 256
                (mode & 0200) ? 'w' : '-', // 128
                (mode & 0100) ? 'x' : '-', // 64
                (mode & 0040) ? 'r' : '-',
                (mode & 0020) ? 'w' : '-',
                (mode & 0010) ? 'x' : '-',
                (mode & 0004) ? 'r' : '-',
                (mode & 0002) ? 'w' : '-',
                (mode & 0001) ? 'x' : '-');

    strncpy(file_detail->mtime, gettime_str(statbuf.st_mtime), MTIME_LEN);
    strncpy(file_detail->birthtime, gettime_str(statbuf.st_mtime), MTIME_LEN);
    file_detail->size = statbuf.st_size;
    file_detail->perm = mode;

    return 0;
}