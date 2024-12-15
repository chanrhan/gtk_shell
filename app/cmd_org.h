#ifndef CMD_ORG_H
#define CMD_ORG_H

#include <pwd.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <sys/times.h>
#include <limits.h>
#include "cmd_config.h"

#define CLK_TCK sysconf(_SC_CLK_TCK) 

// kill -l 명령어 입력 시 출력할 시그널 목록들
char* sig_list[] = {"SIGHUP","SIGINT","SIGQUIT","SIGILL","SIGTRAP","SIGABRT","SIGIOT","SIGBUS","SIGFPE","SIGKILL","SIGUSR1","SIGSEGV","SIGUSR2","SIGPIPE","SIGALRM","SIGTERM","SIGSTKFLT","SIGCHILD","SIGCONT","SIGSTOP","SIGTSTP","SIGTTIN","SIGTTOU","SIGURG","SIGXCPU","SIGXCPU","SIGXFSZ","SIGVTALRM","SIGPROF","SIGWINCH","SIGIO","SIGPOLL","SIGPWR","SIGSYS","SIGUNUSED", NULL};


int search_command(char*);

int search_command(char* cmd){
    for(int i=0;i<cmd_list_size;++i){
        if(strcmp(cmd, cmd_list[i].cmd_str) == 0){
            return i;
        }
    }
    return -1;
}

// help 명령어
// 과제에서 요구한 기능 명령어들에 대한 설명을 출력한다 
int cmd_help(int argc, char** argv){
    for(int i=0;i<cmd_list_size;++i){
        printf("%-6s : %-2s\n", cmd_list[i].cmd_str, cmd_list[i].description);
    }
    
    return 0;
}

// cd 명령어
// 최상위 디렉토리 (/tmp/test) 를 벗어나지 않도록 설계 
int cmd_cd(int argc, char** argv){
    char* path = argv[1];
    if(!path){
        return 1;
    }

    if(strcmp(path, "..") == 0){ // ..을 통해 ROOT 디렉토리를 벗어나는 것을 방지 
        char* curr_dir = getcwd(NULL, MAX_CMD_SIZE); // 현재 작업위치 

        if(strcmp(curr_dir, ROOT_DIR) == 0){
            return 0;
        }
    }
    
    return chdir(root_path(path));
}

// mkdir 명령어
int cmd_mkdir(int argc, char** argv){
    return mkdir(root_path(argv[1]), 0755);
}

// 디버깅용 pwd 명령어
// (과제 내용 포함 X)
int cmd_pwd(int argc, char** argv){
    char* pwd = getcwd(NULL, MAX_CMD_SIZE);
    int rst = checkError((pwd != NULL) ? 0 : 1, "command failed by 'pwd': ");
    if(!rst){
        printf("%s\n", pwd);
    }
    return rst;
}


// rmdir 명령어 
int cmd_rmdir(int argc, char** argv){
    return rmdir(root_path(argv[1]));
}

// rename 명령어 
int cmd_rename(int argc, char** argv){
    return rename(root_path(argv[1]), root_path(argv[2]));
}


// ls 명령어
// 현재 디렉토리의 파일 정보를 목록으로 출력
int cmd_ls(int argc, char** argv){
    int tab_count = 0;
    int opt = 0;
    int show_all = 0;
    int show_long = 0;

    // 옵션 파싱 
    while((opt = getopt(argc, argv, "al")) != -1){
        switch(opt){
            case 'a': // 전체 파일 보여주는 옵션
                show_all = 1;
                break;
            case 'l': // 자세한 정보 보여주는 옵션 
                show_long = 1;
                break;
            default:
                break;
        }
    }
    DIR* dp;
    struct dirent *dent;

    dp = opendir("."); // 현재 디렉토리 열기 

    while((dent = readdir(dp))){ // 파일을 하나씩 읽어옴, 다 읽을 때까지 반복 
        // -a 옵션이 없다면 ., .. 파일은 넘어감
        if(show_all == 0){
            if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0){
                continue;
            }
        }

        // stat 명령어를 통해 파일의 상세 정보 가져와서 출력 
        struct stat statbuf;
        stat(dent->d_name, &statbuf);

        unsigned int mode = statbuf.st_mode;
        
        // -l 옵션일 때만 출력
        if(show_long == 1){
            char kind = 0;

            for(int i=0;i<type_list_size;++i){
                    if((mode & S_IFMT) == type_list[i].type){
                        kind = type_list[i].name;
                        break;
                    }
            }

            // 권한 비트마스크를 통해 파일 권한 문자열(rwxrwxrwx) 생성 
            char* perm = (char*)malloc(11);

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


            printf("%10s ", perm); // 파일 접근 권한
            printf("%2u ",dent->d_type); // 파일 종류
            printf("%4s ", getpwuid(statbuf.st_uid)->pw_name); // 파일 UID 기반 유저명
            printf("%4s ", getpwuid(statbuf.st_gid)->pw_name); // GID 기반 유저명 
            printf("%10s ", gettime_str(statbuf.st_atime)); // 파일 접근 시간
           // printf("%10d ", (int)statbuf.st_mtime);    // 파일 수정 시간
           // printf("%10d ", (int)statbuf.st_ctime);    // 파일 생성 시간
            printf("%2o ", (unsigned int)statbuf.st_nlink);    // 하드 링크 수
            printf("%4d ",(unsigned int)statbuf.st_size);
        }
        
        printf("%4s",dent->d_name);
        if(show_long == 1){
            printf("\n");
        }else{
            if(tab_count > 4){
                printf("\n");
                tab_count = 0;
            }else{
                printf("\t");
                ++tab_count;
            }
        }
    }
    closedir(dp); // 사용 끝났으니 닫기 
    return 0;
}

