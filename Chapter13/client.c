#include "client.h"
#include <ctype.h>

int main()
{
	int server_fifo_fd, client_fifo_fd;
	struct data_to_pass_st my_data;
	int times_to_send;
	char client_fifo[256];

	/* 以只写的阻塞模式打开服务器管道的文件描述符 */
	server_fifo_fd = open(SERVER_FIFO_NAME, O_WRONLY);
	if (server_fifo_fd == -1)
	{
		fprintf(stderr, "Sorry, no server\n");
		exit (EXIT_FAILURE);
	}

	/* 客户端程序获取自己的PID */
	my_data.client_pid = getpid();
	/* 将CLIENT_FIFO_NAME和client_pid结合起来 */
	sprintf(client_fifo, CLIENT_FIFO_NAME, my_data.client_pid);
	if (mkfifo(client_fifo, 0777) == -1)
	{
		fprintf(stderr, "Sorry, can't make %s\n", client_fifo);
		exit (EXIT_FAILURE);
	}

	/* 在每次循环中，客户将数据发送给服务器，然后打开客户FIFO并读回数据 */
	for (times_to_send = 0;times_to_send < 5;times_to_send++)
	{
		/* 通过sprintf组成一个包含客户pid的字符串，然后发送 */
		sprintf(my_data.some_data, "Hello from %d", my_data.client_pid);
		/* 通过printf在终端上显示 某个PID发送了某个字符串 */
		printf("%d sent %s, ", my_data.client_pid, my_data.some_data);
		/* 通过write函数，向服务器管道文件描述符写字符串 */
		write(server_fifo_fd, &my_data, sizeof(my_data));
		/* 打开客户管道 */
		client_fifo_fd = open(client_fifo, O_RDONLY);
		if (client_fifo_fd != -1)
		{
			/* 通过客户管道文件描述符读取数据 */
			if (read(client_fifo_fd, &my_data, sizeof(my_data)) > 0)
			{
				printf("received: %s\n", my_data.some_data);
			}
			close(client_fifo_fd);
		}
	}
	/* 关闭服务器管道，删除客户FIFO文件，并退出 */
	close(server_fifo_fd);
	unlink(client_fifo);
	exit (EXIT_SUCCESS);
}
