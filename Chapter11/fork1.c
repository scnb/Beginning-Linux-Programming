#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


/* 在父进程中的fork调用返回的是新的子进程的PID
   而子进程中的fork调用返回的是0
   父子进程可以通过这一点来判断究竟谁是父进程，谁是子进程。
*/

int main()
{
	pid_t pid;
	char *message;
	int n;

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
			break;
		default:
			message = "This is the parent";
			n = 3;
			break;
	}
	
	for (; n > 0; n--)
	{
		puts(message);
		sleep(1);
	}
	exit (0);
}
