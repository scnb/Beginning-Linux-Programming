#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/* 该程序是服务器程序，它能同时处理多个客户端的连接请求
   该进程通过为每个客户创建一个子进程来处理多个请求
*/

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
	server_len = sizeof(server_address);
	
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	/* 创建一个连接队列 */
	listen(server_sockfd, 5);

	/* 设置SIGCHLD信号以避免出现僵尸进程 */
	signal(SIGCHLD, SIG_IGN);

	while (1)
	{
		char ch;
		printf("server waiting\n");

		/* 接受客户的连接 */
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

		/* 通过fork调用为该客户创建一个子进程，并判断是在子进程还是父进程中 */
		if (fork() == 0)
		{
			/* 在子进程中 */
			read(client_sockfd, &ch, 1);
			/* 该5秒延时是为了模拟服务器的计算时间或数据库访问时间 */
			sleep(5);
			ch++;
			write(client_sockfd, &ch, 1);
			close(client_sockfd);
			exit (0);
		}
		else
		{
			/* 在父进程中 */
			/* 关闭该客户 */
			close(client_sockfd);
		}
	}
}
