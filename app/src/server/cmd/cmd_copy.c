#include "cmd.h"

void copy_recursive(char* source, char* dest){
    FILE* fp;
    // 파일 존재하는지 검사 
    if((fp = fopen(source, "r")) == NULL){
        perror("undefined file name");
        return 1;
    }

    // 파일 크기 계산 
    fseek(fp, 0 , SEEK_END);
    int size = ftell(fp);
    char* buf = (char*)malloc(size+1);
    rewind(fp);

    // 파일 내용 읽기
    if((fread(buf, 1, size, fp)) != NULL){
        FILE *out;
        if((out = fopen(dest, "w")) == NULL){ // 쓰기 모드로 복사 대상 파일 열기 (w모드라 없으면 생성됨)
            perror("undefined file name");
            return 1;
        }
        fputs(buf, out);
        fclose(out);
    }
}

int cmd_copy(req_msg_t req, res_msg_t* res){
    FILE *fp;
    DIR *dp;
    struct dirent *dent;

    char* source = req.args[0];
    char* dest = req.args[1];
    printf("copy\n");
    if(access(source, 0) == -1){
        perror("access: copy file");
        return 1;
    }
    printf("%s -> %s\n", source, dest);

    printf("Begin Search..\n");
    while(empty() != 1){ // stack이 빌 때까지 반복 
        char cwd[MAX_PATH_LEN];
        pop(cwd); // Pop() 
        printf("%s\n", cwd);
        dp = opendir(cwd); 

        if (dp == NULL)
        {
            perror("opendir");
            continue;
        }

        while ((dent = readdir(dp)))
        { 
            if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            {
                continue;
            }

            // 대상이 폴더라면, stack에 삽입 
            if((int)dent->d_type == 4){
                char tmp[MAX_PATH_LEN];
                sprintf(tmp, "%s/%s", cwd, dent->d_name);
                push(tmp);
            }
        }
    }
    closedir(dp); // 사용 끝났으니 닫기

    fclose(fp);
    get_file_list(req.cwd, &res->data);
    return 0;
}