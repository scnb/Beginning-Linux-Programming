#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

/* 该程序读取键盘（标准输入），超时时间设为2.5秒
   该程序只在有输入就绪时才读取键盘
*/

int main()
{
	char buffer[128];
	int result, nread;

	/* fd_set：一种数据结构，已打开的文件描述符的集合 */
	fd_set inputs, testfds;
	struct timeval timeout;

	FD_ZERO(&inputs);
	/* 将文件描述符集合设置为标准输入stdin */
	FD_SET(0, &inputs);

	/* 在标准输入上最多等待2.5秒 */
	while (1)
	{
		testfds = inputs;
		timeout.tv_sec = 2;
		timeout.tv_usec = 500000;

		result = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL, &timeout);
		
		/* 对result进行测试：
		   若没有输入，则程序循环；
		   若出现一个错误，则程序退出;
		   否则，程序就读取标准输入stdin上的输入，并在接受到行尾字符后把他们都回显到屏幕上
		*/
		switch(result)
		{
			case 0:
				printf("timeout\n");
				break;
			case -1:
				perror("select");
				exit (1);
			default:
				/* 调用FD_ISSET函数来检测哪些文件描述符发生了变化 */
				if (FD_ISSET(0, &testfds))
				{
					/* 得到标准输入里有多少字节需要读取，然后将该数值放到nread中 */
					ioctl(0, FIONREAD, &nread);
					if (nread == 0)
					{
						printf("keyboard done\n");
						exit (0);
					}
					/* 通过read函数读取标准输入中的字节 */
					nread = read(0, buffer, nread);
					buffer[nread] = 0;
					printf("read %d from keyboard: %s", nread, buffer);
				}
				break;
		}
	}
}
