#include "cmd.h"

int cmd_ps(req_msg_t req, res_msg_t* res){
    DIR* dir;
    struct dirent* dent;
    char proc_file[64];
    struct stat statbuf;
    
    if((dir = opendir("/proc")) == NULL){
        perror("opendir");
        return 1;
    }


    int opt = 0;
    char flags[4] = {0x00,}; // 옵션 플래그 비트 배열
    int target_proc = 0;


        // -u 옵션 시 CPU 사용률 계산을 위해 uptime 가져오기 
        // 시스템 부팅 후 시간은 /proc/uptime 의 첫 필드에 있음 
        int uptime;
        if(flags[1] == 1){
            char buf[128];
            FILE* fp;
            if((fp = fopen("/proc/uptime","r")) == NULL){
                    perror("fopen proc/uptime");
                    return 1;
            }
            fgets(buf, 128, fp);
            sscanf(buf, "%d", &uptime);
            fclose(fp);
        }

        // 정보 출력
        if(flags[1] == 1 || flags[2] == 1){
            printf("%-20s ", "UID");
        }
        printf("%5s ", "PID");
        if(flags[1] == 1){
            printf("%5s ", "PPID");
            // printf("%2s ", "C");
            printf("%-5s ", "STIME");
        }
        printf("%-7s ", "TTY");
        printf("%7s ", "TIME");
        printf("%-s", "CMD");
        printf("\n");

        // /proc 내에 모든 pid 정보 읽기 
        while((dent = readdir(dir)) != NULL){
        if(strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0){
            // 실행중인 프로세스의 정보가 있는 경로
            sprintf(proc_file, "/proc/%s/stat", dent->d_name);
   
	        if(access(proc_file, F_OK) != 0){
    		    continue;
            }

            if(atoi(dent->d_name) != 0){
                 // -p 옵션 시 타켓 pid가 아니라면 스킵 
                if(target_proc != 0 && target_proc != atoi(dent->d_name)){
                    continue;
                }
                FILE* fp;
                struct stat statbuf;
                
                        char buf[512];
                        int i=0;
                if((fp = fopen(proc_file, "r")) == NULL){
                    perror("fopen");
                    return 1;
                }
                // 파일을 열어서 /proc/[pid]/stat의 문자열을 모두 가져옴 
                fgets(buf, 511, fp);

                // 띄어쓰기를 구분자로 하여 모든 필드값 가져옴 
                char* field[52];
                char* token = strtok(buf," ");
                int k=0;

                while(token != NULL && k < 52){
                    field[k++] = token;
                    token = strtok(NULL," ");
                }
                
                // /proc/[pid]/stat 필드 
                // 0: pid
                // 1: proc_cmd
                // 3: ppid
                // 6: tty_nr
                // 21: starttime
                if(flags[1] ==1 || flags[2] ==1){
                    // stat.st_uid 기반 유저명 가져오기
                    stat(proc_file, &statbuf);
                    printf("%-20s ", getpwuid(statbuf.st_uid)->pw_name);
                }
                printf("%5s ", field[0]); // pid
                if(flags[1] == 1){
                    printf("%5s ", field[3]); // ppid
                    int starttime = atoi(field[21]);
                    printf("%-5d ", starttime);
                }
                printf("%-7s ", field[6]); // tty
                printf("%7s ", field[21]); // time
                char* cmd = (char*)malloc(strlen(field[1]));
                snprintf(cmd, strlen(field[1])-1, "%s", ++field[1]); // 문자열 양 끝 '(',')' 잘라내기 
                printf("%-s ", cmd); // cmd
                printf("\n");

                fclose(fp);
            }
        }	
    }
    
    return 0;
}