// 하드 링크 생성하는 명령어 
int cmd_ln(int argc, char** argv){
    int opt =0;
    while((opt = getopt(argc, argv, "s")) != -1){
	    switch(opt){
		    case 's': // 심볼릭 링크 생성하는 옵션 
			    if(!argv[2] || !argv[3]){
				    perror("invalid arguments");
				    return 1;
			    }
			    return  symlink(root_path(argv[2]),root_path(argv[3])); 
			    break;
			default:
			    break;
	    }
    }

    if(!argv[1] || !argv[2]){
	    perror("invalid arguments");
	    return 1;
    }

    return link(root_path(argv[1]), root_path(argv[2]));
}

// 링크 삭제하는 명령어 
int cmd_rm(int argc, char** argv){
    return unlink(root_path(argv[1]));
}   

// 기존 파일 권한에 비트마스크를 사용하여 원하는 권한의 비트를 Set 하는 함수 
void set_permission(mode_t *mode, char target, char _operator, const char *perms) {
    int shift = 0;
    // 사용자별로 비트마스크 움직일 offset 설정
    if (target == 'u') shift = 0;     
    else if (target == 'g') shift = 3; 
    else if (target == 'o') shift = 6; 

    // +,- 연산자 이후로 문자를 하나씩 읽도록 포인터 반복
    for (const char *p = perms; *p != '\0'; p++) {
        int perm_bit = 0;
        if (*p == 'r') perm_bit = S_IRUSR >> shift;
        else if (*p == 'w') perm_bit = S_IWUSR >> shift;
        else if (*p == 'x') perm_bit = S_IXUSR >> shift;

        if (_operator == '+') *mode |= perm_bit;    // 권한 비트 on
        else if (_operator == '-') *mode &= ~perm_bit; // 권한 비트 off
    }
}

// 권한 변경하는 명령어 
int cmd_chmod(int argc, char** argv){
    const char* path = root_path(argv[1]);
    const char* perm = argv[2];

    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        perror("undefined file name");
        return -1;
    }
    mode_t mode = statbuf.st_mode;

    if (atoi(perm) != 0) {
        mode = strtol(perm, NULL, 8);  // 8진수 권한으로 변경
    } else {
        if (perm[0] == '+' || perm[0] == '-') {
            // 첫 문자가 +, - 인 경우 : 모든 사용자에 대해 권한 변경 
            char _operator = perm[0];
            set_permission(&mode, 'u', _operator, &perm[1]);
            set_permission(&mode, 'g', _operator, &perm[1]);
            set_permission(&mode, 'o', _operator, &perm[1]);
        } else{
            // 개별적인 사용자(u,g,o)에 대해 권한 변경 
            set_permission(&mode, perm[0], perm[1], &perm[2]);
        }
    }

    return chmod(path, mode);
}

// cat <filename> : 파일 내용 터미널에 출력 
// cat > <filename> : 입력한 내용 파일에 쓰기, 없으면 생성 
int cmd_cat(int argc, char** argv){
    FILE *fp; 
    
    int size;
    // ">" 옵션 있는지 검사
    if(strcmp(argv[1], ">") == 0){
        // 파일 열기 시도
        if((fp = fopen(root_path(argv[2]), "w")) == NULL){
            return -1;
        }
        char** line = (char**)malloc(64);
        int i=0;
        // 표준 스트림으로부터 개행문자 단위로 문자열을 읽어와서 line에 저장 
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
    }else{
        // ">" 옵션 없는 경우 : 그냥 파일 읽어서 보여줌

        // 'r' 읽기 모드라 존재하지 않은 파일일 경우 에러 
        if((fp = fopen(root_path(argv[1]), "r")) == NULL){
            return -1;
        }

        // 파일 크기 계산 
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);

        // 파일에 데이터가 있을 시에만 출력하도록 
        if(size > 0){
            rewind(fp);
            char* buf = (char*)malloc(size+1);

            // 파일로부터 전체 데이터를 읽어서 버퍼에 저장 
            if(fread(buf, 1, size, fp) != NULL){
                printf("%s", buf);
            }
            free(buf);
        }
    }
    
    fclose(fp);

    return 0;
}

