#include "cmd.h"

int cmd_copy(req_msg_t req, res_msg_t* res){
    FILE *fp;

    char* source = req.args[0];
    char* dest = req.args[1];
    printf("copy\n");
    if(access(source, 0) == -1){
        perror("access: copy file");
        return 1;
    }
    printf("%s -> %s\n", source, dest);

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

    fclose(fp);
    get_file_list(req.cwd, &res->data);
    return 0;
}