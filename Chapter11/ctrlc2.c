#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig)
{
	printf("OUCH! - I got signal %d\n", sig);
}


int main()
{
	struct sigaction act;

	act.sa_handler = ouch;
	/* 将sa_mask（信号屏蔽字）初始化为0 */
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	/* sigaction连续处理到来的SIGINT信号 */
	sigaction(SIGINT, &act, 0);

	while (1)
	{
		printf("Hello World!\n");
		sleep(1);
	}
}
