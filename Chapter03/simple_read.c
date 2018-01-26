#include <unistd.h>
#include <stdlib.h>

/*
	Linux系统库函数中read函数的原型：
	size_t read(int fildes, void *buf, size_t nbytes);
	fildes：某文件的文件描述符
	buf：缓冲区
	nbytes：要读入的字节数
*/

int main()
{
	char buffer[128];
	int nread;

	nread = read(0,buffer,128);
	if (nread == -1)	//判断read函数是否返回值为-1，即是否出错
	{
		write(2,"A read error has occurred\n",26);
	}

	if ((write(1,buffer,nread)) != nread)	//将读入的数据写入到标准输出
	{
		write(2,"A write error has occurred\n",26);
	}

	exit (0);
}
