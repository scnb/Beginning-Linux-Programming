#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 本程序用popen调用启动带有-a选项的uname命令，然后用返回的文件流读取最多BUFSIZ个字符的数据，并将其显示在屏幕上 */

int main()
{
	FILE *read_fp;
	/* BUFSIZ在stdio.h中，由#define语句定义 */
	char buffer[BUFSIZ + 1];
	int chars_read;
	/* 将buffer全部写NULL */
	memset(buffer, '\0', sizeof(buffer));
	/* 被调用程序（uname -a）的输出被调用程序（当前程序）使用 */
	read_fp = popen("uname -a", "r");
	if (read_fp != NULL)
	{
		/* 从返回的文件流中读取最多BUFSIZ个字符 */
		chars_read = fread(buffer, sizeof(char), BUFSIZ , read_fp);
		if (chars_read > 0)
		{		
			printf("Output was:-\n%s\n", buffer);
		}
		/* 关闭与之关联的文件流 */
		pclose(read_fp);
		exit (EXIT_SUCCESS);
	}
	exit (EXIT_FAILURE);
}
