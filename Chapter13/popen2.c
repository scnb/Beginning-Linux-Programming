#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 本程序用带有参数"w"的popen命令启动od -c命令，然后向od -c命令发送一个字符串，
   该命令接受字符串后，处理并将结果打印到屏幕上 
*/

int main()
{
	FILE *write_fp;
	char buffer[BUFSIZ + 1];

	sprintf(buffer, "Once upon a time, there was ……\n");
	/* 调用程序（当前程序）可以用fwrite函数向被调用程序（od -c）发送数据，而被调用程序可以在自己的标准输入上读取这些数据 */
	write_fp = popen("od -c", "w");
	if (write_fp != NULL)
	{
		fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
		pclose(write_fp);
		exit (EXIT_SUCCESS);
	}
	exit (EXIT_FAILURE);
}
