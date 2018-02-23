#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

/* 使用FIFO实现进程间通信 */
/* 本程序是生产者程序，它在需要时创建管道，然后尽可能
   快的向管道中写入数据
*/

#define FIFO_NAME 	"/tmp/my_fifo"
#define BUFFER_SIZE 	PIPE_BUF
#define TEN_MEG 	(1024*1024*10)

int main()
{
	int pipe_fd;
	int res;
	int open_mode = O_WRONLY;
	int bytes_sent = 0;
	char buffer[BUFSIZ + 1];

	/* 检查FIFO文件是否存在，若不存在则创建之 */
	if (access(FIFO_NAME, F_OK) == -1)
	{
		res = mkfifo(FIFO_NAME, 0777);
		if (res != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
			exit (EXIT_FAILURE);
		}
	}

	printf("Process %d opening FIFO O_WRONLY\n", getpid());
	/* 打开该FIFO文件 */
	pipe_fd = open(FIFO_NAME, open_mode);
	printf("Process %d result %d\n", getpid(), pipe_fd);

	if (pipe_fd != -1)
	{
		/* 通过循环的方式向管道中写入数据 */
		while (bytes_sent < TEN_MEG)
		{
			res = write(pipe_fd, buffer, BUFFER_SIZE);
			if (res == -1)
			{
				fprintf(stderr, "Write error on pipe\n");
				exit (EXIT_FAILURE);
			}
			bytes_sent += res;
		}
		(void)close(pipe_fd);
	}
	else
	{
		exit (EXIT_FAILURE);
	}

	printf("Process %d finished\n", getpid());
	exit (EXIT_SUCCESS);
}
