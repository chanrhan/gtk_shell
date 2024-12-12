#include "cmd.h"

int cmd_mkfile(req_msg_t req, res_msg_t *res){
    char* target;
    if(append_path(req.cwd, req.args[0], target, 1) != 0){
        perror("append path: mkfile");
        return 1;
    }
    FILE *fp; 
    if((fp = fopen(target, "w")) == NULL){
        return 1;
    }
    char** line = (char**)malloc(64);
    int i=0;
    // 파일에 쓰기 
    while(i< 64){
        line[i] = (char*)malloc(2048);
        gets(line[i]); // gets : 개행문자(\n)까지의 모든 문자열 읽어들임 (\n은 NULL로 대치되서 저장됨)
                        // gets_s : 버퍼 크기 지정까지 되는 safe-함수 
                        // scanf : \0(NULL) 까지의 문자열 읽어들임 
        if(feof(stdin)){ // ctrl+D 누르면 입력 종료되게끔 함. 
            clearerr(stdin); // 해당 프로그램 종료 방지 
            break;
        }
        ++i;
    }

    // 표준 스트림으로부터 입력받은 데이터의 출력 스트림을 파일로 지정 
    for(int j=0;j<i;++j){
    if(line[j])	
        fprintf(fp, "%s\n", line[j]);
        // free(line[j]);
    }
    free(line);
    return 0;
}