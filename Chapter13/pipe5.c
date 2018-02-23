#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 父进程向管道中写数据
   子进程中把标准输入与管道的读数据端相连，这样就可以直接从管道读取标准输入的数据 */

int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "123";
	pid_t fork_result;

	/* 调用pipe函数填充file_pipes数组中的两个文件描述符 */
	if (pipe(file_pipes) == 0)
	{
		/* 创建新进程 */
		fork_result = fork();
		if (fork_result == (pid_t)-1)
		{
			fprintf(stderr, "Fork failed");
			exit (EXIT_FAILURE);
		}

		/* 如果是子进程 */
		if (fork_result == (pid_t)0)
		{
			/* 关闭标准输入 */
			close(0);
			/* 调用dup函数打开一个新的文件描述符，且该文件描述符是最小的数字0，即将标准输入与file_pipes[0]指向同一个文件 */
			dup(file_pipes[0]);
			/* 子进程关闭 原先的 用来从管道读取数据的文件描述符file_pipes[0] */
			close(file_pipes[0]);
			/* 子进程关闭 用来向管道写数据的文件描述符file_pipes[1] */
			close(file_pipes[1]);
			execlp("od", "od", "-c", (char *)0);
			exit (EXIT_FAILURE);
		}
		/* 如果是主进程 */
		else
		{
			/* 因为父进程不会从管道读数据，所以关闭读数据的文件描述符 */
			close(file_pipes[0]);
			/* 父进程向管道写入数据 */
			data_processed = write(file_pipes[1], some_data, strlen(some_data));
			/* 父进程关闭管道的写入端，并退出 */
			close(file_pipes[1]);
			printf("%d - wrote %d bytes\n", (int)getpid(), data_processed);
		}
	}
	exit (EXIT_SUCCESS);
}
