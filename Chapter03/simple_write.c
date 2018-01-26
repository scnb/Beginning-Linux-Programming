#include <unistd.h>
#include <stdlib.h>

//Linux库函数中write函数的原型：
/*
	size_t write(int fildes, const void *buf, size_t nbytes);
	fildes：某文件的文件描述符
	buf：缓冲区
	nbytes：要写入的字节数
*/

int main()
{
	if ((write(1,"Here is some data\n",18))!=18)
	{
		write(2,"A write error has occurred on file descripter 1\n",46);
	}
	exit (0);
}
