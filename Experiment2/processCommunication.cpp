#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int flag = 0;

void waiting();

void stop(int dunno);

int main() {
    //  创建两个进程
    int p1,p2;
	//signal(SIGQUIT, SIG_IGN);
	p1 = fork();
	if(p1>0) {
        printf("Process 1 has already created!\n");
        p2 = fork();
        //  在主进程中
        if (p2 > 0) {
            printf("Process 2 has already created!\n");
            flag = 1;
            signal(SIGINT, stop); //  捕捉到C-c信号时，发出信号.
            waiting();
            kill(p1, 16);
            kill(p2, 17);
            wait(0);
            wait(0);
            printf("Parent process is killed!\n");
            exit(0);
        } else {  //  在子进程2中
            flag = 1;
            signal(SIGINT,SIG_IGN);  //  应用全局的
            signal(17, stop); //设置收到信号17时执行stop函数
            waiting();
            printf("\nChild process 2 is killed by parent!\n");
            exit(0);
        }
    } else {  //  在子进程1中
        flag = 1;
        signal(SIGINT,SIG_IGN);
	    signal(16, stop); //设置收到信号16时执行stop函数
        waiting();
		printf("Child process 1 is killed by parent!\n");
        exit(0);
    }
}

void waiting() {
    while (flag != 0);
}

void stop(int dunno) {
    flag = 0;
}
