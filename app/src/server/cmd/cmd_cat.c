#include "cmd.h"

int cmd_cat(req_msg_t req, res_msg_t* res){
    printf("read file\n");
    char dir[MAX_PATH_LEN];
    if(append_path(req.cwd, req.args[0], dir, 0) != 0){
        perror("append path: mkfile");
        return 1;
    }
    
    FILE *fp; 
    if((fp = fopen(dir, "r")) == NULL){
        perror("fopen");
        return 1;
    }

    // 파일 크기 계산 
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);

    // 파일에 데이터가 있을 시에만 출력하도록 
    if(size > 0){
        rewind(fp);

        // 파일로부터 전체 데이터를 읽어서 버퍼에 저장 
        if(fread(res->text_buf, 1, size, fp) != NULL){
            // printf("%s", buf);
        }
        // free(buf);
    }

    printf("read:%s\n", res->text_buf);


    return 0;

}