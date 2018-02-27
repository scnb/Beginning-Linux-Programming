#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* 该程序是客户端程序，它通过回路网络（lookback）连接到一个网络套接字 
   该程序试图连接到IP地址为127.0.0.1的主机上的服务器
*/

/* Note：该程序有一个与硬件有关的细微错误 */

int main()
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch = 'A';

	/* 为客户端创建一个套接字 */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* 为服务器套接字命名 */
	address.sin_family = AF_INET;
	/* 使用inet_addr函数将IP地址的文本表示方式转换为符合套接字地址要求的格式 */
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = 9734;
	len = sizeof(address);

	/* 将客户端套接字与服务器套接字相连接 */
	result = connect(sockfd, (struct sockaddr *)&address, len);
	
	if (result == -1)
	{
		perror("oops:client2");
		exit (1);
	}

	/* 通过套接字进行读写操作 */
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("char from server = %c\n", ch);
	close(sockfd);
	exit (0);
}
