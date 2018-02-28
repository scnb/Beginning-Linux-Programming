#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

/* 该程序让select函数同时检查（监听套接字）和（客户的连接套接字），
   一旦select函数发现有活动发生，就调用FD_ISSET函数遍历所有的文件
   描述符，以检查是哪个文件描述符上有活动发生
*/

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int result;
	fd_set readfds, testfds;

	/* 为服务器创建并命名一个套接字 */
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	/* 创建一个连接队列，初始化readfds以处理来自server_sockfd的输入 */
	listen(server_sockfd, 5);

	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);

	/* 开始等待客户和请求的到来 */
	while (1)
	{
		char ch;
		int fd;
		int nread;

		testfds = readfds;

		printf("server waiting\n");
		result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);

		if (result < 1)
		{
			perror("server5");
			exit (1);
		}

		/* 用FD_ISSET来依次检测每个描述符，以发现活动发生在哪个描述符上 */
		for (fd = 0; fd < FD_SETSIZE; fd++)
		{
			if (FD_ISSET(fd, &testfds))
			{
				/* 如果活动发生在套接字server_sockfd上，则表明是一个新的连接请求 */
				if (fd == server_sockfd)
				{
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
					FD_SET(client_sockfd, &readfds);
					printf("adding client on fd %d\n", client_sockfd);
				}
				/* 否则 就是客户的活动　*/
				else
				{
					ioctl(fd, FIONREAD, &nread);

					/* 若客户没有发送任何字节，则表明客户要退出，则执行close */
					if (nread == 0)
					{
						close(fd);
						FD_CLR(fd, &readfds);
						printf("removing client on fd %d\n", fd);
					}
					/* 否则就将客户发送的字符加1再写回给客户 */
					else
					{
						read(fd, &ch, 1);
						sleep(5);
						printf("serving client on fd %d\n", fd);
						ch++;
						write(fd, &ch, 1);
					}
				}
			}
		}
	}
}
