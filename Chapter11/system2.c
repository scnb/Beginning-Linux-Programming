#include <stdlib.h>
#include <stdio.h>

int main()
{
	printf("Running ps with system\n");
	/* system函数的作用是：把传进去的字符串参数作为命令，并运行之，等待该命令运行完成。*/
	/* 在下面这条命令中，加上了&，则对system函数的调用将在shell命令结束后立刻返回 */
	system("ps ax &");
	printf("Done\n");
	exit (0);
}
