#include "cmd.h"

int cmd_execute(req_msg_t req, res_msg_t* res){
    char path[MAX_PATH_LEN];
    if(append_path(req.cwd, req.args[0], path, 0) != 0){
        perror("append path: target");
        return 1;
    }
	char argv[1][MAX_PATH_LEN];
	snprintf(argv[0], MAX_PATH_LEN, ".%s", path);
	printf("execute: %s\n", argv[0]);
    pid_t pid;
	
    // 자식 프로세스 생성 
	switch(pid = fork()){
		case -1:
			perror("fork");
			return 1;
		case 0: // 자식 프로세스에서 프로그램 실행 
			if(execvp(argv[0], argv) == -1){
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