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
	/* 不需要对函数调用inet_addr进行转换，因为该函数已被定义为产生一个网络字节序 */
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9734);
	len = sizeof(address);

	/* 将客户端套接字与服务器套接字相连接 */
	result = connect(sockfd, (struct sockaddr *)&address, len);
	
	if (result == -1)
	{
		perror("oops:client3");
		exit (1);
	}

	/* 通过套接字进行读写操作 */
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("char from server = %c\n", ch);
	close(sockfd);
	exit (0);
}
