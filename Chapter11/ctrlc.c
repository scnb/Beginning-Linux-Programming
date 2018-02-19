#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/* 信号处理函数 */
void ouch(int sig)
{
	printf("OUCH! - I got signal %d\n", sig);
	/* 截获一次中断信号之后，将恢复为默认的行为（即终止程序）*/
	(void) signal(SIGINT, SIG_DFL);
}


/* 截获按下Ctrl+C组合键时产生的SIGINT信号 */
int main()
{
	(void) signal(SIGINT, ouch);
	while (1)
	{
		printf("Hello World\n");
		sleep(1);
	}
}
