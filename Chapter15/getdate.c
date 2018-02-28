#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* 该程序尝试连接到指定主机返回的地址列表中的第一个地址，如果成功，
   就读取daytime服务返回的信息
*/

int main(int argc, char *argv[])
{
	char *host;
	int sockfd;
	int len, result;
	struct sockaddr_in address;
	struct hostent *hostinfo;
	struct servent *servinfo;
	char buffer[128];

	if (argc == 1)
	{
		host = "localhost";
	}
	else
	{
		host = argv[1];
	}

	/* 按照主机名字查找主机的信息 */
	hostinfo = gethostbyname(host);
	if (!hostinfo)
	{
		fprintf(stderr, "no host: %s\n", host);
		exit (1);
	}

	/* 检查主机上是否有daytime服务 */
	servinfo = getservbyname("daytime", "tcp");
	if (!servinfo)
	{
		fprintf(stderr, "no daytime service\n");
		exit (1);
	}
	printf("daytime port is %d\n", ntohs(servinfo->s_port));

	/* 创建一个套接字 */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* 构造address结构体 */
	address.sin_family = AF_INET;
	address.sin_port = servinfo->s_port;
	address.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;
	len = sizeof(address);

	/* 建立连接，并获取有关信息 */
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if (result == -1)
	{
		perror("oops: getdate");
		exit (1);
	}

	result = read(sockfd, buffer, sizeof(buffer));
	buffer[result] = '\0';
	printf("read %d bytes: %s", result, buffer);

	close(sockfd);
	exit (0);
}
