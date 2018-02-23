#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* 在不同的进程之间，进行数据读写操作 */

int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "123";
	char buffer[BUFSIZ + 1];
	pid_t fork_result;

	memset(buffer, '\0', sizeof(buffer));
	/* 调用pipe函数在file_pipes中填充两个文件描述符 */
	if (pipe(file_pipes) == 0)
	{
		/* 创建一个新进程 */
		fork_result = fork();
		if (fork_result == -1)
		{
			fprintf(stderr, "Fork failure");
			exit (EXIT_FAILURE);
		}

		/* 如果是子进程 */
		if (fork_result == 0)
		{
			/* 从第一个文件描述符处读数据 */
			data_processed = read(file_pipes[0], buffer, BUFSIZ);
			printf("Read %d bytes: %s\n", data_processed, buffer);
			exit (EXIT_SUCCESS);
		}
		/* 如果是父进程 */
		else
		{
			/* 从第二个文件描述符处写数据 */
			data_processed = write(file_pipes[1], some_data, strlen(some_data));
			printf("Wrote %d bytes\n", data_processed);
		}
	}
	exit (EXIT_SUCCESS);
}
