#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

//一个使用open、read和write等系统调用完成的底层复制程序，用来逐个字符的把一个文件复制到另一个文件

int main()
{
	char c;
	int in, out;
	in = open("file.in", O_RDONLY);
	out = open("file.out", O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	while(read(in,&c,1) == 1)
	{
		write(out,&c,1);
	}
	exit (0);
}
