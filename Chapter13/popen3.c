#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 本程序演示当文件流中的数据过多时（超过BUFSIZ个），应该分块读取数据 */

int main()
{
	FILE *read_fp;
	char buffer[BUFSIZ + 1];
	int chars_read;

	memset(buffer, '\0', sizeof(buffer));
	read_fp = popen("ps ax", "r");
	if (read_fp != NULL)
	{
		/* 连续从文件流中读取数据，直到没有字符可以读 */ 
		chars_read = fread(buffer, sizeof(char), BUFSIZ / 10, read_fp);
		while (chars_read > 0)
		{
			buffer[chars_read - 1] = '\0';
			printf("Reading %d:-\n %s\n", BUFSIZ, buffer);
			chars_read = fread(buffer, sizeof(char), BUFSIZ / 10, read_fp);
		}
		pclose(read_fp);
		exit (EXIT_SUCCESS);
	}
	exit (EXIT_FAILURE);
}
