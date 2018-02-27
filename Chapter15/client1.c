#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>

int main()
{
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	char ch = 'A';

	/* 为客户创建一个套接字 */
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	/* 根据服务器的情况给套接字命名 */
	address.sun_family = AF_UNIX;
	/* 服务器的套接字名为 server_socket */
	strcpy(address.sun_path, "server_socket");
	len = sizeof(address);

	/* 将客户端的套接字连接到服务器的套接字 */
	result = connect(sockfd, (struct sockaddr *)&address, len);

	if (result == -1)
	{
		perror("oops: client1");
		exit (1);
	}

	/* 通过sockfd进行读写操作 */
	/* 先通过套接字向服务器写一个字符，然后在读取服务器返回的字符，最后在终端显示该字符 */
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("char from server = %c\n", ch);
	close(sockfd);
	exit (0);
}
