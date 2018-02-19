#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* wait函数的作用：暂停父进程直到它的子进程结束为止
   该调用返回子进程的PID，它通常是已结束运行的子进程的PID
*/

/* 在父进程中的fork调用返回的是新的子进程的PID
   而子进程中的fork调用返回的是0
   父子进程可以通过这一点来判断究竟谁是父进程，谁是子进程。
*/

int main()
{
	pid_t pid;
	char *message;
	int n;
	int exit_code;

	printf("fork program starting\n");
	pid = fork();
	/* 在上一语句调用fork之后，程序被分为两个独立的进程 */
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit (1);
		case 0:
			message = "This is the child";
			n = 5;
			exit_code = 37;
			break;
		default:
			message = "This is the parent";
			n = 3;
			exit_code = 0;
			break;
	}
	
	for (; n > 0; n--)
	{
		puts(message);
		sleep(1);
	}

	/* 下面这段代码由父进程执行 */
	if (pid != 0)
	{
		int stat_val;
		pid_t child_pid;

		child_pid = wait(&stat_val);
		printf("Child has finished: PID = %d\n", child_pid);
		/* 如果子进程正常退出，则WIFEXITED(stat_val)就取一个非零值 */
		if (WIFEXITED(stat_val))
		{
			/* 如果WIFEXITED非零，则返回子进程的退出码 */
			printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
		}
		else
		{
			printf("Child terminated abnormally\n");
		}
	}
	exit (exit_code);
}
