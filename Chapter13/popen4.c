#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 在本程序中，在一个popen调用中启动了shell、cat命令和wc命令，并进行了一次输出重定向 */
/* 该程序的目的是统计当前目录下所有popen*.c文件的总行数 */

int main()
{
	FILE *read_fp;
	char buffer[BUFSIZ + 1];
	int chars_read;
	
	memset(buffer, '\0', sizeof(buffer));
	read_fp = popen("cat popen*.c | wc -l", "r");
	if (read_fp != NULL)
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
		while (chars_read > 0)
		{
			buffer[chars_read - 1] = '\0';
			printf("Reading:-\n %s\n", buffer);
			chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
		}
		pclose(read_fp);
		exit (EXIT_SUCCESS);
	}
	exit (EXIT_FAILURE);
}
