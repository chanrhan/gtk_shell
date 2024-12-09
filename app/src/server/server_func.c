#include "server_func.h"

void get_ls(char *pwd, msg_data_t *data)
{
    DIR *dp;
    struct dirent *dent;

    dp = opendir(pwd); // 현재 디렉토리 열기
    if (dp == NULL)
    {
        perror("opendir");
        exit(1);
    }

    int i = 0;
    while ((dent = readdir(dp)))
    { // 파일을 하나씩 읽어옴, 다 읽을 때까지 반복
        if (strcmp(dent->d_name, ".") == 0)
        {
            continue;
        }

        // stat 명령어를 통해 파일의 상세 정보 가져와서 출력
        struct stat statbuf;
        stat(dent->d_name, &statbuf);

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
                 (mode & 0400) ? 'r' : '-',
                 (mode & 0200) ? 'w' : '-',
                 (mode & 0100) ? 'x' : '-',
                 (mode & 0040) ? 'r' : '-',
                 (mode & 0020) ? 'w' : '-',
                 (mode & 0010) ? 'x' : '-',
                 (mode & 0004) ? 'r' : '-',
                 (mode & 0002) ? 'w' : '-',
                 (mode & 0001) ? 'x' : '-');

        data->files[i].type = (int)dent->d_type;
        strncpy(data->files[i].name, dent->d_name, 16);
        strncpy(data->files[i].atime, gettime_str(statbuf.st_atime), 12);
        data->files[i].size = statbuf.st_size;

        ++i;
        if (i >= FILE_DISPLAY_LIMIT - 1)
        {
            break;
        }
    }
    data->file_len = i;
    // printf("file_len: %d\n", data->file_len);

    closedir(dp); // 사용 끝났으니 닫기
}

int validate_path(char *org_cwd, char *append, char* cwd, int mode)
{
    if (append[0] != '\0')
    {
        if (strcmp(append, "..") == 0)
        {
            // 최상위 ROOT 디렉토리를 벗어나면 
            if(strcmp(org_cwd, ROOT_DIR) == 0 || strncmp(ROOT_DIR, org_cwd, strlen(ROOT_DIR)) != 0){
                strncpy(cwd, org_cwd, CWD_LEN);
                return 1;
            }
            printf("cwd:%s\n", cwd);
            char* last_slash = strrchr(cwd, '/');
            *last_slash = '\0';
            printf("cwd:%s\n", cwd);
        }
        else
        {
            snprintf(cwd, CWD_LEN, "%s/%s", org_cwd, append);
        }
        if (mode == 0 && access(cwd, 0) == -1)
        {
            perror("access");
            // exit(1);
            return 1;
        }
    }else{
        if (mode == 0 && access(org_cwd, 0) == -1)
        {
            perror("access");
            return 1;
        }
        strncpy(cwd, org_cwd, CWD_LEN);
    }
    
    return 0;
}

int cmd_ls(req_msg_t req, res_msg_t *res)
{
    if(validate_path(req.cwd, req.args[0], &res->cwd, 0) != 0){
        printf("access failed!");
        return 1;
    }
    get_ls(res->cwd, &res->data);

    return 0;
}

int cmd_mkdir(req_msg_t req, res_msg_t *res)
{
    char cwd[CWD_LEN];
    printf("cwd:%s,mk:%s,mode:%d\n", req.cwd, req.args[0], (mode_t)atoi(req.args[1]));
    if(validate_path(req.cwd, req.args[0], &cwd, 1) != 0){
        perror("validat_path");
        return 1;
    }

    printf("validate cwd:%s\n", cwd);
    int ret = mkdir(cwd, (mode_t)atoi(req.args[1]));
    if(ret != 0){
        // strncpy(res->ERROR, strerror(errno), 32);
    }
    get_ls(req.cwd, &res->data);
    return ret;
}

int cmd_mk(req_msg_t req, res_msg_t *res){
    
}

int cmd_rmdir(req_msg_t req, res_msg_t *res)
{
    int ret = rmdir(req.args[0]);
    get_ls(req.cwd, &res->data);
    return ret;
}

int cmd_rename(req_msg_t req, res_msg_t *res)
{
    char* cwd;
    if(validate_path(req.cwd, req.args[0], cwd, 0) != 0){
        perror("validat_path");
        return 1;
    }
    int ret = rename(cwd, req.args[1]);
    get_ls(req.cwd, &res->data);
    return ret;
}