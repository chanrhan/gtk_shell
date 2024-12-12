#include "cmd.h"

char stack[256][64];
int top_index = 0;

void push(char* data){
    strncpy(stack[top_index], data, 64);
    printf("push:%s\n",stack[top_index]);
    top_index++;
}
    
void pop(char* out){
    strncpy(out, stack[top_index-1], 64);
    strcpy(stack[top_index-1], "");
    top_index--;
}
    
// int top(){
//     return stack[top_index - 1];
// }
    
int empty(){
    if(top_index == 0 )
        return 1;
    return 0;
}

int size(){
    return top_index;
}

// DFS 탐색 
int cmd_search(req_msg_t req, res_msg_t* res){
    res->status = STATUS_FAIL;
    char* target = &req.args[0];

    DIR *dp;
    struct dirent *dent;
    char path[MAX_PATH_LEN];
    int first = 0;
    int find = 0;

    push(req.cwd);
    printf("Begin Search..\n");
    while(empty() != 1 && find == 0){ // stack이 빌 때까지 반복 
        // stack.pop
        char curr[MAX_PATH_LEN];
        pop(curr);
        char cwd[MAX_PATH_LEN];
        if(first == 0){
            sprintf(cwd, "%s", curr);
            first = 1;
        }else{
            sprintf(cwd, "%s/%s", path, curr);
        }
        printf("cwd:%s\n", cwd);
        dp = opendir(cwd); 

        if (dp == NULL)
        {
            perror("opendir");
            continue;
        }

        int has_child = 0;
        while ((dent = readdir(dp)))
        { 
            if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            {
                continue;
            }

            // 검색 대상 찾은 경우 
            if(strcmp(dent->d_name, target) == 0){
                strncpy(res->cwd, cwd, MAX_PATH_LEN);
                snprintf(res->find, MAX_PATH_LEN, "%s/%s", cwd, dent->d_name);
                // printf("find:%s\n", path);
                find = 1;
                break;
            }
            if((int)dent->d_type == 4){
                push(dent->d_name);
                has_child = 1;
            }
        }
        if(has_child == 1){
            strncpy(path, cwd, MAX_PATH_LEN);
        }
    }
    closedir(dp); // 사용 끝났으니 닫기

    if(find == 0){
        return 1;
    }

    get_file_list(res->cwd, &res->data);
    return 0;
}