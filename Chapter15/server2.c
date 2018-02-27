#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	/* 为服务器创建一个未命名套接字 */
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* 命名该套接字 */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.01");
	server_address.sin_port = 9734;
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	/* 创建监听队列 */
	listen(server_sockfd, 5);
	while (1)
	{
		char ch;
		printf("server waiting\n");

		/* 接受一个连接 */
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

		read(client_sockfd, &ch, 1);
		ch++;
		write(client_sockfd, &ch, 1);
		close(client_sockfd);
	}
}
