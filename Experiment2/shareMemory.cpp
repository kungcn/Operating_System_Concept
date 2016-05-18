#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <sstream>

#define PERM S_IRUSR|S_IWUSR
using namespace std;

string fib(int n) {
    string result = " ";
    int f1 = 0, f2 = 1, tmp = 0;
    if (n == 1) {
        result = "0\n";
    }
    else if (n == 2) {
        result = "0,1\n";
    }
    else if (n > 2) {
        ostringstream ss;
        result = "0,1,";
        for (int i = 3; i < n; i++) {
            tmp = f1 + f2;
            f1 = f2;
            f2 = tmp;
            ss << f2 << ",";
        }
        ss << f1 + f2;
        result += ss.str();
    }
    return result;
}

int main(int argc, char **argv) {
    key_t shmid;
    char *p_addr, *c_addr;
    pid_t pid;
    int N;
    printf("Please input the number:");
    scanf("%d", &N);

    //  检测程序是否输入正确且创建共享内存的空间
    //if(argc != 2) {
    //    fprintf(stderr, "Usage:%s\n\a", argv[0]);
    //    exit(1);
    //}

    if( (shmid = shmget(IPC_PRIVATE, 1024, PERM)) == -1 )   { //(见注1)
        fprintf(stderr, "Create Share Memory Error:%s\n\a", strerror(errno));
        exit(1);
    }

    //  子进程生成斐波拉契数列
    pid = fork();
    //  主进程
    if(pid > 0) {
        sleep(1);
        p_addr = (char*)shmat(shmid, 0, 0);
        printf("Client get: %s\n", p_addr);
        wait();
        exit(0);
    }
    //  子进程
    else if (pid == 0) {
        c_addr = (char*)shmat(shmid, 0, 0);
        memset(c_addr, '\0', 1024);
        strncpy(c_addr, fib(N).c_str(), 1024);
        exit(0);
        fib(N);
    } else {
        printf("Create Child Process Error!\n");
        exit(-1);
    }
}
