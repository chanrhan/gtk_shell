#include "cmd.h"

#define MAX_QUEUE_SIZE 256

// 문자열 큐 (GPT)
char queue[MAX_QUEUE_SIZE][MAX_PATH_LEN];
int front_index = 0;
int rear_index = 0;

// 큐가 비어있는지 확인
int is_empty() {
    return front_index == rear_index;
}

// 큐가 가득 찼는지 확인
int is_full() {
    return (rear_index + 1) % MAX_QUEUE_SIZE == front_index;
}

// 큐에 데이터 추가 (Enqueue)
int enqueue(const char* data) {
    if (is_full()) {
        printf("Queue is full!\n");
        return -1; // 실패
    }
    strncpy(queue[rear_index], data, MAX_PATH_LEN);
    rear_index = (rear_index + 1) % MAX_QUEUE_SIZE;
    return 0; // 성공
}

// 큐에서 데이터 제거 (Dequeue)
int dequeue(char* out) {
    if (is_empty()) {
        printf("Queue is empty!\n");
        return -1; // 실패
    }
    strncpy(out, queue[front_index], MAX_PATH_LEN);
    front_index = (front_index + 1) % MAX_QUEUE_SIZE;
    return 0; // 성공
}

int copy_file(char* source, char* dest){
    printf("copy:%s->%s\n", source, dest);
    FILE* fp;
    // 파일 존재하는지 검사 
    if((fp = fopen(source, "r")) == NULL){
        perror("undefined file name");
        return 1;
    }

    // printf("111\n");
    // 파일 크기 계산 
    fseek(fp, 0 , SEEK_END);
    int size = ftell(fp);
    char* buf = (char*)malloc(size+1);
    rewind(fp);

    // printf("222\n");

    // 파일 내용 읽기
    if((fread(buf, 1, size, fp)) != NULL){
    // printf("333\n");

        FILE *out;
        if((out = fopen(dest, "w")) == NULL){ // 쓰기 모드로 복사 대상 파일 열기 (w모드라 없으면 생성됨)
            perror("undefined file name");
            return 1;
        }
        fputs(buf, out);
        fclose(out);
    }
    // printf("444\n");

    fclose(fp);
    return 0;
}

int cmd_copy(req_msg_t req, res_msg_t* res){
    

    char* source = req.args[0];
    char* dest = req.args[1];
    if(access(source, 0) == -1){
        perror("access: copy file");
        return 1;
    }
    FILE *fp;
    struct stat statbuf;
    // printf("copy: %s -> %s\n", source, dest);

    stat(source, &statbuf);

    // 복사할 대상이 파일인지 폴더인지 확인
    if (S_ISREG(statbuf.st_mode)) {
        printf("IS FILE\n");
        // 파일은 그냥 복사
        int ret = copy_file(source, dest);
        if(ret == 1){
            return 1;
        }
    } else if (S_ISDIR(statbuf.st_mode)) {
        printf("IS DIRECTORY\n");
        char* root = &source;
        int root_len = strlen(source);
        printf("root len:%d\n", root_len);
        enqueue(source);

        DIR *dp;
        struct dirent *dent;
        // 폴더는 재귀적으로 구조를 복사
        printf("BFS Start..\n");
        while(is_empty() != 1){ // queue가 빌 때까지 반복 
            char cwd[MAX_PATH_LEN];
            dequeue(cwd);
            printf("cwd: %s\n", cwd);
            dp = opendir(cwd); 

            if (dp == NULL)
            {
                perror("opendir");
                continue;
            }
            char copy_dir[MAX_PATH_LEN];
            if(*(cwd+root_len) == '\0'){
                snprintf(copy_dir, MAX_PATH_LEN, "%s", dest);
            }else{
                snprintf(copy_dir, MAX_PATH_LEN, "%s/%s", dest, cwd+root_len);
            }
            printf("copy dir: %s\n", copy_dir);
            if(access(copy_dir, 0) == -1){
                mkdir(copy_dir, statbuf.st_mode);
            }

            while ((dent = readdir(dp)))
            { 
                if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
                {
                    continue;
                }

                char curr[MAX_PATH_LEN];
                sprintf(curr, "%s/%s", cwd, dent->d_name);
                char copy_path[MAX_PATH_LEN];
                printf("cut:%s\n", cwd+root_len);
                if(*(cwd+root_len) == '\0'){
                    snprintf(copy_path, MAX_PATH_LEN, "%s/%s", dest, dent->d_name);
                }else{
                    snprintf(copy_path, MAX_PATH_LEN, "%s/%s/%s", dest, cwd+root_len, dent->d_name);
                }
                
                // 대상이 폴더라면, stack에 삽입하고 폴더 생성
                if((int)dent->d_type == 4){
                    printf("make dir\n");
                    enqueue(curr);
                    stat(curr, &statbuf);
                    
                }else if((int)dent->d_type == 8){
                    // 파일이라면, 즉시 복사
                    copy_file(curr, copy_path);
                }
            }
        }
        closedir(dp); // 사용 끝났으니 닫기
    } else {
        perror("Not directory nor File");
        return 1;
    }

    printf("444\n");
    

    
    get_file_list(req.cwd, &res->data);
    return 0;
}