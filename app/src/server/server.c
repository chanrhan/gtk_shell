/*
    기말 과제 1

    이름: 박희찬
    학번: 2019136066
*/
#include "server.h"

// Ctrl+C 시그널 콜백 함수
void ignoreSIGINT(int sig){
    // do nothing
}


int main(int argc, char **argv)
{
    req_msg_q_id = create_msg_q("keyfile", MSG_Q_REQ_PROJ_ID);
    res_msg_q_id = create_msg_q("keyfile", MSG_Q_RES_PROJ_ID);
    
    // tzset();
    char *command, *tok_str;
    char *cmd_argv[4];
    char cmd_argc;
    char *current_dir = "/";

    int ret=0; // return result
    
    command = (char*) malloc(MAX_CMD_SIZE);
    if (command == NULL) { // 미할당 시를 고려한 예외 처리 
        perror("malloc");
        exit(1);
    }
    
    // /tmp/test 가 있는지 확인하고, 없다면 폴더 생성 
    if(access("/tmp", 0) == -1){
        int rst = mkdir("/tmp",0755);
        if(rst){
            return 1;
        }
    }

    if(access("/tmp/test",0) == -1){
        int rst = mkdir("/tmp/test",0755);
        if(rst){
            return 1;
        }
    }

    if(access("/tmp/test/desktop",0) == -1){
        int rst = mkdir("/tmp/test/desktop",0755);
        if(rst){
            return 1;
        }
    }

    if(access("/tmp/test/documents",0) == -1){
        int rst = mkdir("/tmp/test/documents",0755);
        if(rst){
            return 1;
        }
    }

    if(access("/tmp/test/downloads",0) == -1){
        int rst = mkdir("/tmp/test/downloads",0755);
        if(rst){
            return 1;
        }
    }

    // 프로그램을 시작하자마자 최상위 디렉토리로 이동하도록 설정 
    // ROOT_DIR = "/tmp/test";
    // if(chdir(ROOT_DIR) != 0){
    //     return 1;
    // }

    // Ctrl+C 인터럽트 무시
    // signal(SIGINT, ignoreSIGINT);
    
    while(1){
        req_msg_t req_msg_buf;
        res_msg_t res_msg_buf;

        int len = wait_rcv(&req_msg_buf);  

        if(len > 0){
            if(req_msg_buf.cmd == CMD_QUIT){
                printf("QUIT FROM CLIENT\n");
                break;
            }
            ret = select_cmd(req_msg_buf, &res_msg_buf);
            if(ret == 0){
                res_msg_buf.status = STATUS_OK;
            }else{
                res_msg_buf.status = STATUS_FAIL;
            }
            
            if(send_to_view(&res_msg_buf) == 0){
                printf("\nsuccess!\n");
                continue;
            }
            perror("\ncommand failed\n");
        }
    }
    msgctl(req_msg_q_id, IPC_RMID, (struct msqid_ds*)NULL);
    // msgctl(res_msg_q_id, IPC_RMID, (struct msqid_ds*)NULL);

    // do {
    //     // /tmp/test 문자열을 찾고 그 뒤에만 (현재 디렉토리명)으로 표시  
    //     current_dir = strstr(getcwd(NULL, MAX_CMD_SIZE), "/tmp/test")+9; // 문자열 찾기 
    //     if(strlen(current_dir) <= 0 || current_dir == NULL){  // 최상위 디렉토리면 '/' 문자 추가 
    //         current_dir = "/";
    //     }

    //     printf("%s $ ", current_dir); // prompt 띄우기
    //     if (fgets(command, MAX_CMD_SIZE-1, stdin) == NULL) break; // 명령 대기 

    //     tok_str = strtok(command, " \n");
    //     if (tok_str == NULL) continue; 

    //     if (strcmp(tok_str, "quit") == 0) { // 프로그램 종료 감지 
    //         break;
    //     } else {
    //         // TODO: implement functions
    //         // printf("your command: %s\n", tok_str);
    //         // printf("and argument is ");
    //         cmd_argv[0] = tok_str; // cmd 저장

    //         for(cmd_argc =1;cmd_argc < 4; ++cmd_argc){
    //             if(tok_str = strtok(NULL, " \n")){
    //                 cmd_argv[cmd_argc] = tok_str;
    //             }else{
    //                 break;
    //             }
    //         }
        

    //         // 명령어 파악 후 명령어 목록에서 검색
    //         int cmd_idx = search_command(cmd_argv[0]);
    //         if(cmd_idx < 0){
    //             // 프로그램 실행 명령어(./) 인지 확인
    //             // exevp 함수의 인자 배열의 마지막에는 NULL이 들어가야 하기 때문에 NULL문자 삽입 
    //             if(strncmp(cmd_argv[0], "./", 2) == 0){
    //                 char* exe_args[argc+1]; 
    //                 for(int k=0;k<argc;++k){
    //                     exe_args[k] = cmd_argv[k];
    //                 }
    //                 exe_args[argc] = NULL;
    //                 ret = cmd_execute(exe_args); // 프로그램 실행 명령어 
    //             }else{
    //                         printf("%s : command not found\n", cmd_argv[0]);
    //                     }
    //             }else if(cmd_list[cmd_idx].cmd_func){
    //                         // printf("argc: %d\n", cmd_argc);
    //                         if(cmd_argc < cmd_list[cmd_idx].min_argc){
    //                             fprintf(stderr, "Invalid number of command arguments\n");
    //                             printf("usage: %s\n", cmd_list[cmd_idx].usage);
    //                         }else{
    //                             ret = cmd_list[cmd_idx].cmd_func(cmd_argc, cmd_argv); // 명령어 실행 
    //                             if(ret == 0){
    //                                 printf("\nsuccess!\n");
    //                             }else{
    //                                 perror("\ncommand failed\n");
    //                                 printf("usage: %s\n", cmd_list[cmd_idx].usage);
    //                             }
    //                         }   
    //             }
    //         optind = 0; // getopt init 

    //         printf("\n");
    //     }
    // } while (1);

   // free(command);
   // free(tok_str);  
   // free(current_dir);

    return 0;
}
