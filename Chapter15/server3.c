#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* 该程序中有一个关于硬件的微小错误：即Intel和IBMPowerPC的大小端表示
   不同的计算机使用不同的字节序来表示整数
   Intel		32位 int 4字节 字节序：1-2-3-4
   IBMPowerPC	32位 int 4字节 字节序：4-3-2-1
*/

/* Note：上面的BUG已修复
	unsigned long int htonl(unsigned long int hostlong);
	长整数从主机字节序到网络字节序的转换
	unsigned short int htons(unsigned short int hostshort);
	短整数从主机字节序到网络字节序的转换
*/


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
	/* INADDR_ANY表示允许到达服务器任一网络接口的连接 */
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
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
