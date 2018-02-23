#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* 该程序是数据生产者，负责创建管道和启动子进程 */

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
			/* 通过sprintf调用把 读取管道数据的文件描述符 保存到一个缓冲区中 */
			sprintf(buffer, "%d", file_pipes[0]);
			/* 通过execl调用来启动pipe4程序，参数如下：
			   ① 要启动的程序
			   ② argv[0]：程序名
			   ③ argv[1]：想让被调用程序去读取的文件描述符
			   ④ (char *)0：终止被调用程序的参数列表
			*/
			(void)execl("pipe4", "pipe4", buffer, (char *)0);
			exit (EXIT_FAILURE);
		}
		/* 如果是父进程 */
		else
		{
			/* 从第二个文件描述符处写数据 */
			data_processed = write(file_pipes[1], some_data, strlen(some_data));
			printf("%d - wrote %d bytes\n", getpid(), data_processed);
		}
	}
	exit (EXIT_SUCCESS);
}
