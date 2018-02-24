#include "client.h"
#include <ctype.h>

/* 本程序是服务器程序，创建并打开服务器管道，该管道被设置为只读的阻塞模式 */

int main()
{
	int server_fifo_fd, client_fifo_fd;
	struct data_to_pass_st my_data;
	int read_res;
	char client_fifo[256];
	char *tmp_char_ptr;

	mkfifo(SERVER_FIFO_NAME, 0777);
	
	/* 以只读的阻塞模式打开服务器管道 */
	server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);
	if (server_fifo_fd == -1)
	{
		fprintf(stderr, "Server fifo failure\n");
		exit (EXIT_FAILURE);
	}

	sleep(10);
	do
	{
		/* 读取客户发送来的数据 */
		read_res = read(server_fifo_fd, &my_data, sizeof(my_data));
		if (read_res > 0)
		{
			/* 将该临时指针指向结构体中的字符串 */
			tmp_char_ptr = my_data.some_data;
			/* 通过循环的方式，将字符串中的每个字符都转为大写 */
			while (*tmp_char_ptr)
			{
				*tmp_char_ptr = toupper(*tmp_char_ptr);
				tmp_char_ptr++;
			}
			/* 通过sprintf把CLIENT_FIFO_NAME和接收到额client_pid结合在一起 */
			sprintf(client_fifo, CLIENT_FIFO_NAME, my_data.client_pid);

			/* 以只写的阻塞模式打开客户管道，把处理后的数据发送回去 */
			client_fifo_fd = open(client_fifo, O_WRONLY);
			if (client_fifo_fd != -1)
			{
				/* 将处理后的数据通过客户管道写回去 */
				write(client_fifo_fd, &my_data, sizeof(my_data));
				close(client_fifo_fd);
			}
		}
	} while (read_res > 0);
	/* 关闭服务器管道的文件描述符，删除FIFO文件，然后退出 */
	close(server_fifo_fd);
	unlink(SERVER_FIFO_NAME);
	exit (EXIT_SUCCESS);
}
