#include "cmd.h"

cmd_t cmd_list[] = {
    {CMD_LS, cmd_ls},
    {CMD_LS_FULL, cmd_ls_full},
    {CMD_MK_DIR, cmd_mkdir}, 
    {CMD_MK_FILE, cmd_mkfile},
    {CMD_MK_LN, cmd_mkln},
    {CMD_RENAME, cmd_rename},
    {CMD_RM_DIR, cmd_rmdir},
    {CMD_RM_FILE, cmd_rmfile},
    {CMD_MV, cmd_move}, 
    {CMD_CP, cmd_copy},
    {CMD_EXEC, cmd_execute},
    {CMD_PS, cmd_ps},
    {CMD_KILL, cmd_kill},
    {CMD_SEARCH, cmd_search}
};

int cmd_list_size = sizeof(cmd_list) / sizeof(cmd_t);

void get_file_list(char *pwd, msg_data_t *data)
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
        // printf("[%s] a:%u,s:%u,m:%u", statbuf.st_atimespec);

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
        strncpy(data->files[i].mtime, gettime_str(statbuf.st_mtime), MTIME_LEN);
        data->files[i].size = statbuf.st_size;

        ++i;
        if (i >= MAX_FILE_LIST_SIZE - 1)
        {
            break;
        }
    }
    data->file_len = i;
    printf("file_len: %d\n", data->file_len);

    closedir(dp); // 사용 끝났으니 닫기
}

int append_path(char *org_cwd, char *append, char* cwd, int mode)
{
    printf("%s, %s, %s\n", org_cwd, append, cwd);
    if (append[0] != '\0')
    {
        if (strcmp(append, "..") == 0)
        {
            // 최상위 ROOT 디렉토리를 벗어나면 
            if(strcmp(org_cwd, ROOT_DIR) == 0 || strncmp(org_cwd, ROOT_DIR, strlen(ROOT_DIR)) != 0){
                strncpy(cwd, org_cwd, MAX_PATH_LEN);
                return 1;
            }
            printf("cwd:%s\n", cwd);
            // 경로의 마지막 디렉토리를 잘라내어 상위 경로로 이동 
            char* last_slash = strrchr(cwd, '/');
            *last_slash = '\0';
            printf("cwd:%s\n", cwd);
        }
        else
        {
            snprintf(cwd, MAX_PATH_LEN, "%s/%s", org_cwd, append);
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
        strncpy(cwd, org_cwd, MAX_PATH_LEN);
    }
    
    return 0;
}







