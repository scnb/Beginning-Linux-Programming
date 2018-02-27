#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;

	/* 删除以前的套接字，为服务器创建一个未命名的套接字 */
	unlink("server socket");
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	/* 命名套接字 */
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	/* 调用bind函数来给套接字绑定一个名字 */
	/* 把server_address中的信息与server_sockfd相关联 */
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	/* 创建一个监听队列， 开始等待客户进行连接 */
	listen(server_sockfd, 5);
	while (1)
	{
		char ch;
		printf("server waiting\n");
		/* 接受一个连接 */
		client_len = sizeof(client_address);

		/* 处理排在套接字队列中的第一个未处理连接，accept函数创建一个新套接字来与该客户进行通信 */
		/* 连接客户的地址被放入client_address结构中 */
		client_sockfd = accept(server_sockfd, (struct address *)&client_address, &client_len);

		/* 对client_sockfd套接字上的客户进行读写操作 */
		read(client_sockfd, &ch, 1);
		ch++;
		write(client_sockfd, &ch, 1);
		close(client_sockfd);
	}
}
