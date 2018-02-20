#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

static int alarm_fired = 0;

void ding(int sig)
{
	alarm_fired = 1;
}


int main()
{
	pid_t pid;
	printf("Alarm application starting\n");

	pid = fork();
	/* 子进程在等待5秒后发送一个SIGALRM信号给它的父进程 */
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit (1);
		case 0:
			sleep(5);
			kill(getppid(),SIGALRM);
			exit (0);
	}

	/* 父进程通过signal调用捕获SIGALRM信号 */
	printf("waiting for alarm to go off\n");
	/* 设置当捕获SIGALRM信号时，调用ding函数来处理该信号 */
	(void) signal(SIGALRM, ding);
	/* 下面的sleep语句可也模拟“竟态条件”，即信号到来的时间出现在pause函数执行之前，则pause函数会一直等待下去 */
	//sleep(2);
	/* 调用pause函数将父进程挂起，直到有一个信号出现为止 */
	pause();
	if (alarm_fired)
	{
		printf("Ding\n");
	}
	printf("Done.\n");
	exit (0);
}
