#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "123";
	char buffer[BUFSIZ + 1];

	memset(buffer, '\0', sizeof(buffer));

	/* 调用pipe函数在file_pipes数组中填上两个新的文件描述符 */
	if (pipe(file_pipes) == 0)
	{
		/* 将数据写到file_pipes[1]中文件描述符中 */
		data_processed = write(file_pipes[1], some_data, strlen(some_data));
		printf("Wrote %d bytes\n", data_processed);
		/* 从file_pipes[0]中文件描述符以FIFO的方式读出数据 */
		data_processed = read(file_pipes[0], buffer, BUFSIZ);
		printf("Read %d bytes: %s\n", data_processed, buffer);
		exit (EXIT_SUCCESS);
	}
	exit (EXIT_FAILURE);
}