// 파일 복사하는 명령어
int cmd_cp(int argc, char** argv){
    FILE *fp;

    char* argv1 = root_path(argv[1]);
    char* argv2 = root_path(argv[2]);

    // 파일 존재하는지 검사 
    if((fp = fopen(argv1, "r")) == NULL){
        perror("undefined file name");
        return -1;
    }

    // 파일 크기 계산 
    fseek(fp, 0 , SEEK_END);
    int size = ftell(fp);
    char* buf = (char*)malloc(size+1);
    rewind(fp);

    // 파일 내용 읽기
    if((fread(buf, 1, size, fp)) != NULL){
        FILE *out;
        if((out = fopen(argv2, "w")) == NULL){ // 쓰기 모드로 복사 대상 파일 열기 (w모드라 없으면 생성됨)
            perror("undefined file name");
            return -1;
        }
        fputs(buf, out);
        fclose(out);
    }

    fclose(fp);
    return 0;
}

// 시그널 목록 출력하는 함수 
void print_sig_list(void){
	int i=0;
	while(sig_list[i] != NULL){
		printf("%s ",sig_list[i]);
		++i;
		if(i % 5 == 0){
			printf("\n");
		}
	}
}

// 지정한 PID의 프로세스로 시그널을 보내는 명령
int cmd_kill(int argc, char** argv){
    char opt;
    int pid;
   
   // kill 명령어는 -9 이렇게 숫자로 옵션을 받기 때문에
   // getopt를 사용하지 않고 파싱하였다. 

   // 옵션이 존재하는지 검사 
    int has_opt = sscanf(argv[1], "-%c", &opt);
   if(has_opt > 0){
		  if(opt == 'l'){
		 	print_sig_list();
			return 0;
		}else{
            // option을 정수로 파싱
            // char opt로는 두 자릿수 정수(ex.15)를 파싱할 수 없기 때문 
			int opti;
			if(sscanf(argv[1], "-%d", &opti) < 0){
				perror("sscanf");
				return 1;
			}
			pid = atoi(argv[2]);
			if(pid == 0){
				perror("pid must be type 'int'");
				return 1;
			}
			return kill(pid, opti);
		}
  }else{
    // 옵션이 존재하지 않으면, 2번째 인자를 pid로, option은 기본 15(SIGTERM)으로 설정 
  	pid = atoi(argv[1]);
	if(pid == 0){
		perror("pid must be type 'int'");
		return 1;
	}
       return kill(pid, 15);
  }
  return 1;
}

// cpu time 구하는 함수
int getcputime(int uptime, int utime, int stime, int starttime){
	int hz = 100;
	
	int cputime = ((utime+stime)/hz)/(uptime-(starttime/hz))*100;

	return cputime;
}

// 현재 실행중인 프로세스의 목록들을 출력하는 명령
int cmd_ps(int argc, char** argv){
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

    while((opt = getopt(argc, argv, "efuxp:")) != -1){
        switch(opt){
            case 'e': // 미구현
                flags[0] = 1;
                break;
            case 'f':
                flags[1] = 1;
                break;
            case 'u':
                flags[2] = 1;
                break;
            case 'x': // 미구현
                flags[3] = 1; 
                break;
            case 'p':
                if(!optarg){
                    perror("No option argument");
                    return 1;
                }
                target_proc = atoi(optarg);
                break;
        }
    }

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
            printf("%2s ", "C");
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
                    printf("%2d ", getcputime(uptime, atoi(field[13]), atoi(field[14]), starttime)); // cpu time
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




// no...
// root 디렉토리를 변경하는 명령
int cmd_chroot(int argc, char** argv){
    // char* path = argv[1];

    
    // if(strcmp(path, "/") == 0){
    //     ROOT_DIR = "/tmp/test";
    // }else {
    //    char* tmp = (char*)malloc(MAX_CMD_SIZE);
    //    if(path[0] != '/'){
    //    	snprintf(tmp, MAX_CMD_SIZE, "%s%c%s",ROOT_DIR,'/', path);
    //   }else{
    //     tmp = root_path(path);
    //    }
    //    printf("chroot: %s\n",tmp);
    //     if(access(tmp,0) == -1){
    //         perror("");
    //         return 1;
    //     }
    // ROOT_DIR = (char*)malloc(MAX_CMD_SIZE);
    // strcpy(ROOT_DIR, tmp);
    // }
    // // 최상위 ROOT 디렉토리를 벗어나지 않게 하기 위해
    // // ROOT 디렉토리 변경 후 자동으로 위치 변경 
    // chdir(ROOT_DIR);

    // return 0;
}

// 프로그램을 실행하는 명령어
int cmd_execute(char** args){
	pid_t pid;
	
    // 자식 프로세스 생성 
	switch(pid = fork()){
		case -1:
			perror("fork");
			return 1;
		case 0: // 자식 프로세스에서 프로그램 실행 
			if(execvp(args[0], args) == -1){
				perror("execvp");
				exit(1);
			}
			break;
		default:
			{
                int status;
                waitpid(pid, &status, 0); // 자식 프로세스 대기
                break;
            }
	}

	return 0;
}


#endif
