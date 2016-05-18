#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 10
#define MAX_LINE 80

char buffer[BUFFER_SIZE][MAX_LINE] = {'\0'};

int count = 0;

void handle_SIGINT(int a) {
    printf("\n");
    for (int i = 0; i < count; i++)
        printf("Command%d->%s\n", i + 1, buffer[i]);
}


void setup(char inputBuffer[], char *args[],int *background) {
    int length; // inputBuffer的长度
    int i,      /* 循环变量 */
        start,  /* 命令的第一个字符位置 */
        ct;     /* 下一个参数存入args[]的位置 */

    ct = 0;

    /* 读入命令行字符，存入inputBuffer */
    length = read(STDIN_FILENO, inputBuffer, MAX_LINE);  //  如果是键盘读入，就直接为length赋值


    //  解析r命令，且将inputBuffer重新赋值为队列中要执行的指令
    if (inputBuffer[0] == 'r' && (inputBuffer[1] == ' ' | inputBuffer[1] == '\n')) {
        if (inputBuffer[1] == '\n') {
            length = strlen(buffer[count - 1]);  //  重新赋值length，否则下面的正常解析里面的length将会是r指令的长度
            for (int x = 0; x < length; x++) {
                inputBuffer[x] = buffer[count - 1][x];
            }
        }
        else if (inputBuffer[1] == ' ') {
            int flag = 0;
            for (int x = count - 1; x >= 0 ; x--) {  //  找到最近输出的那一条指令的位置
                if (buffer[x][0] == inputBuffer[2]) {
                    flag = x;
                    break;
                }
            }
            length = strlen(buffer[flag]);
            for (int y = 0; y < length; y++) {
                inputBuffer[y] = buffer[flag][y];
            }
        }
    }

    //  将指令入队
    if (count == BUFFER_SIZE) {
        for (int j = 0; j < BUFFER_SIZE - 1; j++) {
            for (int k = 0; k < MAX_LINE; k++)
                buffer[j][k] = buffer[j + 1][k];
        }
        for (int t = 0; t < length; t++)
            buffer[BUFFER_SIZE - 1][t] = inputBuffer[t];
    } else {
        for (int i = 0; i < length; i++) {
            buffer[count][i] = inputBuffer[i];
        }
        count++;
    }

    start = -1;
    if (length == 0) exit(0);            /* 输入ctrl+d，结束shell程序 */
    if (length < 0) {
        perror("error reading the command");
        exit(-1);           /* 出错时用错误码-1结束shell */
    }
    for (i = 0; i < length; i++) {
        switch (inputBuffer[i]) {
	    case ' ':
	    case '\t' :               /* 字符为分割参数的空格或制表符(tab)'\t'*/
            if(start != -1) {
                args[ct] = &inputBuffer[start];
                ct++;
            }
            inputBuffer[i] = '\0'; /* 设置 C string 的结束符 */
            start = -1;
            break;

        case '\n':                 /* 命令行结束 */
            if (start != -1) {
                args[ct] = &inputBuffer[start];
                ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* 命令及参数结束 */
            break;

	    default :             /* 其他字符 */
            if (start == -1)
                start = i;
            if (inputBuffer[i] == '&') {
                *background  = 1;          /*置命令在后台运行*/
                inputBuffer[i] = '\0';
            }
        }
    }
    args[ct] = NULL; /* 命令字符数 > 80 */
}

int main(int argc, char* argv[]) {
    char inputBuffer[MAX_LINE];
    int background;
    char* args[MAX_LINE/2 + 1];
    pid_t pid;
    signal(SIGINT,handle_SIGINT);  // 重新给SIGINT信号一个回调函数

    while (1) {
        background = 0;
        printf("COMMAND->");
        fflush(stdout);
        setup(inputBuffer, args, &background);  //  直接改变了args数组
        if ( (pid = fork()) == -1) {
            printf("Fork Error!\n");
        }
        else if (pid == 0) {
            execvp(args[0], args);
            exit(0);
        }
        if (background == 0) {
            wait(0);
        }
    }
}
